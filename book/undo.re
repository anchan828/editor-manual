
= Undoについて

//lead{
エディター拡張によって変更した状態は「元に戻す」処理を自分で実装しなければいけません。そこでこの章では簡単なサンプルを元にUndoの使い方をマスターしていきます。
//}

== Undoの操作を実感してみる

まずはどのような操作がUndoなのか実感してみましょう。まずは、Cubeを作成します。

//image[ss01][@<code>{Assets/Create/3D Object/Cube}で作成した状態][scale=0.6]{

//}

次に@<code>{Edit/Undo Create Cube}を実行します。ショートカットキーで実行する場合は「command/ctrl + Z」です。

//image[ss02][メニュー名が@<code>{Undo Create Cube}ではない場合、余計な操作を行っている可能性があります。もう一度Cubeを生成しなおしてください。]{
//}

生成されたCubeが削除されましたか？Cubeを生成する前に戻ったことになります。これが「元に戻る = Undo」という操作です。

//pagebreak

== Undoの仕組み

Undoの管理はスタックで行われています。@<fn>{LIFO}

//footnote[LIFO][スタックは、後から入れたものを先に出す「LIFO（後入先出）」です。]

//image[stack][スタックのイメージ]{

//}

//indepimage[ss05]

//pagebreak

== Undoを実装してみる

Undoの操作を実感してみたところで、次はUndoを実装してみましょう。
 
//quote{
前回のUndo履歴をリセットするために@<code>{File/New Scene}で新規シーンにしておきましょう。
//}

=== オブジェクトの作成に対するUndo

下記コードはCubeを生成するためのコードです。@<code>{Example/Create Cube}を実行することによってCubeを生成できます。

//emlist{
using UnityEngine;
using UnityEditor;

public class Example
{
    [MenuItem("Example/Create Cube")]
    static void CreateCube ()
    {
        GameObject.CreatePrimitive (PrimitiveType.Cube);
    }
}
//}

Cubeを生成してもUndoを行うことはできません。これはUndoの実装が行われていないためです。

//image[ss03][Undoという文字が灰色になって選択できないことがわかる]{

//}

さっそく@<b>{Undoクラス}を使って、Undo を実装していきます。

Undo 操作の1例として @<code>{Undo.RegisterCreatedObjectUndo} 関数を使用してUndoを実装してみましょう。この関数はオブジェクトが生成された時に使用するUndoで、この関数によって登録されたオブジェクトは、Undo実行時に破棄されます。

//image[RegisterCreatedObjectUndo][RegisterCreatedObjectUndo関数とUndoを実行した時の動き]{

//}

@<img>{RegisterCreatedObjectUndo}では、RegisterCreatedObjectUndo関数を実行すると「Cubeを作成する」という処理を差分としてスタックに追加します@<fn>{f1}。Undoが実行される際には「Cubeを作成する前に戻す」つまり、Cubeを削除するという処理を行うことになります。

//footnote[f1][実際にはオブジェクトとUndo名がまとめられてスタックに保存されます。]

下記コードのように実装して、実行してみましょう。

//quote{
またUndo履歴をリセットするために@<code>{File/New Scene}で新規シーンにしましょう。
//}

//emlist{
using UnityEngine;
using UnityEditor;

public class Example
{
    [MenuItem("Example/Create Cube")]
    static void CreateCube ()
    {
        var cube = GameObject.CreatePrimitive (PrimitiveType.Cube);
        Undo.RegisterCreatedObjectUndo (cube, "Create Cube");
    }
}
//}

下の画像のようにUndoが登録されていれば成功です。実際にUndoしてみましょう。

//indepimage[ss04]

Undoを実行した後はUndoしたものを元に戻すRedo（取り消す）も実行できます。

//indepimage[ss06]

=== プロパティーの変さらに対するUndo

下記コードはオブジェクトの回転をランダムに設定するコードです。@<code>{Example/Random Rotate}を実行することによって選択しているオブジェクトをランダムに回転させることができます。

//emlist{
using UnityEngine;
using UnityEditor;

