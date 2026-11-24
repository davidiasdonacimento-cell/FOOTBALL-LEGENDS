using System.Collections.Generic;
using System.Linq;
using UnityEngine;

namespace FootballLegends.Match
{
    public struct OffsideResult
    {
        public bool IsOffside;
        public float LineZ;
        public string Explanation;
    }

    public static class OffsideRuleSystem
    {
        // The pitch length is aligned to the Z axis. Direction is +1 or -1.
        public static OffsideResult Evaluate(MatchPlayer receiver, Vector3 ballPosition,
            IReadOnlyList<MatchPlayer> allPlayers, int attackingTeam, int attackingDirection)
        {
            var defenders = allPlayers
                .Where(p => p != null && p.TeamId != attackingTeam)
                .OrderByDescending(p => attackingDirection * p.transform.position.z)
                .ToList();

            if (receiver == null || defenders.Count < 2)
                return new OffsideResult { IsOffside = false, Explanation = "Sem defensores suficientes para avaliar." };

            float secondLastDefender = attackingDirection * defenders[1].transform.position.z;
            float ball = attackingDirection * ballPosition.z;
            float attacker = attackingDirection * receiver.transform.position.z;
            float lineProgress = Mathf.Max(secondLastDefender, ball);
            float lineZ = attackingDirection * lineProgress;

            bool opponentHalf = attacker > 0f;
            bool aheadOfBall = attacker > ball + 0.02f;
            bool aheadOfSecondLast = attacker > secondLastDefender + 0.02f;
            bool offside = opponentHalf && aheadOfBall && aheadOfSecondLast;

            return new OffsideResult
            {
                IsOffside = offside,
                LineZ = lineZ,
                Explanation = offside
                    ? "Atacante em posição de impedimento no momento do passe."
                    : "Posição regular no momento do passe."
            };
        }
    }
}