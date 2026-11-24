using System.Collections.Generic;
using UnityEngine;

namespace FootballLegends.Match
{
    /// <summary>
    /// Rule-based team AI with pass selection, attacking runs and zonal marking.
    /// Tactical choices are refreshed periodically; movement remains frame-based.
    /// </summary>
    public sealed class TacticalPlayerAI : MonoBehaviour
    {
        private MatchPlayer player;
        private IReadOnlyList<MatchPlayer> teammates;
        private IReadOnlyList<MatchPlayer> opponents;
        private Transform ball;
        private Rigidbody ballBody;
        private int attackDirection;
        private Vector3 home;
        private Vector3 targetPosition;
        private Vector3 facingTarget;
        private float thinkTimer;
        private float moveSpeed;
        private float nextPassTime;
        private bool carryingBall;
        private bool targetReady;

        public void Configure(MatchPlayer owner, IReadOnlyList<MatchPlayer> team,
            IReadOnlyList<MatchPlayer> opposition, Transform matchBall, int direction, Vector3 homePosition)
        {
            player = owner;
            teammates = team;
            opponents = opposition;
            ball = matchBall;
            ballBody = matchBall != null ? matchBall.GetComponent<Rigidbody>() : null;
            attackDirection = direction >= 0 ? 1 : -1;
            home = homePosition;
            targetPosition = homePosition;
            facingTarget = matchBall != null ? matchBall.position : homePosition + Vector3.forward * attackDirection;
            targetReady = true;
            nextPassTime = Time.time + Random.Range(0.7f, 1.4f);
            thinkTimer = Random.Range(0f, 0.15f);
        }

        private void Update()
        {
            if (MatchBootstrap.SimulationPaused || player == null || player.IsSentOff ||
                player.IsComplaining || ball == null || teammates == null || opponents == null)
                return;

            thinkTimer -= Time.deltaTime;
            if (thinkTimer <= 0f)
            {
                RecalculateDecision();
                thinkTimer = Random.Range(0.16f, 0.23f);
            }

            if (!targetReady) return;

            Vector3 before = player.transform.position;
            player.MoveTo(targetPosition, moveSpeed);
            player.Face(facingTarget);

            if (carryingBall && ballBody != null && !ballBody.isKinematic)
            {
                ballBody.velocity = Vector3.zero;
                ballBody.angularVelocity = Vector3.zero;
                ball.position = new Vector3(player.transform.position.x, 0.35f,
                    player.transform.position.z + attackDirection * 0.88f);
            }

            float actualSpeed = (player.transform.position - before).magnitude /
                Mathf.Max(Time.deltaTime, 0.001f);
            player.UpdateAnimation(actualSpeed);
        }

