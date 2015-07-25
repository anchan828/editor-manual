= Handle（ハンドル）

@<chapref>{gizmo}にて、オブジェクトの位置や範囲、特定の要素を可視化するための仕組みを紹介しました。本章では、更に込み入った可視化、それだけではなく「@<b>{操作}」を行う @<b>{Handles} について紹介していきます。

@<b>{ハンドル} は、オブジェクトに対してアクションを起こすための仕組みです。例えば @<img>{ss01} の表示は全てハンドルです。

//image[ss01][これらは全てマウスで操作することが出来る]{

//}


== ゲームオブジェクトにハンドルを追加

ゲームオブジェクトに独自のハンドルを追加してみましょう。ハンドルの実装は通常 @<b>{CustomEditor} の @<code>{OnSceneGUI} 関数の中で行います。


//emlist{
using UnityEngine;

public class Example : MonoBehaviour {

}
//}

まずは MonoBehaviour の派生クラスを作成し、そのクラスに対するカスタムエディタを作成します。標準で実装されているハンドルを表示しないように @<code>{Tools.current = Tool.None;} としておきます。これでゲームオブジェクトの位置を変更するためのハンドルが作成できました。

//emlist{
using UnityEngine;
using UnityEditor;

[CustomEditor (typeof(Example))]
public class ExampleInspector : Editor
{
	void OnSceneGUI ()
	{
		Tools.current = Tool.None;
		var component = target as Example;

		
		var transform = component.transform;
		transform.position = Handles.PositionHandle (transform.position, transform.rotation);
	}
}
//}

=== 位置と回転のハンドルを同時に表示する

ハンドルを同時に複数表示することも可能です。

//image[ss02][位置と回転のハンドルを同時表示]{

//}

//emlist{
void OnSceneGUI ()
{
	var component = target as Example;
	
	var transform = component.transform;
	
	if (Tools.current == Tool.Move)
		transform.rotation = Handles.RotationHandle (transform.rotation, transform.position);
	else if (Tools.current == Tool.Rotate)
		transform.position = Handles.PositionHandle (transform.position, transform.rotation);
}
//}

== PositionHandle を自作する

ハンドルは様々なパーツを組み合わせて作成されています。基本的なものでも8種類のパーツが用意されています。

//image[ss05][ハンドルで使えるパーツは様々]{

//}

これらのパーツを使って、PositionHandle と同じものを作成してみます。

=== 矢印のハンドル

矢印のハンドルは、@<code>{Handles.Slider} を使用します。位置を「スライド」させるのに適しているので使用されています。

//emlist{
void OnSceneGUI ()
{
	Tools.current = Tool.None;
	var component = target as Example;
	PositionHandle (component.transform);
}

void PositionHandle (Transform transform)
{
	Handles.Slider (transform.position, transform.right); // X 軸
	Handles.Slider (transform.position, transform.up); // Y 軸
	Handles.Slider (transform.position, transform.forward); // Z 軸
}
//}

これで、３軸の矢印のハンドルを表示することが出来ました。ですが、まだ色がついていません。

//indepimage[ss06]

==== ハンドルに色を付ける

色をつけるには、ハンドルを描画する「前」に @<b>{Handles.color} に値を代入します。各軸の色は既に用意されているものを使いましょう。

//emlist{
void PositionHandle (Transform transform)
{
	Handles.color = Handles.xAxisColor;
	Handles.Slider (transform.position, transform.right); // X 軸

	Handles.color = Handles.yAxisColor;
	Handles.Slider (transform.position, transform.up); // Y 軸

	Handles.color = Handles.zAxisColor;
	Handles.Slider (transform.position, transform.forward); // Z 軸
}	
//}

//indepimage[ss07]


==== ハンドルで位置を移動

Sliderで移動させた結果をオブジェクトへ反映しましょう。

//emlist{
void OnSceneGUI ()
{
	Tools.current = Tool.None;
	var component = target as Example;
	var transform = component.transform;

	transform.position = PositionHandle (transform);
}

Vector3 PositionHandle (Transform transform)
{
	var position = transform.position;

	Handles.color = Handles.xAxisColor;
	position = Handles.Slider (position, transform.right); // X 軸

	Handles.color = Handles.yAxisColor;
	position = Handles.Slider (position, transform.up); // Y 軸

	Handles.color = Handles.zAxisColor;
	position = Handles.Slider (position, transform.forward); // Z 軸

	return position;
}
//}