public class Example
{
    [MenuItem("Example/Random Rotate")]
    static void RandomRotate ()
    {
        var transform = Selection.activeTransform;

        if (transform) {
            transform.rotation = Random.rotation;
        }
    }
}
//}

//image[ss10][変更前と変更後]{

//}

これに@<code>{Undo.RecordObject}関数を使用することによってUndoを実装します。

//emlist{
using UnityEngine;
using UnityEditor;

public class Example
{
    [MenuItem("Example/Random Rotate")]
    static void RandomRotate ()
    {
        var transform = Selection.activeTransform;

        if (transform) {
            Undo.RecordObject (transform, "Rotate " + transform.name);
            transform.rotation = Random.rotation;
        }
    }
}
//}

ランダムな回転が設定される前に@<code>{Undo.RecordObject}関数を実行します。これにより「変更前」のTransformのプロパティーをUndoスタックに保存できるようになります。

//image[ss11][Undoが登録されているとメニューに表示され正しく回転を]{

//}

== どのプロパティーが変更されたかを知るPropertyDiffUndoRecorder

Undoのスタックに保存されるのは「値の変更前と変更後の差分」です。

なので「どのプロパティーが変更されたか」を知る必要があります。その役割を担うのが PropertyDiffUndoRecorder です。

//image[ss07][PropertyDiffUndoRecorderはプロファイラで確認できる]{

//}

PropertyDiffUndoRecorderは、Unityエディターのライフサイクルの最後に Undo の Flush を呼びだします。その時に RecordObject で登録されたオブジェクトの各プロパティーと、Flush が呼び出された時の各プロパティーを使用して差分を求めます。

以下の順に実行され、図にしたものが@<img>{PropertyDiffUndoRecorder}です。

 1. オブジェクトの登録
 2. 値の変更
 3. Flush実行
 4. 差分の出力

//image[PropertyDiffUndoRecorder][PropertyDiffUndoRecorderのサイクル]{

//}

//image[ss08][差分のイメージ（画像は@<href>{https://gist.github.com,gist}のdiffビュー）]{

//}

@<img>{PropertyDiffUndoRecorder}で示したサイクルを確認してみましょう。以下のコードで確認できます。

//emlist{
using UnityEngine;
using UnityEditor;

public class Example
{
    [MenuItem("Example/Random Rotate")]
    static void RandomRotate ()
    {
        var transform = Selection.activeTransform;

        if (transform) {
            Undo.willFlushUndoRecord += () => Debug.Log ("flush");

            Undo.postprocessModifications += (modifications) => {
                Debug.Log ("modifications");
                return modifications;
            };

            Undo.RecordObject (transform, "Rotate " + transform.name);
            Debug.Log ("recorded");

            transform.rotation = Random.rotation;
            Debug.Log("changed");
        }
    }
}
//}

実行すると@<img>{ss09}の順番でログが出力されます。

//image[ss09][@<code>{Example/Random Rotate}を実行した後に出力されたログ]{

//}

== Redoの仕組み

RedoはUndoで処理したものを元に戻す機能です。RedoによりUndoの実行をなかったことにします。

//image[exec_undo_redo][Cubeを作成した後のUndo実行、Redo実行の流れ]{

//}


Redoもスタックで管理されています。


//image[undo_and_redo][UndoとRedoのスタック]{

//}

Undoが実行されると、取り出されたものはRedoのスタックに積まれます。

//image[exec_undo][Undoを2回実行し、3と2がRedoスタックに積まれた状態]{

//}

Redoを実行すると、Redoスタックに格納されたものは再度Undoスタックに格納されます。

//image[exec_redo][Redoを実行し、2がRedoスタックから取り出されUndoスタックに積まれた状態]{

//}

このように、RedoはUndoで処理されたものを利用しますので特別な実装は必要ありません。

== Undoの対象


Undoの対象となるものは@<b>{UnityEngine.Object}を継承した、@<strong>{シリアライズ可能なオブジェクト}です。

=== よくUndoの実装で対象となるもの

