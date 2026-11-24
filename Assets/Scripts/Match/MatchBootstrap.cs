using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

namespace FootballLegends.Match
{
    public sealed class MatchBootstrap : MonoBehaviour
    {
        private const float PitchHalfWidth = 18f;
        private const float PitchHalfLength = 29f;
        private readonly List<MatchPlayer> players = new List<MatchPlayer>();
        private readonly List<MatchPlayer> teamA = new List<MatchPlayer>();
        private readonly List<MatchPlayer> teamB = new List<MatchPlayer>();

        private bool refereeMode;
        private bool matchStopped;
        private float aiClock;
        private int scoreA;
        private int scoreB;
        private int controlledTeam = 0;
        private MatchPlayer controlledPlayer;
        private GameObject ball;
        private Rigidbody ballBody;
        private VirtualJoystick joystick;
        private Text statusText;
        private Text scoreText;
        private GameObject refereePanel;
        private LineRenderer offsideLine;
        private float lineTimer;
        private float sprintUntil;
        private Font uiFont;
        private int refereeSelection;
        private MatchPlayer selectedRefereePlayer;
        private Text selectedPlayerText;
        private Text complaintText;
        private float complaintMessageUntil;

        private void Awake()
        {
            Screen.orientation = ScreenOrientation.LandscapeLeft;
            Application.targetFrameRate = 60;
            QualitySettings.vSyncCount = 0;
            refereeMode = PlayerPrefs.GetInt("FL_MATCH_MODE", 0) == 1;
            BuildPitch();
            BuildTeams();
            BuildBall();
            ConfigureTacticalAI();
            BuildCamera();
            BuildInterface();
            SelectRefereePlayer(0);
            SetStatus(refereeMode ? "MODO ÁRBITRO • IA x IA" : "PARTIDA • CONTROLE DO JOGADOR");
        }

        private void Update()
        {
            if (matchStopped)
            {
                if (lineTimer > 0f) lineTimer -= Time.deltaTime;
                return;
            }

            UpdatePlayerControls();
            UpdateAiMatch();
            CheckGoal();
            CheckGoalkeeperSave();

            if (lineTimer > 0f)
            {
                lineTimer -= Time.deltaTime;
                if (lineTimer <= 0f && offsideLine != null) offsideLine.enabled = false;
            }
            if (complaintText != null && Time.time >= complaintMessageUntil) complaintText.text = "";
            for (int i = 0; i < players.Count; i++)
            {
                MatchPlayer p = players[i];
                if (p != null && !p.IsSentOff && p == controlledPlayer)
                    p.UpdateAnimation(joystick != null && joystick.Value.sqrMagnitude > 0.04f ? 1f : 0f);
            }
        }

        private void BuildPitch()
        {
            RenderSettings.ambientLight = new Color(0.58f, 0.62f, 0.68f);
            RenderSettings.fog = false;

            GameObject field = GameObject.CreatePrimitive(PrimitiveType.Plane);
            field.name = "Match Pitch";
            field.transform.position = Vector3.zero;
            field.transform.localScale = new Vector3(3.6f, 1f, 5.8f);
            SetColor(field, new Color(0.08f, 0.38f, 0.17f));

            CreateMarking(new Vector3(0f, 0.035f, 0f), new Vector3(36f, 0.04f, 0.12f));
            CreateMarking(new Vector3(0f, 0.035f, -29f), new Vector3(36f, 0.04f, 0.12f));
            CreateMarking(new Vector3(0f, 0.035f, 29f), new Vector3(36f, 0.04f, 0.12f));
            CreateMarking(new Vector3(-18f, 0.035f, 0f), new Vector3(0.12f, 0.04f, 58f));
            CreateMarking(new Vector3(18f, 0.035f, 0f), new Vector3(0.12f, 0.04f, 58f));
            CreateMarking(new Vector3(0f, 0.04f, 0f), new Vector3(36f, 0.04f, 0.12f));

            CreateGoal(0, -29f);
            CreateGoal(1, 29f);

            GameObject lightObject = new GameObject("Match Sun");
            Light light = lightObject.AddComponent<Light>();
            light.type = LightType.Directional;
            light.intensity = 1.25f;
            lightObject.transform.rotation = Quaternion.Euler(55f, -30f, 0f);

            GameObject lineObject = new GameObject("Offside Line");
            offsideLine = lineObject.AddComponent<LineRenderer>();
            offsideLine.positionCount = 2;
            offsideLine.startWidth = 0.16f;
            offsideLine.endWidth = 0.16f;
            offsideLine.material = new Material(Shader.Find("Sprites/Default"));
            offsideLine.startColor = new Color(1f, 0.25f, 0.08f, 1f);
            offsideLine.endColor = new Color(1f, 0.25f, 0.08f, 1f);
            offsideLine.enabled = false;
            offsideLine.useWorldSpace = true;
        }

