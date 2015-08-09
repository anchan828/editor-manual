= シーンアセットにスクリプトをアタッチ

//indepimage[frontispiece]

//lead{
事の発端は、「空のゲームオブジェクトにスクリプトをアタッチするくらいなら、シーンファイルにスクリプトをアタッチしたい」というツイートを見かけたからです。面白そうだったので作ってみました。
//}

//pagebreak



== GlobalManager のようなゲームオブジェクト

全体を管理するための管理クラスなどを空のオブジェクトにアタッチしていませんか？

//image[ss01][私もよくやる Manager クラス]{

//}

このような管理クラスは、時には必要不可欠なものです。

=== これがもしシーンにスクリプトをアタッチできたら？

もしシーンファイルにスクリプトをアタッチすることが出来るのであれば、管理クラスのためにゲームオブジェクトを作成する必要がなくなります。それ以外は特に大きな変化はありませんが「もしかすると」こちらのほうが管理のイメージが楽になり、ミスも減るかもしれません。

//image[ss02][空のゲームオブジェクトを生成せずにスクリプトをアタッチ出来る！]{

//}

本章では、シーンアセットにスクリプトをアタッチするまでに使った技術を解説していきます。


== シーンアセットのカスタムエディター

まずはシーンファイルのインスペクターをカスタマイズできるようにしなければいけません。ですが、Scene クラスというものは存在せず、少し工夫が必要になります。

=== DefaultAsset

UnityEditor.DefaultAsset というクラスがあります。これは「@<b>{クラスの用意されていない Unity 上で扱うアセット}」が全て DefaultAsset となります。現在は@<b>{フォルダー}と@<b>{シーンアセット}が DefaultAsset クラスの扱いとなっています。

=== DefaultAsset のカスタムエディター

まずは、DefaultAsset のカスタムエディターを作成します。そして OnEnable 内で、現在表示しているオブジェクトはフォルダーなのかシーンアセットなのかを判断します。

シーンファイルであれば、必要な GUI を描画していきます。

//image[ss03][シーンアセットを選択するとインスペクターにラベルが表示される]{

//}

1点だけ気をつけて欲しいのは、フォルダーとシーンアセットのインスペクターは @<code>{GUI.enabled = false} となっており、GUI を操作することが不可能になっています。強制的に true を設定してあげなければいけません。

//emlist{
[CustomEditor (typeof(DefaultAsset))]
public class SceneInspector : Editor
{
    bool isSceneInspector = false;

    void OnEnable ()
    {
        var assetPath = AssetDatabase.GetAssetPath (target);

        isSceneInspector = Path.GetExtension (assetPath) == ".unity";
    }

    public override void OnInspectorGUI ()
    {
        if (isSceneInspector == false)
            return;

        GUI.enabled = true;

        EditorGUILayout.LabelField ("シーンアセットのインスペクター！");
    }
}
//}

== シーンアセットにアタッチしたコンポーネントをどこに保持するか

通常はシーンアセットにコンポーネントをアタッチすることは出来ません。また、エディター拡張によってシーンアセットにコンポーネント情報を追加することも出来ません。

なので今回は@<b>{見た目だけシーンアセットにコンポーネントがアタッチ出来る}仕組みを実装します。

=== シーンアセットに対応したプレハブを生成する

シーンにアタッチするコンポーネントは@<b>{自動生成されたプレハブ}に保持します。

特定のフォルダー（他のユーザーが触らないEditorフォルダ以下が好ましい）配下にプレハブを作成します。この時、プレハブ名をシーンアセットと関連するものにしますが、シーン名（パスも同様）は同名のものが存在する可能性があるので guid で管理します。

まずは、そのためのユーティリティクラスを作成します。

//emlist{
using UnityEngine;
using UnityEditor;
using System.IO;

public class ScenePrefabUtility
{
    const string PREFAB_FOLDER_PATH = "Assets/Editor/ScenePrefabs";

    [InitializeOnLoadMethod]
    static void CreatePrefabFolder ()
    {
        Directory.CreateDirectory (PREFAB_FOLDER_PATH);
    }

    public static GameObject CreateScenePrefab (string scenePath, 
                                        params System.Type[] components)
    {
        var guid = ScenePathToGUID (scenePath);

        // HideFlags はコンパイルエラーなどの予想外のエラーによって中断された時の対策として
        // 非表示 & 保存禁止
        var go = EditorUtility.CreateGameObjectWithHideFlags (guid, 
                                     HideFlags.HideAndDontSave, components);
        
        var prefabPath = string.Format ("{0}/{1}.prefab", PREFAB_FOLDER_PATH, guid);

        var prefab = PrefabUtility.CreatePrefab (prefabPath, go);

        // プレハブ生成のために作成したゲームオブジェクトは破棄
        Object.DestroyImmediate (go);

        return prefab;
    }

