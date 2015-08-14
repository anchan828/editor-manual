= Gizmo（ギズモ）

ギズモはシーンビューで編集やデバッグのために、オブジェクトの位置や範囲、特定の要素を可視化するための仕組みです。

例えば、プリミティブの Cube を作成したとします。

//image[ss01][作成した Cube を選択した状態でシーンビューで見た時の表示]{

//}

少しわかりづらいですが、@<img>{ss01}の Cube の辺には緑色の線、または青色の線が表示されています。この@<b>{見える線}がギズモです。このギズモがあることによって、「このゲームオブジェクトにはコライダーがアタッチされている」「コライダーの大きさはこれくらいである」のような情報をもたらしてくれます。

== ゲームオブジェクトにギズモを追加

また、ギズモは@<b>{アイコン}も表示できます。これは標準機能として実装されており、インスペクターにあるゲームオブジェクトのアイコンをクリックすることでアイコン選択画面が表示されます。

//image[ss05][インスペクターの左上にあるアイコンをクリック]{

//}

デフォルトで3つの形状とさまざまな色が用意されています。

//indepimage[ss07]

また、好きなテクスチャをアイコンとして指定することも可能です。

//image[ss06][テクスチャは透過しているものが好ましい]{

//}

アイコンを付けることで、「どこにオブジェクトがあるかわかりやすくなる」「描画するものがなく@<b>{シーンビューで選択できなかったオブジェクトを選択できるようになる}」メリットがあります。

//image[ss08][GameManagerは普段ギズモが表示されずシーンビューで選択ができなかったがアイコンをクリックすることで選択できるように][scale=0.5]{

//}

== ギズモの描画

「Box Collider がアタッチされているかのような状態」をギズモで表現してみましょう。

空のゲームオブジェクトを作成し、作成したスクリプトをアタッチします。

//indepimage[ss02]

=== OnDrawGizmos と OnDrawGizmosSelected

最も簡単にギズモを表示する方法として、2つのメソッドが用意されています。そのメソッドは MonoBehaviour の派生クラス内でUpdate 関数のようなコールバックとして使用できます。@<b>{ゲームオブジェクトまたは親を選択中の時のみ表示する} @<code>{OnDrawGizmosSelected}、@<b>{ゲームオブジェクトがアクティブの時に常に表示する} @<code>{OnDrawGizmos} の 2 つがあります。


//image[ss03][OnDrawGizmosSelected を使ってオブジェクトを選択中にギズモを表示]{

//}

//image[ss04][OnDrawGizmosSelected では子要素のギズモもまとめて表示]{

//}

//emlist{
using UnityEngine;

public class Example : MonoBehaviour
{
    void OnDrawGizmosSelected ()
    {
        // ギズモの色を変更
        Gizmos.color = new Color32 (145, 244, 139, 210);
        Gizmos.DrawWireCube (transform.position, transform.lossyScale);
    }
}
//}

=== DrawGizmoAttribute

手軽に使用できる OnDrawGizmos と OnDrawGizmosSelected を紹介しましたが、もともとギズモはエディター側の機能です。ランタイムのスクリプトにエディターのコードを書くのは気持ちの良いものではないかもしれません。@<fn>{1}

ギズモを描画する方法として @<code>{DrawGizmoAttribute} があります。この属性は、@<b>{特定のコンポーネント}に対してギズモを適用するためのものです。つまり、OnDrawGizmos や OnDrawGizmosSelected と同じ機能を扱えるということです。

OnDrawGizmos と同じ機能を DrawGizmoAttribute で実装すると以下のコードになります。 @<code>{GizmoType.NonSelected} のみだと、オブジェクトを選択した時に表示されなくなってしまうので @<code>{GizmoType.Active}@<fn>{2} も追加し、選択状態でもギズモを表示するようにします。

//emlist{
using UnityEngine;
using UnityEditor;

public class EditorScript
{
    [DrawGizmo (GizmoType.NonSelected | GizmoType.Active)]
    static void DrawExampleGizmos (Example example, GizmoType gizmoType)
    {
        var transform = example.transform;
        Gizmos.color = new Color32 (145, 244, 139, 210);

        // GizmoType.Active の時は赤色にする
        if ((gizmoType & GizmoType.Active) == GizmoType.Active)
            Gizmos.color = Color.red;

        Gizmos.DrawWireCube (transform.position, transform.lossyScale);
    }
}
//}

//image[ss09][子要素である中の小さな Cube を選択した状態]{

//}

OnDrawGizmosSelected を表現する場合は @<code>{GizmoType.InSelectionHierarchy} で可能です。

//emlist{
[DrawGizmo (GizmoType.InSelectionHierarchy)]
static void DrawExampleGizmos (Example example, GizmoType gizmoType)
{
    // 略
}
//}

//image[ss10][子要素である中の小さな Cube を選択した状態]{
//}

ギズモは単純な機能ですがアイデア次第で、シーン内をより見やすく、理解しやすくすることが可能です。

//image[ss11][Cubeに上部を知らせる半球と軸を表示][scale=0.5]{

//}

#@# * ギズモの表示の仕方
#@# * ギズモの表示の仕方はさまざまある
#@# ** OnDrawGizmos
#@# ** OnDrawGizmosSelected
#@# ** DrawGizmoAttribute
#@# * DrawGizmoAttribute
#@# ** BoxColliderでギズモ
#@# ** スクリプト貼り付けてギズモ
#@# * アイコンの表示
#@# * Gizmosフォルダーについて

//footnote[1][#if UNITY_EDITOR で囲めばコンパイルは問題ありません]
//footnote[2][「ギズモが」アクティブと判断されるのは「ゲームオブジェクトが」アクティブかつゲームオブジェクトを選択した時]