        private void BuildTeams()
        {
            Vector3[] formationA =
            {
                new Vector3(0f, 0.9f, -22f), new Vector3(-11f, 0.9f, -14f),
                new Vector3(-4f, 0.9f, -12f), new Vector3(4f, 0.9f, -12f),
                new Vector3(11f, 0.9f, -14f), new Vector3(-6f, 0.9f, -3f),
                new Vector3(6f, 0.9f, -3f), new Vector3(0f, 0.9f, 3f),
                new Vector3(-9f, 0.9f, 8f), new Vector3(9f, 0.9f, 8f),
                new Vector3(0f, 0.9f, 12f)
            };
            Vector3[] formationB =
            {
                new Vector3(0f, 0.9f, 22f), new Vector3(-11f, 0.9f, 14f),
                new Vector3(-4f, 0.9f, 12f), new Vector3(4f, 0.9f, 12f),
                new Vector3(11f, 0.9f, 14f), new Vector3(-6f, 0.9f, 3f),
                new Vector3(6f, 0.9f, 3f), new Vector3(0f, 0.9f, -3f),
                new Vector3(-9f, 0.9f, -8f), new Vector3(9f, 0.9f, -8f),
                new Vector3(0f, 0.9f, -12f)
            };

            for (int i = 0; i < formationA.Length; i++)
            {
                teamA.Add(CreatePlayer(0, i + 1, formationA[i], new Color(0.12f, 0.42f, 0.95f)));
                teamB.Add(CreatePlayer(1, i + 1, formationB[i], new Color(0.9f, 0.16f, 0.12f)));
            }
            controlledPlayer = teamA[7];
        }

        private MatchPlayer CreatePlayer(int team, int number, Vector3 position, Color color)
        {
            GameObject model = new GameObject((team == 0 ? "Blue" : "Red") + " Player " + number);
            model.transform.position = position;
            model.AddComponent<CapsuleCollider>();
            MatchPlayer player = model.AddComponent<MatchPlayer>();
            player.Initialize(team, number, number == 1);
            players.Add(player);
            return player;
        }

        private void BuildBall()
        {
            ball = GameObject.CreatePrimitive(PrimitiveType.Sphere);
            ball.name = "Match Ball";
            ball.transform.position = new Vector3(0f, 0.55f, -1f);
            ball.transform.localScale = Vector3.one * 0.58f;
            SetColor(ball, Color.white);
            ballBody = ball.AddComponent<Rigidbody>();
            ballBody.mass = 0.43f;
            ballBody.drag = 0.35f;
            ballBody.angularDrag = 0.08f;
            ballBody.collisionDetectionMode = CollisionDetectionMode.ContinuousDynamic;
        }

        private void ConfigureTacticalAI()
        {
            ConfigureTeamAI(teamA, teamB, 1);
            ConfigureTeamAI(teamB, teamA, -1);
        }

        private void ConfigureTeamAI(List<MatchPlayer> own, List<MatchPlayer> opposition, int direction)
        {
            foreach (MatchPlayer p in own)
            {
                TacticalPlayerAI ai = p.gameObject.AddComponent<TacticalPlayerAI>();
                ai.Configure(p, own, opposition, ball.transform, direction, p.transform.position);
                if (!refereeMode && p == controlledPlayer) ai.enabled = false;
            }
        }