    // プレハブ名をシーンアセットの guid にする
    public static GameObject GetScenePrefab (string scenePath)
    {
        // シーン名だと同名がある可能性があるので guid にする
        var guid = ScenePathToGUID (scenePath);
        var prefabPath = string.Format ("{0}/{1}.prefab", PREFAB_FOLDER_PATH, guid);
        return AssetDatabase.LoadAssetAtPath<GameObject> (prefabPath);
    }

    private static string ScenePathToGUID (string scenePath)
    {
        return AssetDatabase.AssetPathToGUID (scenePath);
    }
}
//}

上記コードを使用してインスペクターを表示する時にプレハブを取得、または生成します。

//emlist{
using UnityEngine;
using UnityEditor;
using System.IO;

[CustomEditor (typeof(DefaultAsset))]
public class SceneInspector : Editor
{
    bool isSceneInspector = false;

    GameObject scenePrefab;

    void OnEnable ()
    {
        var assetPath = AssetDatabase.GetAssetPath (target);

        isSceneInspector = Path.GetExtension (assetPath) == ".unity";

        if (isSceneInspector == false)
            return;

        // プレハブ取得
        scenePrefab = ScenePrefabUtility.GetScenePrefab (assetPath);

        // なければ生成
        if (scenePrefab == null)
            scenePrefab = ScenePrefabUtility.CreateScenePrefab (assetPath);

    }
}
//}

これで、シーンアセットを選択したと同時にプレハブが生成されるようになりました。

//image[ss04][プレハブ名はシーンアセットの guid]{

//}

== プレハブのコンポーネントをシーンアセットのインスペクターに表示

プレハブのコンポーネントを取得後 @<b>{コンポーネントに対応した Editor オブジェクトを生成}します。そして @<code>{Editor.OnInspectorGUI} を呼び出すことによって @<img>{ss05} のようなコンポーネントの表示を行うことが出来ます。

//image[ss05][各コンポーネントのプロパティが表示されている。いつも見る光景]{

//}

=== Editorオブジェクトの生成


Editor オブジェクトを生成します。

まずは、コンポーネントの取得です。これは「プレハブから GetComponents でコンポーネントを取得」するだけです。
その後 @<code>{Editor.CreateEditor} でエディターオブジェクトを生成します。

問題は エディターオブジェクトの保持の仕方です。まず Dictionary<Editor, bool> で保持しているのは @<code>{EditorGUI.InspectorTitlebar} を使用するために bool 値と Editor オブジェクトを紐付けるために使用しています。

//image[ss06][foldout の役割を持つため ▼ をクリックすることで開閉できる]{

//}

次に、Editor オブジェクトの破棄です。普段カスタムエディターの実装のために Editor オブジェクトは使用します。その時は自動的に Editor オブジェクトの生成/破棄が行なわれているため、意識していないかもしれませんが、通常何らかのオブジェクトを生成した場合は適切に破棄をする処理を加えなければいけません。

下記のコードでは OnDisable メソッドや Editor オブジェクトの（再）取得の時に、明示的に @<code>{Object.DestroyImmediate} を呼び出して破棄しています。

破棄せずに放置しておくと、GenericInspector オブジェクトが破棄されずにメモリが圧迫されていきます。

//image[ss07][メモリプロファイラーで見ると GenericInspector が大量にあることが分かる]{

//}


//emlist{
GameObject scenePrefab;

Dictionary<Editor,bool> activeEditors = new Dictionary<Editor, bool> ();

void OnEnable ()
{
    var assetPath = AssetDatabase.GetAssetPath (target);

    isSceneInspector = Path.GetExtension (assetPath) == ".unity";

    if (isSceneInspector == false)
        return;


    scenePrefab = ScenePrefabUtility.GetScenePrefab (assetPath);

    if (scenePrefab == null)
        scenePrefab = ScenePrefabUtility.CreateScenePrefab (assetPath);

    InitActiveEditors ();

}

void OnDisable ()
{
    ClearActiveEditors ();
}

// 生成した Editor オブジェクトの破棄
void ClearActiveEditors ()
{
    foreach (var activeEditor in activeEditors) {
        Object.DestroyImmediate (activeEditor.Key);
    }
    activeEditors.Clear ();
}

void InitActiveEditors ()
{
    ClearActiveEditors ();

    // コンポーネントから　Editor オブジェクトを生成
    foreach (var component in scenePrefab.GetComponents<Component> ()) {
        
        // Transform と RectTransform は省く
        // 本章の目的では必要ないと判断したため
        if (component is Transform || component is RectTransform)
            continue;

        activeEditors.Add (Editor.CreateEditor (component), true);
    }
}
//}

そして、実際にインスペクターに描画する GUI の部分です。

//emlist{
public override void OnInspectorGUI ()
{
    if (isSceneInspector == false)
        return;

    GUI.enabled = true;

    var editors = new List<Editor> (activeEditors.Keys);

    foreach (var editor in editors) {

        DrawInspectorTitlebar (editor);

        GUILayout.Space (-5f);

        if (activeEditors [editor] && editor.target != null)
            editor.OnInspectorGUI ();

        DrawLine ();
    }

    // コンテキストの Remove Component によって削除された場合、Editor.target は null になる
    // その時は初期化する
    if (editors.All (e => e.target != null) == false) {
        InitActiveEditors ();
        Repaint ();
    }
}