=== ハンドルの大きさを固定する

既に実装されている PositionHandle は、シーンビューをズームイン・ズームアウトしてもハンドルの大きさは変わりません。これは内部で、「シーンビューのカメラ」と「ハンドル」の位置を使って、ハンドルのサイズが計算されています。これにより、いくらズームイン・ズームアウトをしても適切なサイズに拡大縮小され、@<b>{見た目の長さ}が同じになります。

//image[ss03][ズームイン・ズームアウトでキューブの大きさが変わってもハンドルの大きさは変わっていない]{

//}

常にハンドルの大きさを固定したいときは、@<b>{Handles.Slider} で大きさを指定しなければいけません。

下記コードは、引数で snap の値も必要なので snap 設定のコードも追加されています。

//emlist{
Vector3 snap;

void OnEnable ()
{
    // SnapSettings の値を取得する
	var snapX = EditorPrefs.GetFloat ("MoveSnapX", 1f);
	var snapY = EditorPrefs.GetFloat ("MoveSnapY", 1f);
	var snapZ = EditorPrefs.GetFloat ("MoveSnapZ", 1f);
	snap = new Vector3 (snapX, snapY, snapZ);
}

Vector3 PositionHandle (Transform transform)
{
	var position = transform.position;

	var size = 1; 

	// X 軸
	Handles.color = Handles.xAxisColor;
	position = Handles.Slider (position, transform.right, size, Handles.ArrowCap, snap.x); 

	// Y 軸
	Handles.color = Handles.yAxisColor;
	position = Handles.Slider (position, transform.up, size, Handles.ArrowCap, snap.y); 

	// Z 軸
	Handles.color = Handles.zAxisColor;
	position = Handles.Slider (position, transform.forward, size, Handles.ArrowCap, snap.z); 

	return position;
}
//}


//image[ss04][ズームアウトするにつれハンドルは小さくなっていく。ハンドルの大きさが固定]{

//}

逆に、ハンドルの@<b>{見た目の大きさ}を同じにするには @<code>{HandleUtility.GetHandleSize} を使用して大きさを求めます。

//emlist{
var size = HandleUtility.GetHandleSize (position);
//}

== 特殊なハンドル

Handles クラスには、特殊なハンドルを作成できる API がいくつか存在します。

=== FreeMoveHandle

3軸を意識せずに自由に動かせるハンドルを作成します。表示するパーツは RectangleCap が好ましいです。

//emlist{
transform.position = 
    Handles.FreeMoveHandle (
        transform.position,
        transform.rotation,
        size,
        snap, Handles.RectangleCap);
//}

//image[ss08][四角の中でドラッグすると動かすことが出来る]{

//}

これは、シーンビューの向きを 2D モードのような 2 軸に表示した時に便利です。こうすることで 3D オブジェクトでも X と Y 軸の位置をまとめて調整することが可能です。

//image[ss09][シーンビューの右上のハンドルで2軸のモードにする]{

//}


=== FreeRotateHandle

3軸を意識せずに自由に回転できるハンドルを作成します。表示するパーツは円で固定です。

//emlist{
transform.rotation = 
    Handles.FreeRotateHandle (
        transform.rotation,
        transform.position,
        size);
//}


//image[ss10][どの角度から見ても円のハンドルが同じ形で表示されている]{

//}


=== DrawAAPolyLine

各頂点（Vector3配列）を線で結び、表示します。

MonoBehaviour の派生クラスに Vector3 の配列を持たせ、

//emlist{
public class Example2 : MonoBehaviour {

	public Vector3[] vertexes;
}
//}

CustomEditor 側で使用します。

//emlist{
void OnSceneGUI ()
{
	Handles.DrawAAPolyLine (component.vertexes);
}
//}

//image[ss11][要素順に線が引かれる]{

//}

複雑な線が引けるようになったのは良いのですが、値の設定が@<img>{ss12}のように扱いにくいので2つ実装を行います。

//image[ss12][値を手入力しなければいけない、また線を引く順番を変更する時に面倒]{

//}

==== PositionHandle

まずは Vector3 の値をシーンビュー上で編集できるようにハンドルを追加します。

