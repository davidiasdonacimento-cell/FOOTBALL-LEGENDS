using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

namespace FootballLegends.Lobby
{
    public sealed class LobbyBootstrap : MonoBehaviour
    {
        private const float UiScale = 1f;
        private Font uiFont;

        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.AfterSceneLoad)]
        private static void EnsureLobbyRuntime()
        {
            if (SceneManager.GetActiveScene().name == "Match") return;
            if (FindObjectOfType<LobbyBootstrap>() == null)
                new GameObject("FOOTBALL LEGENDS - Runtime Lobby").AddComponent<LobbyBootstrap>();
        }

        private void Awake()
        {
            Screen.orientation = ScreenOrientation.LandscapeLeft;
            Application.targetFrameRate = 60;
            QualitySettings.vSyncCount = 0;
            BuildEnvironment();
            BuildInterface();
        }

        private void BuildEnvironment()
        {
            Camera camera = Camera.main;
            if (camera == null)
            {
                GameObject cameraObject = new GameObject("Lobby Camera");
                camera = cameraObject.AddComponent<Camera>();
                cameraObject.tag = "MainCamera";
            }

            camera.transform.position = new Vector3(0f, 5.2f, -12f);
            camera.transform.LookAt(new Vector3(0f, 1.4f, 2f));
            camera.fieldOfView = 48f;
            camera.backgroundColor = new Color(0.035f, 0.075f, 0.12f);
            camera.clearFlags = CameraClearFlags.SolidColor;

            RenderSettings.ambientLight = new Color(0.48f, 0.53f, 0.62f);
            RenderSettings.fog = true;
            RenderSettings.fogColor = new Color(0.10f, 0.16f, 0.22f);
            RenderSettings.fogDensity = 0.012f;

            GameObject lightObject = new GameObject("Stadium Key Light");
            Light sun = lightObject.AddComponent<Light>();
            sun.type = LightType.Directional;
            sun.intensity = 1.15f;
            lightObject.transform.rotation = Quaternion.Euler(48f, -28f, 0f);

            CreatePrimitive("Pitch", PrimitiveType.Plane, new Vector3(0f, 0f, 4f),
                new Vector3(1.8f, 1f, 2.8f), new Color(0.08f, 0.32f, 0.18f));

            CreatePrimitive("Stands Left", PrimitiveType.Cube, new Vector3(-8f, 2.3f, 5f),
                new Vector3(3f, 4.6f, 15f), new Color(0.12f, 0.16f, 0.22f));
            CreatePrimitive("Stands Right", PrimitiveType.Cube, new Vector3(8f, 2.3f, 5f),
                new Vector3(3f, 4.6f, 15f), new Color(0.12f, 0.16f, 0.22f));
            CreatePrimitive("Upper Stand", PrimitiveType.Cube, new Vector3(0f, 4f, 12f),
                new Vector3(19f, 4f, 2f), new Color(0.09f, 0.13f, 0.19f));

            GameObject player = CreatePrimitive("Player Showcase - Blockout",
                PrimitiveType.Capsule, new Vector3(-3.4f, 1.15f, 1.5f),
                new Vector3(0.72f, 1.15f, 0.72f), new Color(0.78f, 0.82f, 0.9f));
            player.transform.rotation = Quaternion.Euler(0f, 18f, 0f);

            CreatePrimitive("Player Head - Blockout", PrimitiveType.Sphere,
                new Vector3(-3.4f, 2.55f, 1.5f), new Vector3(0.48f, 0.48f, 0.48f),
                new Color(0.72f, 0.55f, 0.42f));
        }

        private static Font LoadUiFont()
        {
            Font font = Resources.GetBuiltinResource<Font>("LegacyRuntime.ttf");
            if (font == null) font = Resources.GetBuiltinResource<Font>("Arial.ttf");
            if (font == null) font = Font.CreateDynamicFontFromOSFont("Arial", 18);
            return font;
        }

        private static Shader FindRuntimeShader()
        {
            Shader shader = Shader.Find("Standard");
            if (shader == null) shader = Shader.Find("Universal Render Pipeline/Lit");
            if (shader == null) shader = Shader.Find("Sprites/Default");
            return shader;
        }

        private GameObject CreatePrimitive(string objectName, PrimitiveType type,
            Vector3 position, Vector3 scale, Color color)
        {
            GameObject item = GameObject.CreatePrimitive(type);
            item.name = objectName;
            item.transform.position = position;
            item.transform.localScale = scale;
            Renderer renderer = item.GetComponent<Renderer>();
            Shader shader = FindRuntimeShader();
            if (renderer != null && shader != null)
            {
                Material material = new Material(shader);
                material.color = color;
                renderer.material = material;
            }
            return item;
        }

        private void BuildInterface()
        {
            uiFont = LoadUiFont();

            GameObject canvasObject = new GameObject("Lobby Canvas",
                typeof(Canvas), typeof(CanvasScaler), typeof(GraphicRaycaster));
            Canvas canvas = canvasObject.GetComponent<Canvas>();
            canvas.renderMode = RenderMode.ScreenSpaceOverlay;
            CanvasScaler scaler = canvasObject.GetComponent<CanvasScaler>();
            scaler.uiScaleMode = CanvasScaler.ScaleMode.ScaleWithScreenSize;
            scaler.referenceResolution = new Vector2(1920f, 1080f);
            scaler.matchWidthOrHeight = 0.5f;

            if (FindObjectOfType<EventSystem>() == null)
                new GameObject("EventSystem", typeof(EventSystem), typeof(StandaloneInputModule));

            LobbyController controller = canvasObject.AddComponent<LobbyController>();

            GameObject home = MakePanel(canvas.transform, "Home Panel",
                new Vector2(0.55f, 0.5f), new Vector2(0.86f, 0.88f),
                new Color(0.025f, 0.055f, 0.10f, 0.82f));

            MakeText(home.transform, "FOOTBALL LEGENDS", 42, new Vector2(0.52f, 0.88f),
                new Vector2(0.88f, 0.12f), TextAnchor.MiddleCenter, Color.white);
            MakeText(home.transform, "CENTRAL DO CLUBE", 18, new Vector2(0.52f, 0.77f),
                new Vector2(0.8f, 0.07f), TextAnchor.MiddleCenter, new Color(0.35f, 0.83f, 1f));

            CreateMenuButton(home.transform, "JOGAR", new Vector2(0.78f, 0.60f), controller);
            CreateMenuButton(home.transform, "ELENCO", new Vector2(0.78f, 0.46f), controller);
            CreateMenuButton(home.transform, "EVENTOS", new Vector2(0.78f, 0.32f), controller);
            CreateMenuButton(home.transform, "PERFIL", new Vector2(0.78f, 0.18f), controller);

            GameObject content = MakePanel(canvas.transform, "Section Panel",
                new Vector2(0.5f, 0.5f), new Vector2(0.78f, 0.78f),
                new Color(0.025f, 0.055f, 0.10f, 0.96f));
            Text title = MakeText(content.transform, "SEÇÃO", 40,
                new Vector2(0.5f, 0.76f), new Vector2(0.85f, 0.18f),
                TextAnchor.MiddleCenter, Color.white);
            Text description = MakeText(content.transform, "", 24,
                new Vector2(0.5f, 0.57f), new Vector2(0.82f, 0.14f),
                TextAnchor.MiddleCenter, new Color(0.82f, 0.88f, 0.95f));
            Button playMatch = CreateButton(content.transform, "JOGAR PARTIDA",
                new Vector2(0.5f, 0.38f), new Vector2(0.38f, 0.11f));
            playMatch.onClick.AddListener(controller.StartPlayerMatch);
            Button refereeMatch = CreateButton(content.transform, "SER ÁRBITRO • IA x IA",
                new Vector2(0.5f, 0.23f), new Vector2(0.38f, 0.11f));
            refereeMatch.onClick.AddListener(controller.StartRefereeMatch);
            Button back = CreateButton(content.transform, "VOLTAR",
                new Vector2(0.5f, 0.08f), new Vector2(0.30f, 0.10f));

            content.SetActive(false);
            playMatch.gameObject.SetActive(false);
            refereeMatch.gameObject.SetActive(false);
            back.onClick.AddListener(controller.ReturnToLobby);
            controller.Initialize(home, content, title, description, playMatch.gameObject, refereeMatch.gameObject);
        }

        private GameObject MakePanel(Transform parent, string panelName,
            Vector2 anchor, Vector2 size, Color color)
        {
            GameObject panel = new GameObject(panelName, typeof(RectTransform), typeof(Image));
            panel.transform.SetParent(parent, false);
            RectTransform rect = panel.GetComponent<RectTransform>();
            rect.anchorMin = anchor;
            rect.anchorMax = anchor;
            rect.sizeDelta = new Vector2(1920f * size.x, 1080f * size.y);
            Image image = panel.GetComponent<Image>();
            image.color = color;
            return panel;
        }

        private Text MakeText(Transform parent, string value, int size,
            Vector2 anchor, Vector2 normalizedSize, TextAnchor alignment, Color color)
        {
            GameObject item = new GameObject(value + " Text", typeof(RectTransform), typeof(Text));
            item.transform.SetParent(parent, false);
            RectTransform rect = item.GetComponent<RectTransform>();
            rect.anchorMin = anchor;
            rect.anchorMax = anchor;
            rect.sizeDelta = new Vector2(1920f * normalizedSize.x, 1080f * normalizedSize.y);
            Text text = item.GetComponent<Text>();
            text.font = uiFont;
            text.text = value;
            text.fontSize = size;
            text.alignment = alignment;
            text.color = color;
            text.resizeTextForBestFit = true;
            text.resizeTextMinSize = 14;
            text.resizeTextMaxSize = size;
            return text;
        }

        private void CreateMenuButton(Transform parent, string label,
            Vector2 anchor, LobbyController controller)
        {
            Button button = CreateButton(parent, label, anchor, new Vector2(0.31f, 0.105f));
            button.onClick.AddListener(() => controller.OpenSection(label));
        }

        private Button CreateButton(Transform parent, string label,
            Vector2 anchor, Vector2 normalizedSize)
        {
            GameObject item = new GameObject(label + " Button",
                typeof(RectTransform), typeof(Image), typeof(Button));
            item.transform.SetParent(parent, false);
            RectTransform rect = item.GetComponent<RectTransform>();
            rect.anchorMin = anchor;
            rect.anchorMax = anchor;
            rect.sizeDelta = new Vector2(1920f * normalizedSize.x, 1080f * normalizedSize.y);

            Image image = item.GetComponent<Image>();
            image.color = new Color(0.06f, 0.34f, 0.56f, 0.96f);
            Button button = item.GetComponent<Button>();
            ColorBlock colors = button.colors;
            colors.highlightedColor = new Color(0.10f, 0.52f, 0.78f);
            colors.pressedColor = new Color(0.035f, 0.20f, 0.36f);
            button.colors = colors;

            Text text = MakeText(item.transform, label, 25, new Vector2(0.5f, 0.5f),
                new Vector2(0.92f, 0.8f), TextAnchor.MiddleCenter, Color.white);
            text.raycastTarget = false;
            return button;
        }
    }
}