        private void RecalculateDecision()
        {
            Vector3 ballPosition = ball.position;
            MatchPlayer closestTeammate = FindClosest(teammates, ballPosition, out float ownDistance);
            FindClosest(opponents, ballPosition, out float opponentDistance);

            bool teamHasPossession = ownDistance < 2.5f &&
                ownDistance <= opponentDistance + 0.65f;
            bool opponentHasPossession = opponentDistance < 2.5f &&
                opponentDistance < ownDistance - 0.2f;

            MatchTacticsDirector director = MatchTacticsDirector.Instance;
            MatchPlayer userPlayer = director != null ? director.GetUserControlledPlayer(player.TeamId) : null;
            bool userPlayerHasBall = userPlayer != null &&
                Vector3.Distance(userPlayer.transform.position, ballPosition) < 2.25f;
            if (director != null)
            {
                MatchTacticsDirector.TacticalPhase phase = director.GetPhase(player.TeamId);
                teamHasPossession = phase == MatchTacticsDirector.TacticalPhase.Attacking ||
                    phase == MatchTacticsDirector.TacticalPhase.CounterAttack;
                opponentHasPossession = phase == MatchTacticsDirector.TacticalPhase.Defending;
            }

            carryingBall = false;
            moveSpeed = 4.25f;
            facingTarget = ballPosition;
            Vector3 target = home;

            if (player.IsGoalkeeper)
            {
                float goalZ = -attackDirection * 25.5f;
                float keeperX = Mathf.Clamp(ballPosition.x * 0.62f, -3.25f, 3.25f);
                float depth = Mathf.Clamp(attackDirection * ballPosition.z, -2f, 7f);
                target = new Vector3(keeperX, player.transform.position.y,
                    goalZ + attackDirection * depth * 0.16f);
                moveSpeed = 5.1f;
            }
            else if (closestTeammate == player && !opponentHasPossession)
            {
                target = ballPosition + Vector3.forward * -attackDirection * 0.72f;
                moveSpeed = 5.0f;

                if (teamHasPossession && Vector3.Distance(player.transform.position, ballPosition) < 2.35f)
                {
                    if (!TryExecutePass(ballPosition))
                    {
                        carryingBall = true;
                        target = ChooseDribbleTarget(ballPosition);
                        moveSpeed = 4.65f;
                        facingTarget = target + Vector3.forward * attackDirection * 5f;
                    }
                }
            }
            else if (opponentHasPossession)
            {
                int pressRank = MatchTacticsDirector.Instance != null
                    ? MatchTacticsDirector.Instance.GetPressRank(player.TeamId, player, ballPosition)
                    : GetRankByDistance(teammates, ballPosition, player);
                if (pressRank == 1)
                {
                    target = ballPosition + Vector3.forward * -attackDirection * 0.9f;
                    moveSpeed = 4.9f;
                }
                else if (pressRank == 2)
                {
                    target = ballPosition + Vector3.forward * -attackDirection * 3.1f;
                    target.x = Mathf.Lerp(home.x, ballPosition.x, 0.38f);
                    moveSpeed = 4.55f;
                }
                else
                {
                    target = DefensiveShapeTarget(ballPosition);
                }
            }
            else if (teamHasPossession)
            {
                target = userPlayerHasBall
                    ? SupportControlledCarrier(userPlayer, ballPosition)
                    : AttackingSupportTarget(ballPosition);
                moveSpeed = player.ShirtNumber >= 9 ? 4.95f : 4.5f;
                facingTarget = userPlayerHasBall ? userPlayer.transform.position : ballPosition;
            }
            else
            {
                target = DefensiveShapeTarget(ballPosition);
            }

            target.y = player.transform.position.y;
            target.x = Mathf.Clamp(target.x, -16.2f, 16.2f);
            target.z = Mathf.Clamp(target.z, -27f, 27f);
            targetPosition = target;
            targetReady = true;
        }

        private bool TryExecutePass(Vector3 ballPosition)
        {
            if (ballBody == null || ballBody.isKinematic || Time.time < nextPassTime)
                return false;

            MatchPlayer receiver = ChoosePassReceiver(ballPosition, out float bestScore);
            if (receiver == null || bestScore < 1.15f)
                return false;

            Vector3 target = receiver.transform.position;
            Vector3 flatDirection = target - ballPosition;
            flatDirection.y = 0f;
            if (flatDirection.sqrMagnitude < 0.01f) return false;

            OffsideResult offside = OffsideRuleSystem.Evaluate(receiver, ballPosition,
                BuildPlayerSnapshot(), player.TeamId, attackDirection);
            if (offside.IsOffside) return false;

            flatDirection.Normalize();
            ballBody.velocity = Vector3.zero;
            ballBody.angularVelocity = Vector3.zero;
            ballBody.AddForce(flatDirection * 8.8f + Vector3.up * 0.9f, ForceMode.Impulse);
            player.Face(receiver.transform.position);
            player.TriggerKick();
            carryingBall = false;
            nextPassTime = Time.time + Random.Range(1.8f, 3.0f);
            return true;
        }

        private IReadOnlyList<MatchPlayer> BuildPlayerSnapshot()
        {
            List<MatchPlayer> all = new List<MatchPlayer>(teammates.Count + opponents.Count);
            for (int i = 0; i < teammates.Count; i++) all.Add(teammates[i]);
            for (int i = 0; i < opponents.Count; i++) all.Add(opponents[i]);
            return all;
        }

