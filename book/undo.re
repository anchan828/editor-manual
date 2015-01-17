
= Undoについて


//lead{
エディタ拡張によって変更した状態は「元に戻す」処理を自分で実装しなければいけません。そこでこの章では簡単なサンプルを元にUndoの使い方をマスターしていきます。
//}


== Undoの操作を実感してみる

まずはどのような操作がUndoなのか実感してみましょう。

最初にCubeを作成します。

//image[ss01][@<code>{Assets/Create/3D Object/Cube}で作成した状態]{

//}

次に@<code>{Edit/Undo Create Cube}を実行します。ショートカットキーで実行する場合は「command/ctrl + Z」です。

//image[ss02][メニュー名が@<code>{Undo Create Cube}ではない場合、余計な操作を行っている可能性があります。もう一度Cubeを生成しなおしてみましょう。]{
//}

生成されたCubeが削除されましたか？Cubeを生成する前に戻ったことになります。これが「元に戻る = Undo」という操作です。


== Undoの仕組み

Undoの管理はスタックで行われています。@<fn>{LIFO}

//footnote[LIFO][スタックは、後から入れたものを先に出す「LIFO（後入先出）」です。]

//image[stack][スタックをイメージするとこんな感じ]{

//}

//image[ss05][]{

//}


== Undoを実装してみる

Undoの操作を実感してみたところで、次はUndoを実装してみましょう。
 
//quote{
前回のUndo履歴をリセットするために@<code>{File/New Scene}で新規シーンにしておきましょう。
//}

=== オブジェクトの作成に対するUndo

下記コードはCubeを生成するためのコードです。@<code>{Example/Create Cube}を実行することによってCubeを生成することが出来ます。

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

Cubeを生成してもUndoを行うことは出来ません。これはUndoの実装がおこなれていないためです。

//image[ss03][Undoという文字が灰色になって選択できないことがわかる]{

//}

さっそく@<b>{Undoクラス}を使って実装します。

今回は@<code>{Undo.RegisterCreatedObjectUndo}関数を使用します。この関数はオブジェクトが生成された時に使用するUndoで、この関数でUndo登録されたオブジェクトは、Undo実行時に破棄されます。

//image[RegisterCreatedObjectUndo][RegisterCreatedObjectUndo関数とUndoを実行した時の動き]{

//}

@<img>{RegisterCreatedObjectUndo}では、RegisterCreatedObjectUndo関数を実行すると「Cubeを作成する」という処理を差分としてスタックに追加します@<fn>{f1}。この差分がUndoで使用される際には「Cubeを作成する前に戻す」つまり、Cubeを削除するという処理を行うことになります。

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

Undoを実行した後はUndoしたものを元に戻すRedo（取り消す）も実行することが出来ます。

//indepimage[ss06]

=== プロパティの変更に対するUndo

下記コードはオブジェクトの回転をランダムに設定するコードです。@<code>{Example/Random Rotate}を実行することによって選択しているオブジェクトをランダムに回転させることが出来ます。

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

ランダムな回転が設定される前に@<code>{Undo.RecordObject}関数を実行します。これにより「変更前」のTransformのプロパティをUndoスタックに保存することが出来るようになります。

//image[ss11][Undoが登録されているとメニューに表示され正しく回転を]{

//}

== どのプロパティが変更されたかを知るPropertyDiffUndoRecorder

Undoのスタックに保存されるのは「値が変更されたプロパティの、@<b>{変更前の値} 」です。

なので「どのプロパティが変更されたか」を知る必要があります。その役割を担うのがPropertyDiffUndoRecorderです。

//image[ss07][PropertyDiffUndoRecorderはプロファイラで確認することが出来る]{

//}

PropertyDiffUndoRecorderは、RecordObjectで登録されたオブジェクトを、Unityエディタのライフサイクルの最後あたりでUndoのFlushを呼びだすことで、Undo.RecordObjectが呼び出された時のオブジェクトの各プロパティと、Flushが呼び出された時のオブジェクトの各プロパティを使用して差分を求めます。

以下の順に実行され、図にしたものが@<img>{PropertyDiffUndoRecorder}です。

 1. オブジェクトの登録
 2. 値の変更
 3. Flush実行
 4. 差分の出力

//image[PropertyDiffUndoRecorder][PropertyDiffUndoRecorderのサイクル]{

//}

//image[ss08][差分のイメージ（画像は@<href>{https://gist.github.com,gist}のdiffビュー）]{

//}

@<img>{PropertyDiffUndoRecorder}で示したサイクルを確認してみましょう。以下のコードで確認することが出来ます。

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
これらのオブジェクトを生成したり、オブジェクトのプロパティの値を変更する場合はUndoの実装を行うべきと考えましょう。

 * GameObject
 * Component (MonoBehaviourも含む)
 * ScriptableObject

=== System.Serializable属性を付けたクラスをUndo対象にするには

System.Serializable属性を付けたクラスをUndo対象にする場合は、ComponentやScriptableObjectのプロパティとしてもつことでUndo対象とさせることができます。

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

 * オブジェクトのプロパティ（値）変更に対するUndo
 * オブジェクトへのアクションに対するUndo

=== プロパティ（値）に対するUndo

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

        // これから変更するプロパティのオブジェクトの指定とUndo名
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
実際にUndoを実装する時、UndoGroup（UndoGroupってのは私が勝手につけた）を実行していることになります。



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




この挙動で問題ない場合もあるかもしれませんが、「それぞれのUndoを個別に実行したい」という時もあると思います。


=== まず、Groupってどのタイミングで作られるの？


まず、エディタにもランタイムでいうAwake/Start/Updateのようなライフサイクルがあります。
その1サイクル分が1つのグループとしてまとめられます。



といっても、大抵は「OnGUIなどUnityが用意しているメソッド内で実行されるものが1グループにまとまる」って言った方がわかりやすいし、これも正しい。


=== それぞれのUndoを個別に実行する


1つのグループになっているからまとめてUndoされるわけなので、これを2つのグループに分けちゃえばいいのです。
そのためのAPIが用意されています。


==== Undo.IncrementCurrentGroup


グループはint型でindexとして管理されているので、それをインクリメント(+1する)してグループを分けます。
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
