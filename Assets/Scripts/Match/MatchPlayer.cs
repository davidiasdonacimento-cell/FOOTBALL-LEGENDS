using UnityEngine;

namespace FootballLegends.Match
{
    public sealed class MatchPlayer : MonoBehaviour
    {
        public int TeamId { get; private set; }
        public int ShirtNumber { get; private set; }
        public bool IsGoalkeeper { get; private set; }

        public void Initialize(int teamId, int shirtNumber, bool goalkeeper = false)
        {
            TeamId = teamId;
            ShirtNumber = shirtNumber;
            IsGoalkeeper = goalkeeper;
        }

        public void MoveTo(Vector3 destination, float speed)
        {
            Vector3 flat = new Vector3(destination.x, transform.position.y, destination.z);
            transform.position = Vector3.MoveTowards(transform.position, flat, speed * Time.deltaTime);
        }
    }
}