        private void BuildCamera()
        {
            Camera camera = Camera.main;
            if (camera == null)
            {
                GameObject cameraObject = new GameObject("Match Camera");
                camera = cameraObject.AddComponent<Camera>();
                cameraObject.tag = "MainCamera";
            }
            camera.transform.position = new Vector3(0f, 48f, -36f);
            camera.transform.LookAt(new Vector3(0f, 0f, 0f));
            camera.fieldOfView = 54f;
            camera.clearFlags = CameraClearFlags.SolidColor;
            camera.backgroundColor = new Color(0.06f, 0.12f, 0.18f);
        }

        private void BuildInterface()
        {
            uiFont = Resources.GetBuiltinResource<Font>("LegacyRuntime.ttf");
            GameObject canvasObject = new GameObject("Match HUD", typeof(Canvas), typeof(CanvasScaler), typeof(GraphicRaycaster));
            Canvas canvas = canvasObject.GetComponent<Canvas>();
            canvas.renderMode = RenderMode.ScreenSpaceOverlay;
            CanvasScaler scaler = canvasObject.GetComponent<CanvasScaler>();
            scaler.uiScaleMode = CanvasScaler.ScaleMode.ScaleWithScreenSize;
            scaler.referenceResolution = new Vector2(1920f, 1080f);
            scaler.matchWidthOrHeight = 0.5f;

            if (FindObjectOfType<EventSystem>() == null)
                new GameObject("EventSystem", typeof(EventSystem), typeof(StandaloneInputModule));

            scoreText = MakeText(canvas.transform, "0  -  0", 34, new Vector2(0.5f, 0.94f), new Vector2(0.25f, 0.08f), TextAnchor.MiddleCenter);
            statusText = MakeText(canvas.transform, "", 22, new Vector2(0.5f, 0.84f), new Vector2(0.65f, 0.06f), TextAnchor.MiddleCenter);
            complaintText = MakeText(canvas.transform, "", 24, new Vector2(0.5f, 0.76f), new Vector2(0.72f, 0.06f), TextAnchor.MiddleCenter);

            if (!refereeMode)
            {
                BuildJoystick(canvas.transform);
                CreateHudButton(canvas.transform, "PASSE", new Vector2(0.87f, 0.23f), new Vector2(0.12f, 0.12f), PassBall);
                CreateHudButton(canvas.transform, "CHUTE", new Vector2(0.91f, 0.39f), new Vector2(0.12f, 0.12f), ShootBall);
                CreateHudButton(canvas.transform, "CORRER", new Vector2(0.76f, 0.18f), new Vector2(0.12f, 0.10f), Sprint);
                CreateHudButton(canvas.transform, "TROCAR", new Vector2(0.73f, 0.34f), new Vector2(0.12f, 0.10f), SwitchPlayer);
                CreateHudButton(canvas.transform, "DESARMAR", new Vector2(0.87f, 0.55f), new Vector2(0.14f, 0.10f), Tackle);
            }
            else
            {
                CreateHudButton(canvas.transform, "PAUSAR", new Vector2(0.91f, 0.92f), new Vector2(0.12f, 0.08f), TogglePause);
                refereePanel = new GameObject("Referee Decision Panel", typeof(RectTransform), typeof(Image));
                refereePanel.transform.SetParent(canvas.transform, false);
                RectTransform panelRect = refereePanel.GetComponent<RectTransform>();
                panelRect.anchorMin = panelRect.anchorMax = new Vector2(0.5f, 0.22f);
                panelRect.sizeDelta = new Vector2(1120f, 300f);
                refereePanel.GetComponent<Image>().color = new Color(0.02f, 0.04f, 0.08f, 0.95f);
                selectedPlayerText = MakeText(refereePanel.transform, "JOGADOR SELECIONADO", 20,
                    new Vector2(0.5f, 0.88f), new Vector2(0.9f, 0.18f), TextAnchor.MiddleCenter);
                CreateChildButton(refereePanel.transform, "< JOGADOR", new Vector2(0.14f, 0.62f), SelectPreviousPlayer);
                CreateChildButton(refereePanel.transform, "PRÓXIMO >", new Vector2(0.38f, 0.62f), SelectNextPlayer);
                CreateChildButton(refereePanel.transform, "AMARELO", new Vector2(0.64f, 0.62f), GiveYellowCard);
                CreateChildButton(refereePanel.transform, "VERMELHO", new Vector2(0.87f, 0.62f), GiveRedCard);
                CreateChildButton(refereePanel.transform, "PARAR JOGO", new Vector2(0.26f, 0.25f), StopMatch);
                CreateChildButton(refereePanel.transform, "RETOMAR", new Vector2(0.50f, 0.25f), ResumeMatch);
                CreateChildButton(refereePanel.transform, "IMPEDIMENTO", new Vector2(0.74f, 0.25f), ConfirmOffside);
                CreateChildButton(refereePanel.transform, "SEGUE", new Vector2(0.92f, 0.25f), PlayOn);
                refereePanel.SetActive(true);
            }

            CreateHudButton(canvas.transform, "SAIR", new Vector2(0.08f, 0.92f), new Vector2(0.12f, 0.08f), ReturnToLobby);
        }