よくUndoの実装で対象となるものは以下の3つです。
これらのオブジェクトを生成したり、オブジェクトのプロパティーの値を変更する場合はUndoの実装を行うべきと考えましょう。

 * GameObject
 * Component (MonoBehaviourも含む)
 * ScriptableObject

=== System.Serializable属性を付けたクラスをUndo対象にするには

System.Serializable属性を付けたクラスをUndo対象にする場合は、ComponentやScriptableObjectのプロパティーとしてもつことでUndo対象とさせることができます。

//emlist{
[System.Serializable]
public class PlayerInfo
{
    public string name;
    public int hp;
}
//}

例としてPlayerコンポーネントに変数として持たせます。

//emlist{
using UnityEngine;

public class Player : MonoBehaviour
{
    [SerializeField]
    PlayerInfo info;
}
//}

そしてPlayerコンポーネントをUndo対象として登録します。

//emlist{
using UnityEngine;
using UnityEditor;

public class Example
{
    [MenuItem("Example/Change PlayerInfo")]
    static void ChangePlayerInfo ()
    {
        var player = Selection.activeGameObject.GetComponent<Player> ();

        if (player) {
            Undo.RecordObject (player, "Change PlayerInfo");
            player.info = new PlayerInfo{
                name = "New PlayerName",
                hp = Random.Range(0,10)
            };
        }
    }
}
//}

== Undoの種類


Undoの種類は大きく分けて2種類になります。

 * オブジェクトのプロパティー（値）変さらに対するUndo
 * オブジェクトへのアクションに対するUndo

=== プロパティー（値）に対するUndo

==== Undo.RecordObject(s)


Undoの実装は大抵Undo.RecordObjectで済みます。まずはこのAPIを覚えましょう。


#@# lang: cs
//emlist{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript
{
    [MenuItem("Undo/RecordObject")]
    static void RecordObject ()
    {
        // 選択状態のTransformを取得する
        Transform transform = Selection.activeTransform;

        // これから変更するプロパティーのオブジェクトの指定とUndo名
        Undo.RecordObject (transform, "positionをVector3(0,0,0)に変更");
        transform.position = new Vector3 (0, 0, 0);
    }
}
//}

=== アクションに対するUndo

==== Undo.AddComponent


ゲームオブジェクトにコンポーネントを追加し、Undo対象とします。Editor拡張でコンポーネントを追加する時はこちらを使いましょう。


#@# lang: cs
//emlist{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript
{

    [MenuItem("Undo/AddComponent")]
    static void AddComponent ()
    {
        GameObject go = Selection.activeGameObject;

        Rigidbody rigidbody = Undo.AddComponent<Rigidbody> (go);

        // この後、Undo実行をすればコンポーネントが削除される
    }
}
//}

==== Undo.RegisterCreatedObjectUndo


オブジェクトを作成した時にUndoで破棄するために使用します。
よく使用するのはゲームオブジェクト作成時とScriptableObject作成時です。


#@# lang: cs
//emlist{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript
{
    [MenuItem("Undo/RegisterCreatedObjectUndo")]
    static void RegisterCreatedObjectUndo ()
    {
        GameObject go = new GameObject ();
        Undo.RegisterCreatedObjectUndo (go, "GameObjectを作成");

        // グループをインクリメント
        Undo.IncrementCurrentGroup ();

        Hoge hoge = ScriptableObject.CreateInstance<Hoge> ();
        Undo.RegisterCreatedObjectUndo (hoge, "Hogeを作成");
        // 実際にhogeがUndoされるのか確認。Undoされたらnullになる
        EditorApplication.update += () => Debug.Log (hoge);
    }
}
//}

==== Undo.DestroyObjectImmediate


RegisterCreatedObjectUndoとは逆に、破棄したものをUndoで元に戻す（再生成する）ために使用します。


#@# lang: cs
//emlist{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript
{
    [MenuItem("Undo/DestroyObjectImmediate")]
    static void DestroyObjectImmediate ()
    {
        GameObject go = Selection.activeGameObject;
        // 選択したGameObjectを破棄。Undoで破棄以前の状態に戻る
        Undo.DestroyObjectImmediate (go);
    }
}
//}