        private MatchPlayer ChoosePassReceiver(Vector3 ballPosition, out float bestScore)
        {
            MatchPlayer best = null;
            bestScore = float.MinValue;

            for (int i = 0; i < teammates.Count; i++)
            {
                MatchPlayer candidate = teammates[i];
                if (candidate == null || candidate == player || candidate.IsGoalkeeper ||
                    candidate.IsSentOff || candidate.IsComplaining) continue;

                Vector3 offset = candidate.transform.position - ballPosition;
                float distance = offset.magnitude;
                if (distance < 3.2f || distance > 19f) continue;

                OffsideResult offside = OffsideRuleSystem.Evaluate(candidate, ballPosition,
                    BuildPlayerSnapshot(), player.TeamId, attackDirection);
                if (offside.IsOffside) continue;

                float forwardProgress = attackDirection * offset.z;
                float nearestOpponent = NearestOpponentDistance(candidate.transform.position);
                float laneClearance = PassLaneClearance(ballPosition, candidate.transform.position);
                float score = 2.0f;
                score += Mathf.Clamp(forwardProgress / 7f, -1.1f, 1.8f);
                score += Mathf.Clamp((nearestOpponent - 1.2f) * 0.42f, -1.0f, 1.5f);
                score += Mathf.Clamp((laneClearance - 0.65f) * 0.65f, -1.4f, 1.2f);
                score -= Mathf.Abs(offset.x) / 22f;
                score -= Mathf.Abs(distance - 9f) / 24f;

                MatchTacticsDirector director = MatchTacticsDirector.Instance;
                MatchPlayer userPlayer = director != null
                    ? director.GetUserControlledPlayer(player.TeamId) : null;
                if (candidate == userPlayer)
                    score += 2.6f;

                if (score > bestScore)
                {
                    bestScore = score;
                    best = candidate;
                }
            }
            return best;
        }

        private float NearestOpponentDistance(Vector3 position)
        {
            float best = 100f;
            for (int i = 0; i < opponents.Count; i++)
            {
                MatchPlayer opponent = opponents[i];
                if (opponent == null || opponent.IsSentOff || opponent.IsComplaining) continue;
                best = Mathf.Min(best, Vector3.Distance(position, opponent.transform.position));
            }
            return best;
        }

        private float PassLaneClearance(Vector3 start, Vector3 end)
        {
            float clearance = 100f;
            Vector3 segment = end - start;
            float lengthSquared = segment.sqrMagnitude;
            if (lengthSquared < 0.01f) return 0f;

            for (int i = 0; i < opponents.Count; i++)
            {
                MatchPlayer opponent = opponents[i];
                if (opponent == null || opponent.IsSentOff || opponent.IsComplaining) continue;
                float t = Mathf.Clamp01(Vector3.Dot(opponent.transform.position - start, segment) / lengthSquared);
                Vector3 nearestPoint = start + segment * t;
                clearance = Mathf.Min(clearance, Vector3.Distance(opponent.transform.position, nearestPoint));
            }
            return clearance;
        }

        private Vector3 ChooseDribbleTarget(Vector3 ballPosition)
        {
            float lane = Mathf.Sign(player.transform.position.x);
            if (Mathf.Abs(lane) < 0.1f) lane = player.ShirtNumber % 2 == 0 ? -1f : 1f;
            float pressureLeft = NearestOpponentDistance(ballPosition + Vector3.left * 2.4f);
            float pressureRight = NearestOpponentDistance(ballPosition + Vector3.right * 2.4f);
            float escapeSide = pressureLeft > pressureRight ? -1f : 1f;
            float x = Mathf.Lerp(ballPosition.x, ballPosition.x + escapeSide * 2.3f, 0.55f);
            float z = ballPosition.z + attackDirection * 4.5f;
            return new Vector3(Mathf.Clamp(x, -15.5f, 15.5f), player.transform.position.y, z);
        }

        private Vector3 AttackingSupportTarget(Vector3 ballPosition)
        {
            float lane = Mathf.Sign(home.x);
            if (Mathf.Abs(home.x) < 0.5f)
                lane = player.ShirtNumber % 2 == 0 ? -1f : 1f;

            if (player.ShirtNumber >= 9)
            {
                float defensiveLine = FindDefensiveLineProgress();
                float runZ = Mathf.Max(attackDirection * home.z + 3f,
                    Mathf.Min(defensiveLine - 1.8f, attackDirection * ballPosition.z + 9f));
                float runX = home.x * 0.52f + lane * 1.8f + ballPosition.x * 0.12f;
                return new Vector3(runX, player.transform.position.y, attackDirection * runZ);
            }

            float roleOffset = player.ShirtNumber >= 6 ? 2.0f : -3.8f;
            float width = player.ShirtNumber == 2 || player.ShirtNumber == 5 ? 0.78f : 0.48f;
            float desiredX = home.x * 0.52f + lane * width * 3.8f + ballPosition.x * 0.18f;
            float desiredZ = home.z + attackDirection * roleOffset;
            desiredZ = Mathf.Lerp(desiredZ, ballPosition.z + attackDirection * roleOffset, 0.34f);
            return new Vector3(desiredX, player.transform.position.y, desiredZ);
        }