        private void BuildJoystick(Transform parent)
        {
            GameObject baseObject = new GameObject("Movement Joystick", typeof(RectTransform), typeof(Image), typeof(VirtualJoystick));
            baseObject.transform.SetParent(parent, false);
            RectTransform baseRect = baseObject.GetComponent<RectTransform>();
            baseRect.anchorMin = baseRect.anchorMax = new Vector2(0.13f, 0.22f);
            baseRect.sizeDelta = new Vector2(210f, 210f);
            baseObject.GetComponent<Image>().color = new Color(0.05f, 0.08f, 0.12f, 0.55f);

            GameObject knob = new GameObject("Joystick Handle", typeof(RectTransform), typeof(Image));
            knob.transform.SetParent(baseObject.transform, false);
            RectTransform knobRect = knob.GetComponent<RectTransform>();
            knobRect.anchorMin = knobRect.anchorMax = new Vector2(0.5f, 0.5f);
            knobRect.sizeDelta = new Vector2(88f, 88f);
            knob.GetComponent<Image>().color = new Color(0.35f, 0.75f, 1f, 0.85f);
            joystick = baseObject.GetComponent<VirtualJoystick>();
            joystick.Configure(knobRect, 78f);
        }

        private void UpdatePlayerControls()
        {
            if (joystick == null || controlledPlayer == null) return;
            Vector2 input = joystick.Value;
            Vector3 move = new Vector3(input.x, 0f, input.y);
            float speed = Time.time < sprintUntil ? 8.5f : 5.2f;
            controlledPlayer.transform.position += move * speed * Time.deltaTime;
            controlledPlayer.transform.position = new Vector3(
                Mathf.Clamp(controlledPlayer.transform.position.x, -PitchHalfWidth + 1f, PitchHalfWidth - 1f),
                0.9f,
                Mathf.Clamp(controlledPlayer.transform.position.z, -PitchHalfLength + 1f, PitchHalfLength - 1f));
        }

        private void UpdateAiMatch()
        {
            aiClock += Time.deltaTime;
            if (aiClock >= 4.5f)
            {
                aiClock = 0f;
                List<MatchPlayer> attacking = Random.value > 0.5f ? teamA : teamB;
                int direction = attacking == teamA ? 1 : -1;
                MatchPlayer receiver = attacking[attacking.Count - 1];
                // Deterministic training scenarios alternate between legal and offside runs.
                float runZ = direction * (Random.value < 0.35f ? 21f : 10f);
                receiver.transform.position = new Vector3(receiver.transform.position.x, 0.9f, runZ);
                ball.transform.position = new Vector3(0f, 0.55f, runZ - direction * 4f);
                OffsideResult result = OffsideRuleSystem.Evaluate(receiver, ball.transform.position, players, receiver.TeamId, direction);
                ShowOffsideLine(result.LineZ, result.IsOffside);
                if (result.IsOffside)
                {
                    matchStopped = true;
                    SetStatus("LANCE EM ANÁLISE • " + result.Explanation);
                    if (refereePanel != null) refereePanel.SetActive(true);
                }
                else
                {
                    SetStatus("JOGO EM ANDAMENTO • " + result.Explanation);
                }
            }
        }

