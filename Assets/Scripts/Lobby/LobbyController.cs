using UnityEngine;
using UnityEngine.UI;

namespace FootballLegends.Lobby
{
    public sealed class LobbyController : MonoBehaviour
    {
        [SerializeField] private GameObject homePanel;
        [SerializeField] private GameObject contentPanel;
        [SerializeField] private Text contentTitle;
        [SerializeField] private Text contentDescription;

        public void OpenSection(string section)
        {
            if (homePanel == null || contentPanel == null) return;

            homePanel.SetActive(false);
            contentPanel.SetActive(true);

            if (contentTitle != null) contentTitle.text = section.ToUpperInvariant();
            if (contentDescription != null)
                contentDescription.text = GetSectionDescription(section);
        }

        public void ReturnToLobby()
        {
            if (contentPanel != null) contentPanel.SetActive(false);
            if (homePanel != null) homePanel.SetActive(true);
        }

        private static string GetSectionDescription(string section)
        {
            switch (section)
            {
                case "ELENCO":
                    return "Prévia do elenco e da formação do seu clube.";
                case "EVENTOS":
                    return "Eventos e desafios serão apresentados aqui.";
                case "PERFIL":
                    return "Perfil do atleta e progresso local.";
                case "JOGAR":
                    return "A experiência de partida ainda está em desenvolvimento.";
                default:
                    return "Área do FOOTBALL LEGENDS.";
            }
        }
    }
}