        private Vector3 SupportControlledCarrier(MatchPlayer carrier, Vector3 ballPosition)
        {
            MatchTacticsDirector director = MatchTacticsDirector.Instance;
            if (director != null && director.IsAttackingPatternActive(player.TeamId))
                return SetPlaySupportTarget(carrier);

            float side = Mathf.Sign(home.x);
            if (Mathf.Abs(side) < 0.1f)
                side = player.ShirtNumber % 2 == 0 ? -1f : 1f;

            float carrierProgress = attackDirection * carrier.transform.position.z;
            float forwardLimit = Mathf.Min(FindDefensiveLineProgress() - 2.0f, 25f);
            float supportX;
            float supportProgress;

            if (player.ShirtNumber == 6)
            {
                // Deep midfielder offers the safe vertex of a passing triangle.
                supportX = Mathf.Lerp(home.x, carrier.transform.position.x, 0.45f);
                supportProgress = carrierProgress - 5.0f;
            }
            else if (player.ShirtNumber == 7)
            {
                // One midfielder stays close and wide as the short wall-pass option.
                supportX = carrier.transform.position.x + side * 5.0f;
                supportProgress = carrierProgress - 1.0f;
            }
            else if (player.ShirtNumber == 8)
            {
                // The other midfielder attacks the diagonal space beyond the carrier.
                supportX = carrier.transform.position.x - side * 4.2f;
                supportProgress = Mathf.Min(carrierProgress + 4.5f, forwardLimit);
            }
            else if (player.ShirtNumber == 2 || player.ShirtNumber == 5)
            {
                // Fullbacks hold width and are ready to overlap outside the winger.
                float flank = player.ShirtNumber == 2 ? -1f : 1f;
                supportX = Mathf.Clamp(carrier.transform.position.x + flank * 7.0f, -15.5f, 15.5f);
                supportProgress = Mathf.Min(carrierProgress + 1.5f, forwardLimit);
            }
            else if (player.ShirtNumber >= 9)
            {
                supportX = carrier.transform.position.x + side * 4.2f;
                supportProgress = Mathf.Min(carrierProgress + 7.0f, forwardLimit);
            }
            else
            {
                supportX = Mathf.Lerp(home.x, carrier.transform.position.x, 0.42f);
                supportProgress = carrierProgress - 6.0f;
            }

            supportX = Mathf.Clamp(supportX, -15.5f, 15.5f);
            supportProgress = Mathf.Clamp(supportProgress, -25f, 25f);
            return new Vector3(supportX, player.transform.position.y, attackDirection * supportProgress);
        }

        private Vector3 SetPlaySupportTarget(MatchPlayer carrier)
        {
            float carrierProgress = attackDirection * carrier.transform.position.z;
            float onsideLimit = Mathf.Min(FindDefensiveLineProgress() - 2.2f, 25f);
            float side = Mathf.Sign(home.x);
            if (Mathf.Abs(side) < 0.1f) side = player.ShirtNumber % 2 == 0 ? -1f : 1f;
            float x;
            float progress;

            if (player.ShirtNumber == 2 || player.ShirtNumber == 5)
            {
                // Outside overlap: fullback accelerates beyond the wide attacker.
                float flank = player.ShirtNumber == 2 ? -1f : 1f;
                x = carrier.transform.position.x + flank * 8.0f;
                progress = Mathf.Min(carrierProgress + 6.0f, onsideLimit);
                moveSpeed = 5.25f;
            }
            else if (player.ShirtNumber == 6)
            {
                x = carrier.transform.position.x - side * 4.5f;
                progress = carrierProgress - 4.5f;
            }
            else if (player.ShirtNumber == 7)
            {
                x = carrier.transform.position.x + side * 4.5f;
                progress = carrierProgress - 0.8f;
            }
            else if (player.ShirtNumber == 8)
            {
                x = carrier.transform.position.x - side * 3.5f;
                progress = Mathf.Min(carrierProgress + 5.5f, onsideLimit);
                moveSpeed = 5.0f;
            }
            else if (player.ShirtNumber >= 9)
            {
                x = carrier.transform.position.x + side * (player.ShirtNumber == 10 ? -5.5f : 5.5f);
                progress = Mathf.Min(carrierProgress + (player.ShirtNumber == 9 ? 9.0f : 6.5f), onsideLimit);
                moveSpeed = 5.1f;
            }
            else
            {
                x = Mathf.Lerp(home.x, carrier.transform.position.x, 0.35f);
                progress = carrierProgress - 6.5f;
            }

            return new Vector3(Mathf.Clamp(x, -15.5f, 15.5f), player.transform.position.y,
                attackDirection * Mathf.Clamp(progress, -25f, 25f));
        }

