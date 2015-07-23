= Gizmo（ギズモ）

//indepimage[frontispiece]
//pagebreak


ギズモはシーンビューで編集やデバッグのために、オブジェクトの位置や範囲、特定の要素を可視化するための仕組みです。

例えば、プリミティブのキューブを作成したとします。

//image[ss01][作成したキューブを選択した状態でシーンビューで見た時の表示]{

//}

少しわかりづらいですが、@<img>{ss01}のキューブの辺には緑色の線、または青色の線が表示されています。この@<b>{見える線}がギズモです。このギズモがあることによって、「このゲームオブジェクトにはコライダーがアタッチされている」「コライダーの範囲がこれくらいである」のような情報をもたらしてくれます。

== ゲームオブジェクトにギズモを追加

また、ギズモは@<b>{アイコン}も表示することが出来ます。これは標準機能として実装されており、インスペクターにあるゲームオブジェクトのアイコンをクリックすることでアイコン選択画面が表示されます。

//image[ss05][インスペクターの左上にあるアイコンをクリック]{

//}

デフォルトで3つの形状と様々な色が用意されています。

//indepimage[ss07]

また、好きなテクスチャをアイコンとして指定することも可能です。

//image[ss06][テクスチャは透過しているものが好ましい]{

//}

アイコンを付けることで、「どこにオブジェクトがあるかわかりやすく」「@<b>{シーンビューで選択できなかったオブジェクトを選択できるように}」なります。

//image[ss08][GameManagerは普段ギズモが表示されずシーンビューで選択ができなかったがアイコンをクリックすることで選択できるように]{

//}

== ギズモの描画

「Box Collider がアタッチされているかのような状態」をギズモで表現してみましょう。

空のゲームオブジェクトを作成し、作成したスクリプトをアタッチします。

//indepimage[ss02]

=== OnDrawGizmos と OnDrawGizmosSelected

最も簡単にギズモを表示する方法として、2つのメソッドが用意されています。そのメソッドは MonoBehaviour の派生クラスで、 Update 関数のようなコールバックとして使用することが出来ます。@<b>{ゲームオブジェクトまたは親を選択中の時のみ表示する} @<code>{OnDrawGizmosSelected}、@<b>{ゲームオブジェクトがアクティブの時に常に表示する} @<code>{OnDrawGizmos} というように 2 つは動作が異なります。


//image[ss03][OnDrawGizmosSelected を使ってオブジェクトを選択中にギズモを表示]{

//}

//image[ss04][子要素のギズモもまとめて表示]{

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

手軽に使用できる OnDrawGizmos と OnDrawGizmosSelected を紹介しましたが、元々ギズモはエディタ側の機能です。ランタイムのスクリプトにエディタのコードを書くのは気持ちの良いものではないかもしれません。@<fn>{1}

ギズモを描画する方法として @<code>{DrawGizmoAttribute} が提供されています。この属性は、@<b>{特定のコンポーネント}に対してギズモを適用するためのものです。つまり、OnDrawGizmos と同じ機能を扱えるということです。

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

//image[ss09][中の小さなキューブを選択した状態]{

//}

OnDrawGizmosSelected を表現する場合は @<code>{GizmoType.InSelectionHierarchy} で可能です。


#@# * ギズモの表示の仕方
#@# * ギズモの表示の仕方は様々ある
#@# ** OnDrawGizmos
#@# ** OnDrawGizmosSelected
#@# ** DrawGizmoAttribute
#@# * DrawGizmoAttribute
#@# ** BoxColliderでギズモ
#@# ** スクリプト貼り付けてギズモ
#@# * アイコンの表示
#@# * Gizmosフォルダについて

//footnote[1][#if UNITY_EDITOR で囲めばコンパイルは問題ありません]
//footnote[2][「ギズモが」アクティブと判断されるのは「ゲームオブジェクトが」アクティブかつゲームオブジェクトを選択した時]