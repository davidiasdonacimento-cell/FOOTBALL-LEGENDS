using System.Collections.Generic;
using UnityEngine;

namespace FootballLegends.Match
{
    /// <summary>
    /// Lightweight team-aware match AI. Tactical decisions are refreshed at a
    /// lower frequency, while movement and animation are updated every frame.
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

            bool teamHasPossession = ownDistance < 2.35f &&
                ownDistance <= opponentDistance + 0.55f;
            bool opponentHasPossession = opponentDistance < 2.35f &&
                opponentDistance < ownDistance - 0.25f;

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
                target = ballPosition + new Vector3(0f, 0f, -attackDirection * 0.72f);
                moveSpeed = 5.0f;
                if (teamHasPossession && Vector3.Distance(player.transform.position, ballPosition) < 2.2f)
                {
                    carryingBall = true;
                    target = ballPosition + Vector3.forward * attackDirection * 1.2f;
                    moveSpeed = 4.65f;
                    facingTarget = target + Vector3.forward * attackDirection * 5f;
                }
            }
            else if (opponentHasPossession)
            {
                int pressIndex = GetRankByDistance(teammates, ballPosition, player);
                if (pressIndex == 1)
                {
                    target = ballPosition + Vector3.forward * -attackDirection * 0.9f;
                    moveSpeed = 4.9f;
                }
                else if (pressIndex == 2)
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
                target = SupportTarget(ballPosition);
                moveSpeed = 4.4f;
                facingTarget = ballPosition;
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

        private Vector3 SupportTarget(Vector3 ballPosition)
        {
            float lane = Mathf.Sign(home.x);
            if (Mathf.Abs(home.x) < 0.5f)
                lane = (player.ShirtNumber % 2 == 0) ? -1f : 1f;

            float roleOffset;
            if (player.ShirtNumber >= 9) roleOffset = 6.5f;
            else if (player.ShirtNumber >= 6) roleOffset = 2.0f;
            else roleOffset = -3.8f;

            float width = player.ShirtNumber == 2 || player.ShirtNumber == 5 ? 0.78f : 0.48f;
            float desiredX = home.x * 0.52f + lane * width * 3.8f + ballPosition.x * 0.18f;
            float desiredZ = home.z + attackDirection * roleOffset;
            desiredZ = Mathf.Lerp(desiredZ, ballPosition.z + attackDirection * roleOffset, 0.34f);
            return new Vector3(desiredX, player.transform.position.y, desiredZ);
        }

        private Vector3 DefensiveShapeTarget(Vector3 ballPosition)
        {
            float compactX = Mathf.Lerp(home.x, ballPosition.x, 0.22f);
            float lineShift = Mathf.Clamp(ballPosition.z * 0.20f, -5.5f, 5.5f);
            float roleDepth = player.ShirtNumber <= 5 ? -attackDirection * 1.4f : 0f;
            float z = home.z + lineShift + roleDepth;
            return new Vector3(compactX, player.transform.position.y, z);
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