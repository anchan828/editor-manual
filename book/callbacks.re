= さまざまなイベントのコールバック

Unityでは、「ビルド後」「コンパイル後」「プラットフォームの変更後」など、特定のアクションの後に呼び出されるさまざまなコールバックが存在します。
これらのコールバックの中には、使用することで作業の自動化が行える「知らなければ損」なコールバックもあります。

== PostProcessBuildAttribute

Build Settings ウィンドウや @<code>{BuildPipeline.BuildPlayer} によって行われるアプリのビルド後に呼び出されるコールバックです。

ビルドで生成された、Xcode プロジェクトや Android Project などで、成果物をさらに加工したり、アプリを生成するまで自動化することも可能です。
CIツールと組み合わせたり、ゲーム開発において必須と言えるくらい重要なコールバックなのでぜひ覚えておきましょう。

以下のサンプルでは @<code>{Xcode Manipulation API}@<fn>{xcode_api} を使ってフレームワークを追加しています。

//emlist{
using UnityEditor;
using UnityEditor.Callbacks;
using System.IO;
using UnityEditor.iOS.Xcode;

public class NewBehaviourScript
{
    // callbackOrder で実行順を指定することが出来る
    // 0 が内部で使われている order なので 1以上を指定する
    [PostProcessBuild(1)]
    static void OnPostProcessBuild (BuildTarget buildTarget, string path)
    {
        if (buildTarget != BuildTarget.iOS)
            return;

        // Xcode プロジェクトのパスを取得
        var xcodeprojPath = Path.Combine (path, "Unity-iPhone.xcodeproj");
        var pbxprojPath = Path.Combine (xcodeprojPath, "project.pbxproj");

        // Xcode プロジェクトロード
        PBXProject proj = new PBXProject ();
        proj.ReadFromFile (pbxprojPath);

        var target = proj.TargetGuidByName("Unity-iPhone");

        proj.AddFrameworkToProject (target, "Social.framework", false);
        proj.WriteToFile (pbxprojPath);
    }
}
//}


//footnote[xcode_api][@<href>{http://docs.unity3d.com/ScriptReference/iOS.Xcode.PBXProject.html}]

== PostProcessSceneAttribute

シーンの再生時やビルド時のシーン構築時などの「シーンをロードした後」に呼び出されるコールバックです。この属性を付けたメソッド内でゲームオブジェクトやプレハブから生成をすることで、「手動で Prefab をシーンに含める作業」以上の強制力を持たせることができます。

//image[ss01][ゲームを再生すると自動的にゲームオブジェクトが生成される]{
//}

//emlist{
using UnityEngine;
using UnityEditor;
using UnityEditor.Callbacks;

public class NewBehaviourScript
{
    [PostProcessScene]
    static void OnPostProcessScene ()
    {
        new GameObject ("OnPostProcessScene: " + Application.loadedLevelName);
    }
}
//}

ただし、ゲーム再生時とビルド時でシーンアセットのパスを取得する方法に問題が発生してしまいます。
通常、現在開いているシーンのパスを取得するには @<code>{EditorApplication.currentScene} を使用します。
エディター上でゲームを再生するときは大丈夫なのですが、問題なのはビルド時です。ビルド時は @<b>{Editor API の一部が動作しません。}EditorApplication.currentScene もその影響を受けてしまい、ビルド時には常に空文字を返してしまいます。

//image[ss02][左がビルド時の Debug.Log。右がエディター上でゲーム再生時の Debug.Log]{

//}

//emlist{
using UnityEditor;
using UnityEditor.Callbacks;
using UnityEngine;

public class NewBehaviourScript
{
    [PostProcessScene]
    static void OnPostProcessScene ()
    {
        var currentScenePath = EditorApplication.currentScene;

        // ゲーム再生時はシーンのパスを取得できる
        // ビルド時は空文字
        Debug.Log (currentScenePath);
    }
}
//}

対策としては、@<code>{Application.loadedLevel} と @<code>{EditorBuildSettings.scenes} を組み合わせてシーンファイルのパスを取得します。

//image[ss03][左がビルド時の Debug.Log。右がエディター上でゲーム再生時の Debug.Log]{

//}