==== Undo.SetTransformParent


ゲームオブジェクトの親子関係を作成/変更するときに使用するUndoの実装です。


#@# lang: cs
//emlist{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript
{
    [MenuItem("Undo/SetTransformParent")]
    static void SetTransformParent ()
    {

        Transform root = GameObject.Find("Main Cameta").transform;

        Transform transform = Selection.activeTransform;

        Undo.SetTransformParent (transform, root, "Main Cametaオブジェクトの子要素にする");
    }
}
//}

== キャンセル処理（Revert）

Undo登録して値を変更している中で「Esc」ボタンなどで、その操作自体をキャンセルしたいときがあります。
キャンセル時はUndo登録した時の値に戻りますが、キャンセルなのでRedo（Esc押す直前の状態）が実行できてしまうのは不自然です。
なのでRedoを行わないUndoの実装をしなければなりません。

このキャンセル処理のことをRevertと言います。

//image[revert][]{

//}


==== Undo.RevertAllInCurrentGroup


現在のグループのUndoを実行します。「Revert」とあるように、戻すという処理だけなのでRedoはできません。


#@# lang: cs
//emlist{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript
{
    // 必ず宝くじが当選するメソッド
    [MenuItem("Undo/RevertAllInCurrentGroup")]
    static void RevertAllInCurrentGroup ()
    {
        GameObject ticket = new GameObject ("Ticket");

        Undo.RegisterCreatedObjectUndo (ticket, "宝くじ買いました");

        // チケットの番号はこれです
        int number = ticket.GetInstanceID ();

        // 当選番号はこれです
        int winningNumber = 1234;

        if (ticket.GetInstanceID () != winningNumber) {
          // 当たりませんでした
          // 宝くじを買ったのをなかったことにします
          Undo.RevertAllInCurrentGroup ();
        }
    }
}

//}

==== Undo.RevertAllDownToGroup


もう1つ@<strong>{Undo.RevertAllDownToGroup}があります。これは指定したグループインデックスまで戻します。

これは、連続的に値が変更しているものをRevertするときに@<strong>{RevertAllDownToGroup}を使用します。たとえば、

ゲームオブジェクトに3つのコンポーネントをつけるとしましょう。
この時、グループインデックスはそれぞれ異なるものとします。

「1つめのコンポーネントを追加した状態まで戻る」というような実装を行うには下記のコードになります。


#@# lang: cs
//emlist{
using UnityEngine;
using UnityEditor;

public class ExampleWindow : EditorWindow
{

    // ウィンドウ作成
    [MenuItem("Window/ExampleWindow")]
    static void Open ()
    {
      GetWindow<ExampleWindow> ();
    }

    GameObject go;

    int group1 = 0;
    int group2 = 0;
    int group3 = 0;

    void OnEnable ()
    {
        go = GameObject.Find ("New Game Object");
    }

    void OnGUI ()
    {
        // マウスをクリックしたら
        if (Event.current.type == EventType.MouseDown) {

            // 現在のグループインデックスを保持
            group1 = Undo.GetCurrentGroup();

            // 1つめ追加
            Undo.AddComponent<Rigidbody> (go);

            // インクリメント
            Undo.IncrementCurrentGroup();

            // 現在のグループインデックスを保持
            group2 = Undo.GetCurrentGroup();

            // 2つめ追加
            Undo.AddComponent<BoxCollider> (go);

            // インクリメント
            Undo.IncrementCurrentGroup();

            // 現在のグループインデックスを保持
            group3 = Undo.GetCurrentGroup();

            // 3つめ追加
            Undo.AddComponent<ConstantForce>(go);
        }

        if (Event.current.type == EventType.MouseUp) {
            // group2まで戻る（1つ目のみが追加されている状態）
            Undo.RevertAllDownToGroup(group2);
            // コンポーネントのGUIが変更されたことによる
            // 描画エラーを回避するためにExitGUIを呼び出す
            EditorGUIUtility.ExitGUI();
        }
    }
}
//}

