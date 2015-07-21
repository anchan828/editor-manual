= ScriptableObject

前提知識としては、公式ドキュメント - @<href>{http://docs.unity3d.com/ja/current/Manual/class-ScriptableObject.html, ScriptableObject} をご覧ください。本章では、エディタ拡張で ScriptableObject を使う方法について説明していきます。

== ScriptableObject とは

Unity には独自のシリアライズ機構を持っており、全てのオブジェクトは UnityEngine.Object 経由でシリアライズが行われます。ですがユーザーは UnityEngine.Object の派生クラスを作成することが出来ないため、必ず ScriptableObject を使用する必要があります。

少しいい方を変えてみると「アセットを作成するには ScriptableObject を使う必要がある」とも言えます。

== ScriptableObject は Unity エディターの要

ScriptableObject は Unity エディタのいたる所で使われています。例えば、Scene ビューやGame ビューなどの EditorWindow は ScriptableObject 経由で作成されていたり、インスペクターに GUI を表示するための Editor オブジェクトも ScriptableObject で作成されています。Unity エディターは ScriptableObject で作成されているといっても過言ではありません。

//image[ss02][アセンブリブラウザで見ると ScriptableObject が継承されているのが分かる]{

//}

== ScriptableObject を作成する

ScriptableObject を作成するには、まず ScriptableObject クラスを継承したクラスを作成する必要があります。この時、クラス名とアセット名は同じでなければいけません。MonoBehaviour と同じ制限です。

//emlist{
using UnityEngine;

public class ExampleAsset : ScriptableObject
{

}
//}

=== インスタンス化

ScriptableObject を生成するには @<code>{ScriptableObject.CreateInstance} で生成します。@<code>{new} を使用してインスタンス化はしてはいけません。理由は MonoBehaviour と同じで、Unity のシリアライズ機構経由でオブジェクトを作成する必要があるからです。

//emlist{
using UnityEngine;
using UnityEditor;

public class ExampleAsset : ScriptableObject
{
	[MenuItem ("Example/Create ExampleAsset Instance")]
	static void CreateExampleAssetInstance ()
	{
		var exampleAsset = CreateInstance<ExampleAsset> ();
	}
}
//}

=== アセットとして保存

次にインスタンス化したオブジェクトをアセットとして保存します。アセットの作成は @<code>{AssetDatabase.CreateAsset} を使って作成することが可能です。

アセットの拡張子は、@<b>{必ず} @<code>{.asset} でなくてはいけません。他の拡張子にしてしまうと、Unity が ScriptableObject 経由で作成されたアセットと認識しません。

//emlist{
[MenuItem ("Example/Create ExampleAsset")]
static void CreateExampleAsset ()
{
	var exampleAsset = CreateInstance<ExampleAsset> ();

	AssetDatabase.CreateAsset (exampleAsset, "Assets/Editor/ExampleAsset.asset");
	AssetDatabase.Refresh ();
}
//}

//image[ss01][]{
//}

=== ScriptableObject をアセットから読み込む

読み込む方法も簡単で @<code>{AssetDatabase.LoadAssetAtPath} を使って読み込みます。

//emlist{
[MenuItem ("Example/Load ExampleAsset")]
static void LoadExampleAsset ()
{
    var exampleAsset = AssetDatabase.LoadAssetAtPath<ExampleAsset> ("Assets/Editor/ExampleAsset.asset");
}
//}

=== インスペクターにプロパティーを表示する

MonoBehaviour と同じで、フィールドに SerializeField を付けるだけで表示されるようになります。また、 PropertyDrawer も適用されます。

//indepimage[ss03]


== アイコンの変更

デフォルトのアイコンは @<icon>{ScriptableObject} となっています。特に重要ではありませんがこのアイコンを変更する方法があります。

=== スクリプトにアイコンを設定

スクリプトアセットを選択してアイコンの部分を選択すると、アイコン変更パネルが表示されます。ここで「Other」ボタンをクリックして変更したいアイコンのテクスチャを選んでください。

//image[ss04][]{

//}

//image[ss05][わかりにくいがスクリプトと ScriptableObject にアイコンが設定されたのが分かる]{

//}

=== Gozmos にアイコンを置く

もう1つアイコンを変更する方法として、@<b>{Gizmos}フォルダーに@<code>{[クラス名] Icon} という名前でアイコン画像を置くと変更されるようになります。Gizmos フォルダーが Assets フォルダー直下のみという仕様で少し使いづらいかもしれませんが、同じアイコン画像が3つ並ばなくても良くなる点で、この方法も覚えておくと便利かもしれません。

//image[ss06][]{

//}

== CustomEditor

CustomEditor も MonoBehaviour と同様の方法で作成することが可能です。

//emlist{
using UnityEditor;

[CustomEditor (typeof(ExampleAsset))]
public class ExampleAssetInspector : Editor
{
    public override void OnInspectorGUI ()
    {
    }
}
//}