        private void UpdateAiMatch()
        {
            aiClock += Time.deltaTime;
            if (aiClock < 1.5f) return;
            aiClock = 0f;
            MatchPlayer owner = FindClosestActivePlayer(ball.transform.position);
            if (owner == null || Vector3.Distance(owner.transform.position, ball.transform.position) > 3.4f) return;
            List<MatchPlayer> own = owner.TeamId == 0 ? teamA : teamB;
            int direction = owner.TeamId == 0 ? 1 : -1;
            MatchPlayer receiver = FindForwardReceiver(owner, own, direction);
            if (receiver == null) return;
            OffsideResult result = OffsideRuleSystem.Evaluate(receiver, ball.transform.position, players, owner.TeamId, direction);
            ShowOffsideLine(result.LineZ, result.IsOffside);
            if (result.IsOffside)
            {
                if (refereeMode)
                {
                    matchStopped = true;
                    SetStatus("LANCE EM ANÁLISE • POSSÍVEL IMPEDIMENTO");
                }
                else SetStatus("IMPEDIMENTO • PASSE CANCELADO");
                return;
            }
            Vector3 directionToReceiver = (receiver.transform.position - ball.transform.position).normalized;
            ballBody.velocity = Vector3.zero;
            ballBody.AddForce(directionToReceiver * 10.5f + Vector3.up * 1.1f, ForceMode.Impulse);
            owner.Face(receiver.transform.position);
        }

        private MatchPlayer FindClosestActivePlayer(Vector3 position)
        {
            MatchPlayer best = null;
            float bestDistance = float.MaxValue;
            foreach (MatchPlayer p in players)
            {
                if (p == null || p.IsSentOff || p.IsComplaining) continue;
                float d = (p.transform.position - position).sqrMagnitude;
                if (d < bestDistance) { bestDistance = d; best = p; }
            }
            return best;
        }

        private MatchPlayer FindForwardReceiver(MatchPlayer owner, List<MatchPlayer> own, int direction)
        {
            MatchPlayer best = null;
            float bestProgress = float.MinValue;
            foreach (MatchPlayer p in own)
            {
                if (p == null || p == owner || p.IsSentOff || p.IsGoalkeeper) continue;
                float progress = direction * p.transform.position.z;
                if (progress > bestProgress && Vector3.Distance(p.transform.position, owner.transform.position) > 3f)
                { bestProgress = progress; best = p; }
            }
            return best;
        }

        private void CheckGoal()
        {
            if (ball.transform.position.y > 2.7f || Mathf.Abs(ball.transform.position.x) > 3.6f) return;
            if (ball.transform.position.z > 29.2f) AwardGoal(0);
            else if (ball.transform.position.z < -29.2f) AwardGoal(1);
        }

        private void AwardGoal(int team)
        {
            if (team == 0) scoreA++; else scoreB++;
            if (scoreText != null) scoreText.text = "AZUL  " + scoreA + "  -  " + scoreB + "  VERMELHO";
            SetStatus(team == 0 ? "GOOOOOOL DO TIME AZUL!" : "GOOOOOOL DO TIME VERMELHO!");
            matchStopped = true;
            Invoke(nameof(RestartAfterGoal), 2f);
        }

        private void RestartAfterGoal()
        {
            ballBody.velocity = Vector3.zero;
            ballBody.angularVelocity = Vector3.zero;
            ball.transform.position = new Vector3(0f, 0.55f, 0f);
            matchStopped = false;
        }

        private void CheckGoalkeeperSave()
        {
            if (ballBody.velocity.sqrMagnitude < 0.2f) return;
            foreach (MatchPlayer keeper in players)
            {
                if (keeper == null || !keeper.IsGoalkeeper || keeper.IsSentOff) continue;
                if (Vector3.Distance(keeper.transform.position, ball.transform.position) < 1.3f &&
                    Mathf.Abs(ball.transform.position.z) > 21f)
                {
                    ballBody.velocity = Vector3.zero;
                    ballBody.angularVelocity = Vector3.zero;
                    ball.transform.position = keeper.transform.position + new Vector3(0f, 0.2f, keeper.TeamId == 0 ? 0.8f : -0.8f);
                    SetStatus("DEFESA DO GOLEIRO!");
                    break;
                }
            }
        }