== グループを理解する


Undoにはグループという概念が存在します。

例えば、下記のコード


#@# lang: cs
//emlist{
GameObject enemy = new GameObject ("Enemy");
Undo.RegisterCreatedObjectUndo (enemy, "Enemyを作成");

GameObject effect = new GameObject ("Effect");
Undo.RegisterCreatedObjectUndo (effect, "Effectを作成");
//}


のように書き、複数のUndo対象を登録して、これを実際にUndo実行するとどうなるでしょうか。


=== まとめてUndoする


先ほどのコードでUndo実行をすると、2つのオブジェクトのUndoが同時に実行されます。



//indepimage[img01]


これは PropertyDiffUndoRecorder が Flush されるまで、1つのグループとしてまとめられるからです。
Undo 時には 1グループまとめて処理が走ります。



この挙動で問題ない場合もあるかもしれませんが、「それぞれのUndoを個別に実行したい」という時もあります。

=== それぞれのUndoを個別に実行する


1つのグループになっているため、まとめてUndoされるわけなので、これを2つのグループに分ければ問題ありません。
そのためのAPIが用意されています。

==== Undo.IncrementCurrentGroup


グループはint型でindexとして管理されているので、それをインクリメント(+1)してグループを分けます。
そうすると、それぞれのものは別のグループとして認識され個別にUndoできるようになります。


#@# lang: cs
//emlist{
GameObject enemy = new GameObject ("Enemy");
Undo.RegisterCreatedObjectUndo (enemy, "Enemyを作成");

Undo.IncrementCurrentGroup();

GameObject effect = new GameObject ("Effect");
Undo.RegisterCreatedObjectUndo (effect, "Effectを作成");
//}


//indepimage[img02]

=== 個別だった Undo を1つにまとめる

最初は個別で独立した Undo 処理でしたが、最後には Undo を1つにまとめてしまう事が可能です。

カラーピッカーでこの例を見ることができます。

カラーピッカーの表示中は、各RGBA成分に対しての Undo が適用されます。しかし、カラーピッカーで色を決定した後は、カラーピッカーを開く前の色へと Undo しなくてはいけません。これはからピッカーを閉じた時に、各RGBA成分で分かれていた Undo を 1 つにまとめることで実現が可能になります。

//image[ss12][各成分に対する Undo と、色に対する Undo の違い]{

//}

==== CollapseUndoOperations

Undo を一つにまとめる方法は CollapseUndoOperations を使用します。

CollapseUndoOperations は指定した group から今までのグループを全て1つにまとめる機能です。


下記コードは、Cube/Plane/Cylinder を作成するコードです。各ゲームオブジェクトの生成に対して Undo が行われます。また、EditorWindow を閉じた時には Undo を実行した時に Cube/Plane/Cylinder が全て削除されます。

下記コードのように @<code>{OnEnable} 内でグループIDを保持しておき、@<code>{OnDisable} で今までの Undo を1つにまとめます。


//emlist{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript : EditorWindow
{
    int groupID = 0;

    [MenuItem ("Window/Example")]
    static void Open ()
    {
        GetWindow<NewBehaviourScript> ();
    }

    void OnEnable ()
    {
        groupID = Undo.GetCurrentGroup ();
    }

    void OnDisable ()
    {
        Undo.CollapseUndoOperations (groupID);
    }

    void OnGUI ()
    {
        if (GUILayout.Button ("Cube 作成")) {
            var cube = GameObject.CreatePrimitive (PrimitiveType.Cube);
            Undo.RegisterCreatedObjectUndo (cube, "Create Cube");
        }

        if (GUILayout.Button ("Plane 作成")) {
            var plane = GameObject.CreatePrimitive (PrimitiveType.Plane);
            Undo.RegisterCreatedObjectUndo (plane, "Create Plane");
        }

        if (GUILayout.Button ("Cylinder 作成")) {
            var cylinder = GameObject.CreatePrimitive (PrimitiveType.Cylinder);
            Undo.RegisterCreatedObjectUndo (cylinder, "Create Cylinder");
        }
    }
}
//}