void DrawInspectorTitlebar (Editor editor)
{
    var rect = GUILayoutUtility.GetRect (GUIContent.none, 
                                         GUIStyle.none, 
                                         GUILayout.Height (20));
    rect.x = 0;
    rect.y -= 5;
    rect.width += 20;
    activeEditors [editor] = EditorGUI.InspectorTitlebar (rect, 
                                                          activeEditors [editor], 
                                                          new []{ editor.target });
}

void DrawLine ()
{
    EditorGUILayout.Space ();
    var lineRect = GUILayoutUtility.GetRect (GUIContent.none, 
                                             GUIStyle.none, 
                                             GUILayout.Height (2));
    lineRect.y -= 3;
    lineRect.width += 20;
    Handles.color = Color.black;

    var start = new Vector2 (0, lineRect.y);
    var end = new Vector2 (lineRect.width, lineRect.y);
    Handles.DrawLine (start, end);
}
//}

これで @<img>{ss08} のような表示ができるようになりました。ですがまだシーンアセットにコンポーネントをアタッチすることが出来ません。

//image[ss08][ゲームオブジェクトにアタッチされたようなコンポーネントと同じ表示]{

//}


== コンポーネントをインスペクターにドラッグ & ドロップ

次に@<img>{ss09}のようにコンポーネントをドラッグ & ドロップによってアタッチします。

//image[ss09][インスペクターの何もない空間にコンポーネントをドラッグ＆ドロップできる]{

//}

//emlist{
void OnEnable ()
{
    // Undo によって変更された状態を初期化
    Undo.undoRedoPerformed += InitActiveEditors;
}

void OnDisable ()
{
    Undo.undoRedoPerformed -= InitActiveEditors;
}

public override void OnInspectorGUI ()
{

    // 略

    // OnInspectorGUI の最後に実装

    // 残りの余った領域を取得
    var dragAndDropRect = GUILayoutUtility.GetRect (GUIContent.none, 
                                                     GUIStyle.none, 
                                                     GUILayout.ExpandHeight (true), 
                                                     GUILayout.MinHeight (200));

    switch (Event.current.type) {
        // ドラッグ中 or ドロップ実行
        case EventType.DragUpdated:
        case EventType.DragPerform:

            // マウス位置が指定の範囲外であれば無視
            if (dragAndDropRect.Contains (Event.current.mousePosition) == false)
                break;

            // カーソルをコピー表示にする
            DragAndDrop.visualMode = DragAndDropVisualMode.Copy;

            // ドロップ実行
            if (Event.current.type == EventType.DragPerform) {
                DragAndDrop.AcceptDrag ();

                // ドロップしたオブジェクトがスクリプトアセットかどうか
                var components = DragAndDrop.objectReferences
                    .Where (x => x.GetType () == typeof(MonoScript))
                    .OfType<MonoScript> ()
                    .Select (m => m.GetClass ());

                // コンポーネントをプレハブにアタッチ
                foreach (var component in components) {
                    Undo.AddComponent (scenePrefab, component);
                }

                InitActiveEditors ();
            }
            break;
    }

    // ドロップできる領域を確保
    GUI.Label (dragAndDropRect, "");
}
//}

これで基本的な機能が出来上がりました。

最後に、Unity エディターがクラッシュしてしまうと@<b>{プレハブの仕様に則って}未保存のコンポーネント情報が消えてしまうのでコンポーネントを追加した直後や @<code>{OnDisable} 内など適切なタイミングで @<code>{AssetDatabase.SaveAssets} を呼び出すといいかもしれません。

//emlist{
void OnDisable ()
{
    AssetDatabase.SaveAssets ();
}
//}

== シーン再生時&ビルド時に自動的にプレハブをインスタンス化

シーンアセットにアタッチしたコンポーネントはプレハブで管理されています。なので「どこかのタイミングで」シーン内にプレハブをインスタンス化しなければいけません。

シーン再生時やビルド時にプレハブのインスタンス化したゲームオブジェクトを含めるには、@<code>{UnityEditor.Callbacks} の @<code>{PostProcessSceneAttribute} を使用します。

//emlist{
using UnityEngine;
using UnityEditor;
using System.IO;
using UnityEditor.Callbacks;

public class ScenePrefabUtility
{
    [PostProcessScene]
    static void OnPostProcessScene ()
    {
        // 現在開いているシーンからシーンパスを取得
        var scenePath = EditorBuildSettings.scenes [Application.loadedLevel].path;

        if (string.IsNullOrEmpty (scenePath))
            return;

        // 自動で生成しているプレハブを取得
        var prefab = GetScenePrefab (scenePath);

        // インスタンス化
        if (prefab) 
            GameObject.Instantiate (prefab).name = "ScenePrefab";
    }
}
//}

これで「シーンアセットにスクリプトをアタッチ」から「実際に使用する」ことが可能になりました。