        private void ShowOffsideLine(float z, bool flagged)
        {
            if (offsideLine == null) return;
            offsideLine.startColor = flagged ? Color.red : Color.yellow;
            offsideLine.endColor = flagged ? Color.red : Color.yellow;
            offsideLine.SetPosition(0, new Vector3(-PitchHalfWidth, 0.12f, z));
            offsideLine.SetPosition(1, new Vector3(PitchHalfWidth, 0.12f, z));
            offsideLine.enabled = true;
            lineTimer = 4f;
        }

        private void ConfirmOffside()
        {
            if (refereePanel != null) refereePanel.SetActive(false);
            matchStopped = false;
            SetStatus("ÁRBITRO: IMPEDIMENTO MARCADO • TIRO LIVRE INDIRETO");
            if (offsideLine != null) offsideLine.enabled = false;
        }

        private void PlayOn()
        {
            if (refereePanel != null) refereePanel.SetActive(false);
            matchStopped = false;
            SetStatus("ÁRBITRO: SEGUE O JOGO");
            if (offsideLine != null) offsideLine.enabled = false;
        }

        private void SelectPreviousPlayer() { SelectRefereePlayer(-1); }
        private void SelectNextPlayer() { SelectRefereePlayer(1); }

        private void SelectRefereePlayer(int step)
        {
            if (players.Count == 0) return;
            refereeSelection = (refereeSelection + step + players.Count) % players.Count;
            selectedRefereePlayer = players[refereeSelection];
            if (selectedPlayerText != null)
                selectedPlayerText.text = (selectedRefereePlayer.TeamId == 0 ? "AZUL" : "VERMELHO") +
                    " • CAMISA " + selectedRefereePlayer.ShirtNumber +
                    (selectedRefereePlayer.IsSentOff ? " • EXPULSO" : "");
        }

        private void GiveYellowCard()
        {
            if (selectedRefereePlayer == null || selectedRefereePlayer.IsSentOff) return;
            selectedRefereePlayer.ReceiveCard(false);
            ShowComplaint("AH, FUI ROUBADO! VOCÊ TÁ DE BRINCADEIRA?");
            SetStatus("ÁRBITRO: CARTÃO AMARELO APLICADO");
        }

        private void GiveRedCard()
        {
            if (selectedRefereePlayer == null || selectedRefereePlayer.IsSentOff) return;
            selectedRefereePlayer.ReceiveCard(true);
            ShowComplaint("ISSO É INJUSTO! EU NÃO FIZ NADA!");
            SetStatus("ÁRBITRO: CARTÃO VERMELHO • EXPULSO");
        }

        private void ShowComplaint(string line)
        {
            if (complaintText != null) complaintText.text = line;
            complaintMessageUntil = Time.time + 3.8f;
        }

        private void StopMatch() { matchStopped = true; SetStatus("ÁRBITRO: PARTIDA PARADA"); }
        private void ResumeMatch() { matchStopped = false; SetStatus("ÁRBITRO: PARTIDA RETOMADA"); }

        private void PassBall()
        {
            if (controlledPlayer == null) return;
            MatchPlayer receiver = teamA[teamA.Count - 1];
            ballBody.velocity = Vector3.zero;
            ballBody.angularVelocity = Vector3.zero;
            ball.transform.position = controlledPlayer.transform.position + new Vector3(0f, 0.35f, 0f);
            OffsideResult result = OffsideRuleSystem.Evaluate(receiver, ball.transform.position, players, 0, 1);
            ShowOffsideLine(result.LineZ, result.IsOffside);
            if (result.IsOffside)
            {
                SetStatus("IMPEDIMENTO • PASSE CANCELADO");
                return;
            }
            Vector3 passDirection = (receiver.transform.position - ball.transform.position).normalized;
            ballBody.AddForce(passDirection * 10f + Vector3.up * 1.2f, ForceMode.Impulse);
            SetStatus("PASSE PARA O JOGADOR " + receiver.ShirtNumber);
        }

