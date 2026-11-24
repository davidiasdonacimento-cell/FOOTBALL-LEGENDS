using UnityEngine;
using UnityEngine.UI;

namespace FootballLegends.Lobby
{
    public sealed partial class LobbyController
    {
        public void Initialize(GameObject home, GameObject content, Text title, Text description)
        {
            homePanel = home;
            contentPanel = content;
            contentTitle = title;
            contentDescription = description;
        }
    }
}