@<code>{Application.loadedLevel} は Build Settings の「Scenens In Build」に登録しているシーンのインデックスを表しています。そして、@<code>{EditorBuildSettings.scenes} は Scenens In Build で設定されているシーン情報の配列を持っています。このインデックスとシーン配列は正しく関係性を持っており、以下のコードのように使用することでシーンのパスが取得できるようになります。

//emlist{
[PostProcessScene]
static void OnPostProcessScene ()
{
    var index = Application.loadedLevel;
    var currentScenePath = EditorBuildSettings.scenes [index].path;

    Debug.Log (currentScenePath);
}
//}

PostProcessScene を使った実装例は @<chapref>{attach_script_to_scene} で紹介しています。ぜひご覧ください。

== InitializeOnLoad

Unity エディターの起動時や、スクリプトのコンパイル直後にクラスの静的コンストラクタを呼び出すための属性です。コンパイル直後に毎回行いたい処理、例えば、（コンパイルごとにデリゲートに登録したものがリセットされる理由で）デリゲートの再登録を行う目的として使用されます。

注意したいのはエディター上でゲームを再生した直後にも InitializeOnLoad 属性の付いた静的コンストラクタが呼び出されてしまいます。

この対策としては、静的コンストラクタ内で @<b>{EditorApplication.isPlayingOrWillChangePlaymode} を使い、ゲーム再生時に呼び出されたものであるかを判断します。

//emlist{
using UnityEditor;
using UnityEngine;

[InitializeOnLoad]
public class NewBehaviourScript
{
    static NewBehaviourScript ()
    {
        if (EditorApplication.isPlayingOrWillChangePlaymode)
            return;

        Debug.Log ("call");
    }
}
//}

また、Unityエディターを起動した直後のみ実行したい場合は、@<code>{EditorApplication.timeSinceStartup} を組み合わせて使用します。


//emlist{
using UnityEditor;
using UnityEngine;

[InitializeOnLoad]
public class NewBehaviourScript
{
    static NewBehaviourScript ()
    {
        // 10秒以内かどうか
        if (EditorApplication.timeSinceStartup > 10)
            return;

        Debug.Log ("起動時であると判断する");
    }
}
//}

== InitializeOnLoadMethod

InitializeOnLoad のメソッド版です。静的メソッドにこの属性を付加することで @<code>{InitializeOnLoad} と同じタイミング（正確にはInitializeOnLoadが先に呼び出されます）で呼び出すことができます。

//emlist{
using UnityEditor;
using UnityEngine;

public class NewBehaviourScript
{
    [InitializeOnLoadMethod]
    static void RunMethod ()
    {
    }
}
//}

== DidReloadScripts

機能的には @<code>{InitializeOnLoadMethod} とほぼ同じです。ただ1点だけ違うところは@<b>{実行順を選べる}点です。引数として @<code>{callbackOrder} があり、昇順に実行できます。

//emlist{
using UnityEditor;
using UnityEngine;
using UnityEditor.Callbacks;

public class NewBehaviourScript
{

    [DidReloadScripts(0)]
    static void First ()
    {
        Debug.Log ("最初に処理する");
    }

    [DidReloadScripts(1)]
    static void Second ()
    {
        Debug.Log ("次に処理する");
    }
}
//}

== EditorUserBuildSettings.activeBuildTargetChanged

Build Settings ウィンドウや @<code>{EditorUserBuildSettings.SwitchActiveBuildTarget} によって、@<b>{プラットフォームが変更された時}に呼び出されるコールバックです。プラットフォームごとに異なる設定、例えば BundleIdentifier の変更や、StreamingAssets や Resources フォルダーの中身の変更などで活用できます。

//emlist{
using UnityEditor;
using UnityEngine;

public class NewBehaviourScript
{

    [InitializeOnLoadMethod]
    static void ChangeBundleIdentifier ()
    {
        // プラットフォームごとに bundleIdentifier を切り替える
        EditorUserBuildSettings.activeBuildTargetChanged += () => {

            var bundleIdentifier = "com.kyusyukeigo.superapp";

            switch (EditorUserBuildSettings.activeBuildTarget) {
                case BuildTarget.iOS:
                    bundleIdentifier += ".ios";
                    break;
                case BuildTarget.Android:
                    bundleIdentifier += ".android";
                    break;
                case BuildTarget.WSAPlayer:
                    bundleIdentifier += ".wp";
                    break;
                default:
                    break;
            }

            if (Debug.isDebugBuild)
                bundleIdentifier += ".dev";

            PlayerSettings.bundleIdentifier = bundleIdentifier;
        };
    }
}
//}