        private void ShootBall()
        {
            if (controlledPlayer == null) return;
            ballBody.velocity = Vector3.zero;
            ballBody.angularVelocity = Vector3.zero;
            ball.transform.position = controlledPlayer.transform.position + new Vector3(0f, 0.4f, 0.6f);
            ballBody.AddForce(new Vector3(0f, 4.5f, 19f), ForceMode.Impulse);
            SetStatus("CHUTE EXECUTADO");
        }

        private void Sprint() { sprintUntil = Time.time + 2.5f; SetStatus("CORRIDA ATIVADA"); }

        private void SwitchPlayer()
        {
            int index = teamA.IndexOf(controlledPlayer);
            for (int i = 1; i <= teamA.Count; i++)
            {
                MatchPlayer candidate = teamA[(index + i) % teamA.Count];
                if (candidate == null || candidate.IsSentOff) continue;
                if (controlledPlayer != null)
                {
                    TacticalPlayerAI oldAi = controlledPlayer.GetComponent<TacticalPlayerAI>();
                    if (oldAi != null) oldAi.enabled = true;
                }
                controlledPlayer = candidate;
                TacticalPlayerAI newAi = controlledPlayer.GetComponent<TacticalPlayerAI>();
                if (newAi != null) newAi.enabled = false;
                SetStatus("JOGADOR " + controlledPlayer.ShirtNumber + " SELECIONADO");
                return;
            }
        }

        private void Tackle()
        {
            if (controlledPlayer == null) return;
            MatchPlayer nearest = null;
            float nearestDistance = float.MaxValue;
            foreach (MatchPlayer opponent in teamB)
            {
                float distance = Vector3.Distance(controlledPlayer.transform.position, opponent.transform.position);
                if (distance < nearestDistance) { nearestDistance = distance; nearest = opponent; }
            }
            if (nearest != null && nearestDistance <= 4.2f)
            {
                ballBody.velocity = Vector3.zero;
                ball.transform.position = controlledPlayer.transform.position + new Vector3(0f, 0.35f, 0f);
                SetStatus("DESARME BEM-SUCEDIDO");
            }
            else
            {
                SetStatus("APROXIME-SE DO ADVERSÁRIO PARA DESARMAR");
            }
        }
        private void TogglePause() { matchStopped = !matchStopped; SetStatus(matchStopped ? "PARTIDA PAUSADA" : "PARTIDA RETOMADA"); }

        private void ReturnToLobby()
        {
            UnityEngine.SceneManagement.SceneManager.LoadScene("Lobby");
        }

        private void CreateGoal(int team, float z)
        {
            Color color = new Color(0.88f, 0.91f, 0.96f);
            CreatePost(new Vector3(-3.6f, 1.2f, z), new Vector3(0.18f, 2.4f, 0.18f), color);
            CreatePost(new Vector3(3.6f, 1.2f, z), new Vector3(0.18f, 2.4f, 0.18f), color);
            CreatePost(new Vector3(0f, 2.4f, z), new Vector3(7.2f, 0.18f, 0.18f), color);
        }

        private void CreatePost(Vector3 position, Vector3 scale, Color color)
        {
            GameObject post = GameObject.CreatePrimitive(PrimitiveType.Cube);
            post.transform.position = position;
            post.transform.localScale = scale;
            SetColor(post, color);
        }

        private void CreateMarking(Vector3 position, Vector3 scale)
        {
            GameObject marking = GameObject.CreatePrimitive(PrimitiveType.Cube);
            marking.transform.position = position;
            marking.transform.localScale = scale;
            SetColor(marking, Color.white);
        }

        private void SetColor(GameObject target, Color color)
        {
            Renderer renderer = target.GetComponent<Renderer>();
            if (renderer == null) return;
            Shader shader = Shader.Find("Universal Render Pipeline/Lit");
            if (shader == null) shader = Shader.Find("Standard");
            renderer.material = new Material(shader);
            renderer.material.color = color;
        }