//emlist{
void OnSceneGUI ()
{
	var vertexes = component.vertexes;

	for (int i = 0; i < vertexes.Length; i++) {

		vertexes [i] = Handles.PositionHandle (vertexes [i], Quaternion.identity);
	}

	Handles.DrawAAPolyLine (vertexes);
}
//}

ハンドルが大きいと感じる場合は、本章で自作した PositionHandle を使用してみましょう。

//image[ss13][各点に PositionHandle が表示された]{

//}


==== ReorderableList

インスペクターでの表示を、@<b>{ReorderableList} で行います。ReorderableList を使用することで、要素の追加と並び替えがとても楽になります。詳しくは @<chapref>{reorderblelist}をご覧ください。

//indepimage[ss14]

//emlist{
using UnityEngine;
using UnityEditor;
using UnityEditorInternal;

[CustomEditor (typeof(Example2))]
public class ExampleInspector2 : Editor
{
	ReorderableList reorderableList;
	Example2 component;

	void OnEnable ()
	{
		Tools.current = Tool.None;
		component = target as Example2;
		reorderableList = new ReorderableList (component.vertexes, typeof(Vector3));

		reorderableList.drawElementCallback = (rect, index, isActive, isFocused) => {
			component.vertexes [index] = EditorGUI.Vector3Field (rect, GUIContent.none, component.vertexes [index]);
		};

		reorderableList.onAddCallback = (list) => {
			ArrayUtility.Add (ref component.vertexes, Vector3.zero);
			ActiveEditorTracker.sharedTracker.ForceRebuild ();
		};
		reorderableList.onRemoveCallback = (list) => {
			ArrayUtility.Remove (ref component.vertexes, component.vertexes [list.index]);
			ActiveEditorTracker.sharedTracker.ForceRebuild ();
		};
		reorderableList.onChangedCallback = (list) => SceneView.RepaintAll ();
	}

	public override void OnInspectorGUI ()
	{
		reorderableList.DoLayoutList ();
	}
}
//}


== GUI の描画（2D）

//image[ss15][ナビゲーションウィンドウを開いている時に右下に GUI が表示されている]{

//}

ハンドルは 3D 空間に特化した可視化ですが、2D 空間に対しての描画も出来ます。

=== 3D 空間と 2D 空間

この2つの描画する仕組みは別になっているため少しだけ気をつけなければいけない部分があります。2D 向けの GUI を描画するには @<code>{Handles.BeginGUI} と @<code>{Handles.EndGUI} を呼びださなければいけません。


//emlist{
void OnSceneGUI ()
{
	Handles.BeginGUI ();

	GUILayout.Button ("Button", GUILayout.Width (50));

	Handles.EndGUI ();
}
//}
//image[ss16][左上にボタンが表示された]{

//}
===[column] Handles.BeginGUI を GUI.Scope で表現する

GUI.Scope の機能を使って、 Handles.BeginGUI と EndGUI の記述を省くことで、いくらかコードの可読性が上がるかもしれません。

まずは HandleGUIScope を作成します。

//emlist{
public class HandleGUIScope : GUI.Scope
{
	public HandleGUIScope ()
	{
		Handles.BeginGUI ();
	}

	protected override void CloseScope ()
	{
		Handles.EndGUI ();
	}
}
//}

あとは以下のように使用するだけです。

//emlist{
void OnSceneGUI ()
{
	using (new HandleGUIScope ()) {
		GUILayout.Button ("Button", GUILayout.Width (50));
	}
}
//}

===[/column]


=== ウィンドウの表示

GUI クラスの使い方は、インスペクターや他のウィンドウでの使い方と変わりません。試しにウィンドウを表示してみましょう。

//image[ss17][左上に GUI のウィンドウが表示される]{

//}

//emlist{
int windowID = 1234;
Rect windowRect;

void OnSceneGUI ()
{
	Handles.BeginGUI ();

	windowRect = GUILayout.Window (windowID, windowRect, (id) => {

		EditorGUILayout.LabelField ("Label");

		EditorGUILayout.ToggleLeft ("Toggle", false);

		GUILayout.Button ("Button");

		GUI.DragWindow();
		
	}, "Window", GUILayout.Width (100));

	Handles.EndGUI ();
}
//}
