using UnityEngine;
using UnityEngine.UI;

namespace FootballLegends.Lobby
{
    public sealed partial class LobbyController
    {
        public void Initialize(GameObject home, GameObject content, Text title, Text description,
            GameObject matchButton, GameObject refereeButton)
        {
            homePanel = home;
            contentPanel = content;
            contentTitle = title;
            contentDescription = description;
            playerMatchButton = matchButton;
            refereeMatchButton = refereeButton;
        }
    }
}