== EditorApplication.hierarchyWindowChanged と projectWindowChanged

選択しているオブジェクトを変更したり、新規でゲームオブジェクトを作成するなど、さまざまな要素によってヒエラルキーやプロジェクトの構成が変化した時に呼び出されます。

シーン内のゲームオブジェクトを対象に、何らかの処理をしている場合に使用すると便利です。

下記コードは、シーン内のカメラをリストアップするコードです。

//image[ss09][シーンビューの左上にウィンドウが追加された]{

//}

//emlist{
using UnityEditor;
using UnityEngine;
using System.Linq;

public class NewBehaviourScript
{

    [InitializeOnLoadMethod]
    static void DrawCameraNames ()
    {

        var selected = 0;
        var displayNames = new string[0];
        var windowRect = new Rect (10, 20, 100, 24);

        // 変更があれば初期化
        EditorApplication.hierarchyWindowChanged += () => {
            var cameras = Object.FindObjectsOfType<Camera> ();
            displayNames = new string[]{ "None", "" };
            ArrayUtility.AddRange (ref displayNames,
                 cameras.Select (c => c.name).ToArray ());
        };

        // 任意のタイミングで呼び出すことも出来る
        EditorApplication.hierarchyWindowChanged ();

        // 全シーンビューのGUIデリゲート
        SceneView.onSceneGUIDelegate += (sceneView) => {

            GUI.skin = EditorGUIUtility.GetBuiltinSkin (EditorSkin.Inspector);

            Handles.BeginGUI ();

            int windowID =
                EditorGUIUtility.GetControlID (FocusType.Passive, windowRect);
            // シーンビューにウィンドウを追加
            windowRect = GUILayout.Window (windowID, windowRect, (id) => {

                selected = EditorGUILayout.Popup (selected, displayNames);

                // ドラッグできるように
                GUI.DragWindow ();

            }, "Window");

            Handles.EndGUI ();
        };
    }
}
//}

さらに上記のコードにシーンのカメラとゲーム内のカメラの Transform を同期させると、シーンビューのカメラの位置/向きがそのままゲームビューにも反映されます。@<fn>{sync_camera}

//image[ss10][Unite 2014 で発表した Sync Camera の機能になる]{

//}

//footnote[sync_camera][@<href>{https://github.com/anchan828/unitejapan2014/tree/master/SyncCamera/Assets}]

== EditorApplication.hierarchyWindowItemOnGUI と projectWindowItemOnGUI

ヒエラルキーやプロジェクトウィンドウで、各ゲームオブジェクトやアセットの@<b>{文字が描画されている範囲}をコールバックとして取得できます。

//image[ss11][各ゲームオブジェクト名が描画されてる範囲を取得できる]{

//}

これらは、小さな範囲の中でゲームオブジェクトの情報を表示してもいいですし、ボタンなどを配置して何らかのトリガーとするのもいいかもしれません。

だた、注意して欲しいのはアセットストアにあるアセットでも、@<code>{hierarchyWindowItemOnGUI} を使用している可能性がある点です。もし、使用していた場合、かつ、何らかのトリガーとなっている場合はhierarchyWindowItemOnGUI の使用は控えたほうがいいかもしれません。

//image[ss04][各ゲームオブジェクトの右端にアタッチされているコンポーネントを表示]{

//}

//emlist{
using UnityEditor;
using UnityEngine;

public class NewBehaviourScript
{

    [InitializeOnLoadMethod]
    static void DrawComponentIcons ()
    {
        EditorApplication.hierarchyWindowItemOnGUI += (instanceID, selectionRect) => {
            // インスタンスIDからゲームオブジェクトを取得
            var go = (GameObject)EditorUtility.InstanceIDToObject (instanceID);

            if (go == null)
                return;

            var position = new Rect (selectionRect) {
                width = 16,
                height = 16,
                x = Screen.width - 20
            };

            foreach (var component in go.GetComponents<Component>()) {
                // Transform は全ゲームオブジェクトにあり
                // 無駄な情報なので表示しない
                if (component is Transform)
                    continue;

                var icon = AssetPreview.GetMiniThumbnail (component);

                GUI.Label (position, icon);
                position.x -= 16;
            }
        };
    }
}
//}

