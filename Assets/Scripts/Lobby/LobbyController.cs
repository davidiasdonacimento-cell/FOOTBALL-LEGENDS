using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

namespace FootballLegends.Lobby
{
    public sealed partial class LobbyController : MonoBehaviour
    {
        [SerializeField] private GameObject homePanel;
        [SerializeField] private GameObject contentPanel;
        [SerializeField] private Text contentTitle;
        [SerializeField] private Text contentDescription;
        [SerializeField] private GameObject playerMatchButton;
        [SerializeField] private GameObject refereeMatchButton;

        public void OpenSection(string section)
        {
            if (homePanel == null || contentPanel == null) return;

            homePanel.SetActive(false);
            contentPanel.SetActive(true);

            if (contentTitle != null) contentTitle.text = section.ToUpperInvariant();
            if (contentDescription != null)
                contentDescription.text = GetSectionDescription(section);

            bool showMatchModes = section == "JOGAR";
            if (playerMatchButton != null) playerMatchButton.SetActive(showMatchModes);
            if (refereeMatchButton != null) refereeMatchButton.SetActive(showMatchModes);
        }

        public void StartPlayerMatch()
        {
            PlayerPrefs.SetInt("FL_MATCH_MODE", 0);
            PlayerPrefs.Save();
            SceneManager.LoadScene("Match");
        }

        public void StartRefereeMatch()
        {
            PlayerPrefs.SetInt("FL_MATCH_MODE", 1);
            PlayerPrefs.Save();
            SceneManager.LoadScene("Match");
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
                    return "Eventos e desafios locais do clube.";
                case "PERFIL":
                    return "Perfil do atleta e progresso local.";
                case "JOGAR":
                    return "Escolha como participar: controlar um time ou apitar uma partida entre duas IAs.";
                default:
                    return "Área do FOOTBALL LEGENDS.";
            }
        }
    }
}