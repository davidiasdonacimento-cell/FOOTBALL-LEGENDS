using System.IO;
using System.Linq;
using FootballLegends.Lobby;
using FootballLegends.Match;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEditor.Build.Reporting;
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

        public static void BuildAndroid()
        {
            EnsureProjectReady();
            Directory.CreateDirectory("build/Android");
            EditorUserBuildSettings.buildAppBundle = false;

            string[] scenes = EditorBuildSettings.scenes
                .Where(scene => scene.enabled)
                .Select(scene => scene.path)
                .ToArray();

            BuildPlayerOptions options = new BuildPlayerOptions
            {
                scenes = scenes,
                locationPathName = "build/Android/FOOTBALL-LEGENDS.apk",
                target = BuildTarget.Android,
                options = BuildOptions.None
            };

            BuildReport report = BuildPipeline.BuildPlayer(options);
            if (report.summary.result != BuildResult.Succeeded)
                throw new System.Exception("Android build failed: " + report.summary.result);
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

            const string matchScenePath = "Assets/Scenes/Match/Match.unity";
            if (!File.Exists(matchScenePath))
            {
                Scene matchScene = EditorSceneManager.NewScene(NewSceneSetup.EmptyScene, NewSceneMode.Single);
                GameObject matchRoot = new GameObject("FOOTBALL LEGENDS - Match Bootstrap");
                matchRoot.AddComponent<MatchBootstrap>();
                EditorSceneManager.SaveScene(matchScene, matchScenePath);
            }

            EditorBuildSettings.scenes = new[]
            {
                new EditorBuildSettingsScene(ScenePath, true),
                new EditorBuildSettingsScene(matchScenePath, true)
            };

            PlayerSettings.productName = "FOOTBALL LEGENDS";
            PlayerSettings.companyName = "Football Legends Studio";
            PlayerSettings.defaultInterfaceOrientation = UIOrientation.LandscapeLeft;
            PlayerSettings.SetApplicationIdentifier(BuildTargetGroup.Android, "com.footballlegends.game");
            PlayerSettings.Android.minSdkVersion = GetMinimumAndroidSdkVersion();
            PlayerSettings.Android.targetArchitectures = AndroidArchitecture.ARM64;
            PlayerSettings.SetScriptingBackend(BuildTargetGroup.Android, ScriptingImplementation.IL2CPP);
        }

        private static AndroidSdkVersions GetMinimumAndroidSdkVersion()
        {
            return AndroidSdkVersions.AndroidApiLevel26;
        }
    }
}
