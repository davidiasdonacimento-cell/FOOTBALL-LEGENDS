using System.IO;
using FootballLegends.Lobby;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;
using UnityEngine.SceneManagement;

namespace FootballLegends.Editor
{
    [InitializeOnLoad]
    public static class ProjectSetup
    {
        private const string ScenePath = "Assets/Scenes/Lobby/Lobby.unity";

        static ProjectSetup()
        {
            EditorApplication.delayCall += EnsureProjectReady;
        }

        public static void EnsureProjectReady()
        {
            Directory.CreateDirectory("Assets/Scenes/Lobby");
            Directory.CreateDirectory("Assets/Scenes/Match");
            Directory.CreateDirectory("Assets/Prefabs/Lobby");
            Directory.CreateDirectory("Assets/Models/Players");
            Directory.CreateDirectory("Assets/Models/Stadium");
            Directory.CreateDirectory("Assets/Materials");
            Directory.CreateDirectory("Assets/Animations");
            Directory.CreateDirectory("Assets/Audio");

            if (!File.Exists(ScenePath))
            {
                Scene scene = EditorSceneManager.NewScene(NewSceneSetup.EmptyScene, NewSceneMode.Single);
                GameObject root = new GameObject("FOOTBALL LEGENDS - Lobby Bootstrap");
                root.AddComponent<LobbyBootstrap>();
                EditorSceneManager.SaveScene(scene, ScenePath);
            }

            EditorBuildSettings.scenes = new[]
            {
                new EditorBuildSettingsScene(ScenePath, true)
            };

            PlayerSettings.productName = "FOOTBALL LEGENDS";
            PlayerSettings.companyName = "Football Legends Studio";
            PlayerSettings.defaultInterfaceOrientation = UIOrientation.LandscapeLeft;
            PlayerSettings.SetApplicationIdentifier(BuildTargetGroup.Android, "com.footballlegends.game");
            PlayerSettings.Android.minSdkVersion = Android最低Sdk();
            PlayerSettings.Android.targetArchitectures = AndroidArchitecture.ARM64;
            PlayerSettings.SetScriptingBackend(BuildTargetGroup.Android, ScriptingImplementation.IL2CPP);
        }

        private static AndroidSdkVersions Android最低Sdk()
        {
            return AndroidSdkVersions.AndroidApiLevel26;
        }
    }
}
