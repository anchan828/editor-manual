= ScriptableObject


== ScriptableObject とは

ScriptableObject は独自のアセットを作成するための仕組みです。また、Unityのシリアライズ機構を扱うための形式とも言えます。

Unity には独自のシリアライズ機構を持っており、全てのオブジェクト（UnityEngine.Object）は、そのシリアライズ機構を通してデータのシリアライズ/デシリアライズを行い、ファイルとUnityエディター間のやりとりをしています。シリアライズ機構については@<chapref>{serializedobject}を参照してください。

Unity 内部のアセット（マテリアルやアニメーションクリップ等）は全て UnityEngine.Object の派生クラスです。独自のアセットを作成するために、UnityEngine.Object の派生クラスを作成したいですが、ユーザー側では UnityEngine.Object の派生クラスを作成するのは禁止されています。ユーザーが Unity のシリアライズ機構を利用した、独自のアセットを作成するには ScriptableObject を扱う必要があります。

== ScriptableObject は Unity エディターの要

ScriptableObject は Unity エディターのいたる所で使われています。例えば、シーンビューやゲームビューなどの エディターウインドウ は、ScriptableObject の派生クラスから生成されており、また、インスペクターに GUI を表示するための Editor オブジェクトも ScriptableObject の派生クラスから生成されています。Unity エディターは ScriptableObject で作成されているといっても過言ではありません。

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

アセットの拡張子は、@<b>{必ず} @<code>{.asset} でなくてはいけません。他の拡張子にしてしまうと、Unity が ScriptableObject 派生のアセットと認識しません。

//emlist{
[MenuItem ("Example/Create ExampleAsset")]
static void CreateExampleAsset ()
{
    var exampleAsset = CreateInstance<ExampleAsset> ();

    AssetDatabase.CreateAsset (exampleAsset, "Assets/Editor/ExampleAsset.asset");
    AssetDatabase.Refresh ();
}
//}

//indepimage[ss01]

また、@<code>{CreateAssetMenu} 属性を使うことで簡単にアセットを作成することが出来ます。

//emlist{
using UnityEngine;
using UnityEditor;

[CreateAssetMenu(menuName = "Example/Create ExampleAsset Instance")]
public class ExampleAsset : ScriptableObject
{
}
//}

CreateAssetMenu を使用した場合は 「Assets/Create」配下にメニューが作成されます。

//indepimage[ss07]

=== スクリプトからアセットの ScriptableObject をロードする

読み込む方法も簡単で @<code>{AssetDatabase.LoadAssetAtPath} を使って読み込みます。

//emlist{
[MenuItem ("Example/Load ExampleAsset")]
static void LoadExampleAsset ()
{
    var exampleAsset = 
    　　　　AssetDatabase.LoadAssetAtPath<ExampleAsset> 
                    ("Assets/Editor/ExampleAsset.asset");
}
//}

=== インスペクターにプロパティーを表示する

MonoBehaviour と同じで、フィールドに SerializeField を付けるだけで表示されるようになります。また、 PropertyDrawer も適用されます。

//indepimage[ss03]

//emlist{
using UnityEngine;
using UnityEditor;

public class ExampleAsset : ScriptableObject
{
    [SerializeField]
    string str;

    [SerializeField, Range (0, 10)]
    int number;

    [MenuItem ("Example/Create ExampleAsset Instance")]
    static void CreateExampleAssetInstance ()
    {
        var exampleAsset = CreateInstance<ExampleAsset> ();

        AssetDatabase.CreateAsset (exampleAsset, "Assets/Editor/ExampleAsset.asset");
        AssetDatabase.Refresh ();
    }
}
//}

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