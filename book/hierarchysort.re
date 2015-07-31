= HierarchySort

//indepimage[frontispiece]


Hierarchyウインドウでは、ゲームオブジェクトの順番を任意に並び替えることが可能です。デフォルトでは uGUI の描画順を決定する @<b>{Transform Sort} となります。また、Preferencesウインドウにある「Enable Alpha Numeric Sorting」を有効にすると、ゲームオブジェクトをアルファベット順に並び替える @<b>{Alphabetical Sort} を選択できるようになります。

//image[ss01][Alphabetical Sort は、Unity4.5までデフォルトだったソート方法]{

//}

ただ、ここで1つ抑えてほしいことは あくまで Hierarchy の表示順をかえるということであり GameObject.Find などのゲームオブジェクトの取得順は変更されていない、ということに注意してください。

さらに、HierarchySortを自分で作成する方法が用意されています。

== HierarchySortを自作する

公式のスクリプトリファレンスで@<href>{http://docs.unity3d.com/ja/current/ScriptReference/BaseHierarchySort.html,サンプル}として、Alphabetical Sort を作成する方法が掲載されています。

HierarchySortを自作するには、@<b>{BaseHierarchySort}を継承したクラスを用意して@<code>{Compare}メソッドをオーバーライドします。

//emlist[][cs]{
public class AlphaNumericSort : BaseHierarchySort
{
	public override int Compare(GameObject lhs, GameObject rhs)
	{
		if (lhs == rhs) return 0;
		if (lhs == null) return -1;
		if (rhs == null) return 1;

		return EditorUtility.NaturalCompare(lhs.name, rhs.name);
	}
}
//}

== TagSort を作成する

今回は ゲームオブジェクトのタグ名で並び替えるHierarchySortを作成してみましょう。

=== BaseHierarchySort を継承したクラスを作成する

先ほど AlphaNumericSort のサンプルで見たように BaseHierarchySort クラスを継承した TagSort クラスを作成します。

//emlist[][cs]{
using UnityEngine;
using UnityEditor;
public class TagSort : BaseHierarchySort
{
}
//}

=== Compare をオーバーライドして比較を行う

次に、ゲームオブジェクトの@<href>{http://docs.unity3d.com/jp/current/ScriptReference/GameObject-tag.html,タグ}で比較するように実装します。

//emlist[][cs]{
using UnityEngine;
using UnityEditor;
public class TagSort : BaseHierarchySort
{
  public override int Compare (GameObject lhs, GameObject rhs)
  {
    if (lhs == rhs) return 0;
    if (lhs == null) return -1;
    if (rhs == null) return 1;
    return EditorUtility.NaturalCompare (lhs.tag, rhs.tag);
  }
}
//}

//image[ss02][EditorOnlyとMainCameraのタグが設定されており、タグ名のアルファベット順に並んでいることが分かる]{

//}

これ自体で Tag を使ってのソートが出来るようになりました。インスペクターを見てみると確かに並び替わっていることがわかりますが、GameObject がどの Tag を持っているのかHierarchyウインドウを見た限りではわかりません。

=== Hierarchyのゲームオブジェクト名の横にタグ名を表示する


そこで@<href>{http://docs.unity3d.com/ScriptReference/EditorApplication-hierarchyWindowItemOnGUI.html,EditorApplication.hierarchyWindowItemOnGUI}を使用してタグ名を表示したいと思います。


//emlist[][cs]{
using UnityEngine;
using UnityEditor;

[InitializeOnLoad]
public class TagSort : BaseHierarchySort
{
  static TagSort()
  {
    EditorApplication.hierarchyWindowItemOnGUI += (instanceID, selectionRect) =>
    {
      var go = EditorUtility.InstanceIDToObject(instanceID) as GameObject;
      selectionRect.x += selectionRect.width - 64;
      selectionRect.width = 64;
      style.fontSize = 8;
      EditorGUI.LabelField(selectionRect, go.tag, Style.miniBox);
    };
  }

  public override int Compare(GameObject lhs, GameObject rhs)
  {
    if (lhs == rhs)
      return 0;
    if (lhs == null)
      return -1;
    if (rhs == null)
      return 1;
    return EditorUtility.NaturalCompare(lhs.tag, rhs.tag);
  }

  private class Style
  {
    public static GUIStyle miniBox;

    static Style ()
    {
      miniBox = new GUIStyle ("box");
      miniBox.fontSize = 8;
    }
  }
}
//}


表示を行うだけなら上記の書き方でいいと思うかもしれません。ですがこの書き方は「@<strong>{TagSortを選択していなくてもTag名が表示されてしまう}」という状況によっては「厄介だ」と思う方もいると思います。


== 現在何でソートされているかを知る


なので今回は「TagSortを選択している時だけタグ名を表示する」という手法を取りたいと思います。
現在何でソートされているかを知るには、APIとして正式に提供されていないため少し面倒くさいことをしなければいけません。
面倒くさいことをするためにまず、Hierarchyウインドウの@<tt>{Type}を取得しなければいけません。
その際にはUnityEngine.Types.GetTypeを使用すると便利です。Types.GetTypeはAssemblyから特定のTypeを取得するためのAPIとなっています。


//emlist[][cs]{
Type hierarcyType = Types.GetType("UnityEditor.SceneHierarchyWindow", "UnityEditor.dll");
//}


次に、先ほど取得した@<tt>{hierarcyType}を使用してHierarchyウインドウを取得します。


//emlist[][cs]{
EditorWindow hierarcyWindow = EditorWindow.GetWindow(hierarcyType, false);
//}


複数対応する場合は@<tt>{Resources.FindObjectsOfTypeAll(Type);}を使用して全てのHierarchyウインドウを取得しますが複雑になってしまうので今回は単体ウインドウのみ対応で実装を行います。Hierarchyウインドウのオブジェクトの中に、Serializeされた@<tt>{m_CurrentSortMethod}という文字列データが存在します。これが現在選択されているソート名となります。



m_CurrentSortMethodはSerializeされているデータなのでSerializedObject#FindPropertyで取得するようにしましょう。そのためにはまずHierarchyウインドウの@<tt>{SerializedObject}を取得しなければいけません。今回はSerializedObjectを取得ではなく作成します。SerializedObjectを使用する場面は普段はCustomEditorなどで、すでに作成されたSerializedObjectを使ってアクセスしている方が多いと思います。なので、自分で作成するのは初めての方も多いのではないでしょうか。


//emlist[][cs]{
SerializedObject hierarcyWindowObject = new SerializedObject(hierarcyWindow);
//}


こうして現在のソート名を取得することが可能になりました。


//emlist[][cs]{
var currentSortName = hierarcyWindowObject.FindProperty("m_CurrentSortMethod").stringValue;
//}


ですがこれだけではソート名を取得するには不十分です。
確かに現在のタグ名を取得しているように見えますが細かく言うと「@<strong>{SerializedObject作成時のソート名を取得している}」ことになっています。「@<strong>{最新のSerializedObjectからソート名を取得するためには}@<tt>{SerializedObject#Update}を呼び出してあげてください。こうすることによって常に最新の「現在のソート名」を取得することが可能です。


=== 現在のソート名がTagSort名以外であればタグ表示をやめる


ここまでくれば残りの実装は簡単です。


//emlist[][cs]{

EditorApplication.hierarchyWindowItemOnGUI += HierarchyWindowItemOnGUI;

private void HierarchyWindowItemOnGUI(int instanceID, Rect selectionRect)
{
  hierarcyWindowObject.Update();
  var currentSortName =
      hierarcyWindowObject.FindProperty("m_CurrentSortMethod").stringValue;

  if (currentSortName != "TagSort")
  {
    EditorApplication.hierarchyWindowItemOnGUI -= HierarchyWindowItemOnGUI;
    hierarcyWindowObject = null;
    return;
  }

  // Tag名表示
}
//}

=== HierarchySort のアイコンを変更

TagSort専用のアイコンに変更します。

//image[ss03][]{

//}

用意したアイコンを「Editor Default Resources/Icons」フォルダに格納します。

//image[ss04][]{

//}

これにより、@<code>{EditorGUIUtility.IconContent ("TagSort");}でアイコンをロードすることが可能になりました。このアイコンを適用するために、@<code>{content}変数をオーバーライドします。
//emlist[][cs]{
public class TagSort : BaseHierarchySort
{
	private GUIContent m_content = EditorGUIUtility.IconContent ("TagSort");

	public override GUIContent content {
		get {
			return m_content;
		}
	}

	// 略
}
//}


実際のコードがこちらになります。


//emlist[][cs]{
using UnityEngine;
using UnityEditor;

public class TagSort : BaseHierarchySort
{
	private SerializedObject hierarcyWindowObject;

	public override int Compare (GameObject lhs, GameObject rhs)
	{
		if (lhs == rhs)
			return 0;
		if (lhs == null)
			return -1;
		if (rhs == null)
			return 1;
		return EditorUtility.NaturalCompare (lhs.tag, rhs.tag);
	}
	private GUIContent m_content = EditorGUIUtility.IconContent ("TagSort");

	public override GUIContent content {
		get {

			if (hierarcyWindowObject == null) {
				var hierarcyType = Types.GetType ("UnityEditor.SceneHierarchyWindow", "UnityEditor.dll");
				var hierarcyWindows = Resources.FindObjectsOfTypeAll (hierarcyType);
				if (hierarcyWindows.Length != 0) {
					hierarcyWindowObject = new SerializedObject (hierarcyWindows[0]);
					EditorApplication.hierarchyWindowItemOnGUI += HierarchyWindowItemOnGUI;
				}
			}

			return m_content;
		}
	}

	private void HierarchyWindowItemOnGUI (int instanceID, Rect selectionRect)
	{
		hierarcyWindowObject.Update ();

		var currentSortName =
			hierarcyWindowObject.FindProperty ("m_CurrentSortMethod").stringValue;

		if (currentSortName != "TagSort") {
			EditorApplication.hierarchyWindowItemOnGUI -= HierarchyWindowItemOnGUI;
			hierarcyWindowObject = null;
			return;
		}

		var go = EditorUtility.InstanceIDToObject (instanceID) as GameObject;

		selectionRect.x += selectionRect.width - 64;
		selectionRect.width = 64;

		EditorGUI.LabelField (selectionRect, go.tag, Style.miniBox);
	}

	private class Style
	{
		public static GUIStyle miniBox;

		static Style ()
		{
			miniBox = new GUIStyle ("box");
			miniBox.fontSize = 8;
		}
	}
}
//}