        private Text MakeText(Transform parent, string value, int size, Vector2 anchor,
            Vector2 normalizedSize, TextAnchor alignment)
        {
            GameObject obj = new GameObject(value + " Text", typeof(RectTransform), typeof(Text));
            obj.transform.SetParent(parent, false);
            RectTransform rect = obj.GetComponent<RectTransform>();
            rect.anchorMin = rect.anchorMax = anchor;
            rect.sizeDelta = new Vector2(1920f * normalizedSize.x, 1080f * normalizedSize.y);
            Text text = obj.GetComponent<Text>();
            text.font = uiFont;
            text.text = value;
            text.fontSize = size;
            text.alignment = alignment;
            text.color = Color.white;
            text.resizeTextForBestFit = true;
            text.resizeTextMinSize = 12;
            text.resizeTextMaxSize = size;
            return text;
        }

        private void CreateHudButton(Transform parent, string label, Vector2 anchor,
            Vector2 normalizedSize, UnityEngine.Events.UnityAction action)
        {
            GameObject obj = CreateButtonObject(parent, label, anchor, normalizedSize);
            obj.GetComponent<Button>().onClick.AddListener(action);
        }

        private void CreateChildButton(Transform parent, string label, Vector2 anchor,
            UnityEngine.Events.UnityAction action)
        {
            GameObject obj = new GameObject(label + " Button",
                typeof(RectTransform), typeof(Image), typeof(Button));
            obj.transform.SetParent(parent, false);
            RectTransform rect = obj.GetComponent<RectTransform>();
            rect.anchorMin = rect.anchorMax = anchor;
            rect.sizeDelta = new Vector2(310f, 112f);
            obj.GetComponent<Image>().color = new Color(0.04f, 0.31f, 0.55f, 0.96f);
            Button button = obj.GetComponent<Button>();
            ColorBlock colors = button.colors;
            colors.highlightedColor = new Color(0.10f, 0.52f, 0.78f);
            colors.pressedColor = new Color(0.02f, 0.18f, 0.32f);
            button.colors = colors;
            button.onClick.AddListener(action);

            GameObject labelObject = new GameObject("Label", typeof(RectTransform), typeof(Text));
            labelObject.transform.SetParent(obj.transform, false);
            RectTransform labelRect = labelObject.GetComponent<RectTransform>();
            labelRect.anchorMin = Vector2.zero;
            labelRect.anchorMax = Vector2.one;
            labelRect.offsetMin = new Vector2(8f, 6f);
            labelRect.offsetMax = new Vector2(-8f, -6f);
            Text labelText = labelObject.GetComponent<Text>();
            labelText.font = uiFont;
            labelText.text = label;
            labelText.fontSize = 21;
            labelText.alignment = TextAnchor.MiddleCenter;
            labelText.color = Color.white;
            labelText.resizeTextForBestFit = true;
            labelText.resizeTextMinSize = 13;
            labelText.resizeTextMaxSize = 21;
            labelText.raycastTarget = false;
        }

        private GameObject CreateButtonObject(Transform parent, string label, Vector2 anchor, Vector2 size)
        {
            GameObject obj = new GameObject(label + " Button", typeof(RectTransform), typeof(Image), typeof(Button));
            obj.transform.SetParent(parent, false);
            RectTransform rect = obj.GetComponent<RectTransform>();
            rect.anchorMin = rect.anchorMax = anchor;
            rect.sizeDelta = new Vector2(1920f * size.x, 1080f * size.y);
            Image image = obj.GetComponent<Image>();
            image.color = new Color(0.04f, 0.31f, 0.55f, 0.94f);
            Button button = obj.GetComponent<Button>();
            ColorBlock colors = button.colors;
            colors.highlightedColor = new Color(0.10f, 0.52f, 0.78f);
            colors.pressedColor = new Color(0.02f, 0.18f, 0.32f);
            button.colors = colors;
            Text labelText = MakeText(obj.transform, label, 22, new Vector2(0.5f, 0.5f),
                new Vector2(0.92f, 0.8f), TextAnchor.MiddleCenter);
            labelText.raycastTarget = false;
            return obj;
        }

        private void SetStatus(string message)
        {
            if (statusText != null) statusText.text = message;
        }
    }
}