== EditorApplication.playmodeStateChanged

@<icon>{ss05}の再生状態を切り替えた時に呼び出されるコールバックです。ただし、引数もなく@<b>{「現在の再生状態はなにか」}はユーザーの手で判断しなくてはいけません。

//emlist{
[InitializeOnLoadMethod]
static void CheckPlaymodeState ()
{
    EditorApplication.playmodeStateChanged += () => {

        if (EditorApplication.isPaused) {
            // 一時停止中
        }

        if (EditorApplication.isPlaying) {
            // 再生中
        }

        if (EditorApplication.isPlayingOrWillChangePlaymode) {
            // 再生中または再生ボタンを押した直後
            // コンパイルやさまざまな処理が走っている状態
        }
    };
}
//}

下記は「コンパイルエラーが出たまま再生ボタンを押すと、ドラクエの呪いの効果音を再生する」サンプル

//emlist{
using UnityEngine;
using UnityEditor;
using System.Reflection;

[InitializeOnLoad]
public class CompileError
{
    // 効果音。自由に変更する
    // http://commons.nicovideo.jp/material/nc32797
    const string musicPath = "Assets/Editor/nc32797.wav";
    const BindingFlags flags = BindingFlags.NonPublic | BindingFlags.Instance;

    static CompileError ()
    {
        EditorApplication.playmodeStateChanged += () => {

            // 再生ボタンをおした時であること
            if (!EditorApplication.isPlayingOrWillChangePlaymode
                 && EditorApplication.isPlaying)
                return;

            // SceneViewが存在すること
            if (SceneView.sceneViews.Count == 0)
                return;

            EditorApplication.delayCall += () => {
                var content = typeof(EditorWindow)
                    .GetField ("m_Notification", flags)
                    .GetValue (SceneView.sceneViews [0]) as GUIContent;

                if (content != null && !string.IsNullOrEmpty (content.text)) {
                    GetAudioSource ().Play ();
                }
            };
        };
    }

    static AudioSource GetAudioSource ()
    {
        var gameObjectName = "HideAudioSourceObject";
        var gameObject = GameObject.Find (gameObjectName);

        if (gameObject == null) {
            //HideAndDontSaveフラグを立てて非表示・保存しないようにする
            gameObject =
                EditorUtility.CreateGameObjectWithHideFlags (gameObjectName,
                    HideFlags.HideAndDontSave, typeof(AudioSource));
        }

        var hideAudioSource = gameObject.GetComponent<AudioSource> ();

        if (hideAudioSource.clip == null) {
            hideAudioSource.clip =
                AssetDatabase.LoadAssetAtPath (musicPath,
                                            typeof(AudioClip)) as AudioClip;
        }

        return hideAudioSource;
    }
}
//}

== EditorApplication.modifierKeysChanged

何らかの修飾キーを押した時に呼び出されるコールバックです。EditorWindow やインスペクターでは修飾キーを押しただけでは GUI の再描画処理が行われません。それを可能にするためにこのコールバックを使用します。

//emlist{
using UnityEditor;
using UnityEngine;

public class NewBehaviourScript : EditorWindow
{
    [MenuItem ("Window/Example")]
    static void CheckModifierKeysChanged ()
    {
        GetWindow<NewBehaviourScript> ();
    }

    void OnEnable ()
    {
        EditorApplication.modifierKeysChanged += Repaint;
    }

    void OnGUI ()
    {
        GUILayout.Label (Event.current.modifiers.ToString());
    }
}

//}

== EditorApplication.update

Unity エディターの更新タイミングで呼び出されるコールバックです。エディターにも MonoBehaviour の Update のような一定の更新タイミングがあります。呼び出されるのは約 200回/秒 となり、描画系処理の前に実行されます。@<fn>{2}

例えば WWW の通信でも使用できます。

//emlist{
using UnityEditor;
using UnityEngine;
using System;
using System.IO;

public class NewBehaviourScript
{

