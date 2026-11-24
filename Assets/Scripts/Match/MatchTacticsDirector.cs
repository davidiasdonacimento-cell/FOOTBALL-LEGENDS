using System.Collections.Generic;
using UnityEngine;

namespace FootballLegends.Match
{
    /// <summary>
    /// Shared team-level tactical state for both sides. It coordinates phases
    /// without replacing each player's individual decision logic.
    /// </summary>
    public sealed class MatchTacticsDirector : MonoBehaviour
    {
        public enum TacticalPhase { Defending, Attacking, CounterAttack, LooseBall }

        public static MatchTacticsDirector Instance { get; private set; }

        private IReadOnlyList<MatchPlayer> teamA;
        private IReadOnlyList<MatchPlayer> teamB;
        private Transform ball;
        private int possessionTeam = -1;
        private int previousPossessionTeam = -1;
        private float possessionChangedAt;
        private float refreshAt;
        private MatchPlayer userControlledPlayer;
        private int attackingPatternTeam = -1;
        private float attackingPatternUntil;

        public void Configure(IReadOnlyList<MatchPlayer> first, IReadOnlyList<MatchPlayer> second, Transform matchBall)
        {
            teamA = first;
            teamB = second;
            ball = matchBall;
            possessionTeam = -1;
            previousPossessionTeam = -1;
            possessionChangedAt = Time.time;
            RefreshPossession();
        }

        private void Awake()
        {
            if (Instance != null && Instance != this)
                Destroy(Instance);
            Instance = this;
        }

        private void OnDestroy()
        {
            if (Instance == this) Instance = null;
        }

        private void Update()
        {
            if (MatchBootstrap.SimulationPaused || Time.time < refreshAt) return;
            refreshAt = Time.time + 0.18f;
            RefreshPossession();
        }

        private void RefreshPossession()
        {
            if (ball == null || teamA == null || teamB == null) return;
            float a = ClosestDistance(teamA, ball.position);
            float b = ClosestDistance(teamB, ball.position);
            int detected = -1;
            if (Mathf.Min(a, b) <= 2.65f)
                detected = a <= b ? 0 : 1;

            if (detected != possessionTeam && detected != -1)
            {
                previousPossessionTeam = possessionTeam;
                possessionTeam = detected;
                possessionChangedAt = Time.time;
            }
            else if (detected == -1 && Mathf.Min(a, b) > 4.2f)
            {
                possessionTeam = -1;
            }
        }

        public void SetUserControlledPlayer(MatchPlayer player)
        {
            userControlledPlayer = player;
        }

        public MatchPlayer GetUserControlledPlayer(int teamId)
        {
            if (userControlledPlayer == null || userControlledPlayer.IsSentOff ||
                userControlledPlayer.TeamId != teamId)
                return null;
            return userControlledPlayer;
        }

        public bool IsUserPlayerNearBall(int teamId, Vector3 ballPosition, float radius)
        {
            MatchPlayer userPlayer = GetUserControlledPlayer(teamId);
            return userPlayer != null &&
                (userPlayer.transform.position - ballPosition).sqrMagnitude <= radius * radius;
        }

        public bool ActivateAttackingPattern(int teamId, MatchPlayer carrier, float duration)
        {
            if (carrier == null || carrier.IsSentOff || carrier.TeamId != teamId || duration <= 0f)
                return false;
            attackingPatternTeam = teamId;
            attackingPatternUntil = Time.time + duration;
            return true;
        }

        public bool IsAttackingPatternActive(int teamId)
        {
            return attackingPatternTeam == teamId && Time.time < attackingPatternUntil;
        }

        public TacticalPhase GetPhase(int teamId)
        {
            if (possessionTeam < 0) return TacticalPhase.LooseBall;
            if (possessionTeam != teamId) return TacticalPhase.Defending;
            if (previousPossessionTeam >= 0 && previousPossessionTeam != teamId &&
                Time.time - possessionChangedAt < 3.5f)
                return TacticalPhase.CounterAttack;
            return TacticalPhase.Attacking;
        }

        public int GetPressRank(int teamId, MatchPlayer subject, Vector3 ballPosition)
        {
            IReadOnlyList<MatchPlayer> own = teamId == 0 ? teamA : teamB;
            if (own == null || subject == null) return int.MaxValue;
            float subjectDistance = (subject.transform.position - ballPosition).sqrMagnitude;
            int rank = 1;
            for (int i = 0; i < own.Count; i++)
            {
                MatchPlayer other = own[i];
                if (other == null || other == subject || other.IsSentOff || other.IsComplaining) continue;
                if ((other.transform.position - ballPosition).sqrMagnitude + 0.0001f < subjectDistance)
                    rank++;
            }
            return rank;
        }

        private float ClosestDistance(IReadOnlyList<MatchPlayer> team, Vector3 point)
        {
            float best = float.MaxValue;
            for (int i = 0; i < team.Count; i++)
            {
                MatchPlayer p = team[i];
                if (p == null || p.IsSentOff || p.IsComplaining) continue;
                best = Mathf.Min(best, Vector3.Distance(p.transform.position, point));
            }
            return best;
        }
    }
}