        private float FindDefensiveLineProgress()
        {
            float line = 27f;
            bool found = false;
            for (int i = 0; i < opponents.Count; i++)
            {
                MatchPlayer opponent = opponents[i];
                if (opponent == null || opponent.IsSentOff || opponent.IsGoalkeeper) continue;
                float progress = attackDirection * opponent.transform.position.z;
                if (!found || progress < line)
                {
                    line = progress;
                    found = true;
                }
            }
            return found ? line : 18f;
        }

        private Vector3 DefensiveShapeTarget(Vector3 ballPosition)
        {
            float compactX = Mathf.Lerp(home.x, ballPosition.x, 0.22f);
            float lineShift = Mathf.Clamp(ballPosition.z * 0.20f, -5.5f, 5.5f);
            float roleDepth = player.ShirtNumber <= 5 ? -attackDirection * 1.4f : 0f;
            Vector3 target = new Vector3(compactX, player.transform.position.y, home.z + lineShift + roleDepth);

            MatchPlayer mark = FindZonalMarkTarget(ballPosition);
            if (mark != null)
            {
                float markDistance = Vector3.Distance(player.transform.position, mark.transform.position);
                float influence = player.ShirtNumber <= 5 ? 0.48f : 0.30f;
                if (markDistance < 13f)
                {
                    Vector3 markPosition = mark.transform.position;
                    markPosition.z -= attackDirection * 1.8f;
                    target = Vector3.Lerp(target, markPosition, influence);
                }
            }
            return target;
        }

        private MatchPlayer FindZonalMarkTarget(Vector3 ballPosition)
        {
            MatchPlayer best = null;
            float bestScore = float.MaxValue;
            float ownProgress = attackDirection * player.transform.position.z;

            for (int i = 0; i < opponents.Count; i++)
            {
                MatchPlayer candidate = opponents[i];
                if (candidate == null || candidate.IsSentOff || candidate.IsGoalkeeper) continue;

                float opponentProgress = attackDirection * candidate.transform.position.z;
                bool inRelevantZone = player.ShirtNumber <= 5
                    ? opponentProgress > ownProgress - 2f
                    : Mathf.Abs(opponentProgress - ownProgress) < 12f;
                if (!inRelevantZone) continue;

                float distance = Vector3.Distance(player.transform.position, candidate.transform.position);
                float ballSidePenalty = Vector3.Distance(candidate.transform.position, ballPosition) * 0.12f;
                float score = distance + ballSidePenalty;
                if (score < bestScore)
                {
                    bestScore = score;
                    best = candidate;
                }
            }
            return best;
        }

        private MatchPlayer FindClosest(IReadOnlyList<MatchPlayer> list, Vector3 position, out float distance)
        {
            MatchPlayer best = null;
            float bestSqrDistance = float.MaxValue;
            for (int i = 0; i < list.Count; i++)
            {
                MatchPlayer candidate = list[i];
                if (candidate == null || candidate.IsSentOff || candidate.IsComplaining) continue;
                float sqrDistance = (candidate.transform.position - position).sqrMagnitude;
                if (sqrDistance < bestSqrDistance)
                {
                    bestSqrDistance = sqrDistance;
                    best = candidate;
                }
            }
            distance = best == null ? float.MaxValue : Mathf.Sqrt(bestSqrDistance);
            return best;
        }

        private int GetRankByDistance(IReadOnlyList<MatchPlayer> list, Vector3 position, MatchPlayer subject)
        {
            float subjectDistance = (subject.transform.position - position).sqrMagnitude;
            int rank = 1;
            for (int i = 0; i < list.Count; i++)
            {
                MatchPlayer candidate = list[i];
                if (candidate == null || candidate == subject || candidate.IsSentOff || candidate.IsComplaining)
                    continue;
                float candidateDistance = (candidate.transform.position - position).sqrMagnitude;
                if (candidateDistance + 0.0001f < subjectDistance) rank++;
            }
            return rank;
        }
    }
}