    [InitializeOnLoadMethod]
    static void TestWWW ()
    {
        // 画像を取得して保存する
        EditorWWW ("http://placehold.it/350x150", (www) => {

            var assetPath = "Assets/New Texture.png";
            File.WriteAllBytes (assetPath, www.bytes);
            AssetDatabase.ImportAsset(assetPath);

        });
    }

    static void EditorWWW (string url, Action<WWW> callback)
    {
        var www = new WWW (url);

        EditorApplication.CallbackFunction update = null;

        update = () => {
            // 毎フレームチェック
            if (www.isDone && string.IsNullOrEmpty (www.error)) {
                callback (www);
                EditorApplication.update -= update;
            }
        };

        EditorApplication.update += update;
    }
}
//}

この他にも@<b>{独自のコールバックを実装するときにも使用できます。}

下記コードは、フォーカスしている EditorWindow が変更するごとに呼び出されるコールバックを作成します。
//emlist{
using UnityEngine;
using UnityEditor;
using System;

[InitializeOnLoad]
class EditorApplicationUtility
{
    public static Action<EditorWindow> focusedWindowChanged;

    static EditorWindow currentFocusedWindow;

    static EditorApplicationUtility ()
    {
        EditorApplication.update += FocusedWindowChanged;

    }

    static void FocusedWindowChanged ()
    {
        if (currentFocusedWindow != EditorWindow.focusedWindow) {
            currentFocusedWindow = EditorWindow.focusedWindow;
            focusedWindowChanged (currentFocusedWindow);
        }
    }
}


[InitializeOnLoad]
public class Test
{
    static Test ()
    {
        EditorApplicationUtility.focusedWindowChanged += (window) => {
            Debug.Log (window);
        };
    }
}
//}

@<chapref>{time_control}の様な時間管理のために使用するのもオススメです。

== EditorApplication.delayCall

インスペクター関連の更新処理後に実行されるコールバックです。1度実行すると登録されたデリゲートは破棄されます。

//image[ss06][Unityエディター実行の流れ]{

//}

使いドコロとしては、エディターのライフサイクルにどうしても逆らわなければいけない場合に使います。
例えば、MonoBehaviour の OnValidate メソッド内ではオブジェクトの破棄はできません。 OnValidate はインスペクターの更新中に実行されるため、オブジェクトの破棄が禁止されています。

//image[ss07][エラーが出る]{

//}

どうしてもオブジェクトの破棄を行いたい時に @<b>{EditorApplication.delayCall} を使用して、一番最後に処理を@<b>{後回し}させることでさまざまな問題を回避します。

//emlist{
using UnityEngine;

public class NewBehaviourScript : MonoBehaviour
{
    public GameObject go;

    #if UNITY_EDITOR
    void OnValidate ()
    {
        UnityEditor.EditorApplication.delayCall += () => {
            DestroyImmediate (go);
            go = null;
        };
    }
    #endif
}
//}

== EditorApplication.globalEventHandler

Unity エディター全体で何らかのイベントが実行された時に実行されるコールバックです。これは正式には公開されていませんが、何らかのキーイベントやマウス位置など把握したい時に使用すると便利です。


下記コードは、globalEventHandler を使いやすくしたラッパークラスです。

//image[ss08][適当なところでキーを押すとログが表示されていく]{

//}

//emlist{
using UnityEngine;
using UnityEditor;
using System.Reflection;
using CallbackFunction = UnityEditor.EditorApplication.CallbackFunction;

[InitializeOnLoad]
class EditorApplicationUtility
{
    static BindingFlags flags =
        BindingFlags.Static | BindingFlags.Instance | BindingFlags.NonPublic;

    static FieldInfo info = typeof(EditorApplication)
                                     .GetField ("globalEventHandler", flags);

    public static CallbackFunction globalEventHandler {
        get {
            return  (CallbackFunction)info.GetValue (null);
        }
        set {
            CallbackFunction functions = (CallbackFunction)info.GetValue (null);
            functions += value;
            info.SetValue (null, (object)functions);
        }
    }
}


[InitializeOnLoad]
public class Test
{
    static Test ()
    {
        EditorApplicationUtility.globalEventHandler += () => {
            Debug.Log (Event.current);
        };
    }
}
//}


//footnote[2][200回/秒実行されるのはバグとの報告があります。ドキュメントでは 100回/秒と記載されています。]
