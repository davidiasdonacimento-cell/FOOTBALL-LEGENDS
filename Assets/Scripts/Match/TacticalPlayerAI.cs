using System.Collections.Generic;
using UnityEngine;

namespace FootballLegends.Match
{
    public sealed class TacticalPlayerAI : MonoBehaviour
    {
        private MatchPlayer player;
        private IReadOnlyList<MatchPlayer> teammates;
        private IReadOnlyList<MatchPlayer> opponents;
        private Transform ball;
        private int attackDirection;
        private Vector3 home;
        private float thinkTimer;

        public void Configure(MatchPlayer owner, IReadOnlyList<MatchPlayer> team,
            IReadOnlyList<MatchPlayer> opposition, Transform matchBall, int direction, Vector3 homePosition)
        {
            player = owner;
            teammates = team;
            opponents = opposition;
            ball = matchBall;
            attackDirection = direction;
            home = homePosition;
        }

        private void Update()
        {
            if (player == null || player.IsSentOff || player.IsComplaining || ball == null) return;
            thinkTimer -= Time.deltaTime;
            if (thinkTimer > 0f) return;
            thinkTimer = Random.Range(0.12f, 0.24f);

            Vector3 ballPos = ball.position;
            MatchPlayer closest = null;
            float nearest = float.MaxValue;
            for (int i = 0; i < teammates.Count; i++)
            {
                MatchPlayer candidate = teammates[i];
                if (candidate == null || candidate.IsSentOff) continue;
                float d = (candidate.transform.position - ballPos).sqrMagnitude;
                if (d < nearest) { nearest = d; closest = candidate; }
            }

            Vector3 target = home;
            if (player.IsGoalkeeper)
            {
                target = new Vector3(Mathf.Clamp(ballPos.x * 0.55f, -3.1f, 3.1f),
                    player.transform.position.y, -attackDirection * 25.5f);
            }
            else if (closest == player)
            {
                target = ballPos + new Vector3(0f, 0f, -attackDirection * 1.1f);
            }
            else
            {
                float advance = Mathf.Clamp(ballPos.z * 0.48f, -10f, 10f);
                target = home + new Vector3(ballPos.x * 0.16f, 0f, advance);
                if (player.ShirtNumber >= 9)
                    target += new Vector3(0f, 0f, attackDirection * 4.5f);
                if (player.ShirtNumber <= 4)
                    target -= new Vector3(0f, 0f, attackDirection * 2.5f);
            }

            target.x = Mathf.Clamp(target.x, -16f, 16f);
            target.z = Mathf.Clamp(target.z, -27f, 27f);
            Vector3 before = player.transform.position;
            player.MoveTo(target, player.IsGoalkeeper ? 5.5f : 4.1f);
            player.Face(ballPos);
            float actualSpeed = (player.transform.position - before).magnitude / Mathf.Max(Time.deltaTime, 0.001f);
            player.UpdateAnimation(actualSpeed);
        }
    }
}