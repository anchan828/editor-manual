
= Undoについて


Unity4.3よりUnityEditor.UndoのAPIが一新されました。



格段と使いやすくなったので1つ1つ紹介していきます。


== Undoの対象


Undoの対象となるものは@<b>{UnityEngine.Object}を継承した、@<strong>{シリアライズ可能なオブジェクト}です。
グループという概念があるのですが難しいので一番最後に書きました。


=== よくUndo実装で対象となるもの


Undo実装で対象となるものは以下の3つです。
これらのオブジェクトを生成したり、オブジェクトのプロパティの値を変更する場合はUndoの実装を行うべきと考えましょう。

 * GameObject
 * Component (MonoBehaviourも含む)
 * ScriptableObject


System.Serializable属性を付けたクラスをUndo対象にする場合は、ComponentやScriptableObjectのプロパティとして保持させることでUndo対象とさせることができます。


== Undoの種類


Undoの種類は大きく分けて2種類になります。

 * オブジェクトのプロパティ（値）に対するUndo
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


ゲームオブジェクトの親子関係を作成するときのUndoを実装します。


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

== キャンセル処理


Undo登録して値を変更してたけど「Esc」ボタンなどで、その操作自体をキャンセルするときがあります。
キャンセル時、Undo登録した時の値に戻りますが、キャンセルなのでRedo（Esc押す直前の状態）ができてしまうのは不自然です。

なのでRedoは行わないUndo実行をしなければいけません。


==== Undo.RevertAllInCurrentGroup


現在のグループのUndoを実行します。「Revert」とあるように、戻すという処理だけなのでRedoはできません。


#@# lang: cs
//emlist{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript
{
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
