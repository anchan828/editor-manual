= ProjectWindowUtil


普段 「Assets/Create/」メニューからスクリプトファイルやマテリアルなどさまざまなアセットを作成します。作成時に、「NewBehaviourScript」を初期名から変更するテキストフィールドが表示されたりと、ユーザーにアセットを作成するために便利な機能がそろっています。

//image[ss01][左から、フォルダーを選択、Create/Materialを選択、フォルダー以下にNew Material生成]{

//}

これらの機能は @<code>{ProjectWindowUtil} クラスとして提供されており、@<code>{AssetDatabase.CreateAsset} よりもユーザーとの対話に優れたインタラクティブな実装を行うことができます。


== アセットの生成

アセットの生成は @<code>{ProjectWindowUtil.CreateAsset} を使用します。第2引数の pathName は "Assets/New Material.mat" というような階層を表現するパスではなく"New Material.mat" のアセット名のみを設定するようにしてください。こうすることで、現在選択中のフォルダー以下にアセットを生成するようになります。

//emlist{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript
{
    [MenuItem ("Assets/Create ExampleAssets")]
    static void CreateExampleAssets ()
    {
        var material = new Material (Shader.Find ("Standard"));

        ProjectWindowUtil.CreateAsset (material, "New Material.mat");
    }
}
//}

== アセット名決定後に行う EndNameEditAction を使いこなす

例えば、Unity 上でスクリプトファイルを作成した時、クラス名がスクリプトファイル名と同じになります。これはアセット名の決定後に EndNameEditAction が実行され、その中でスクリプトファイルのテキストを書き換えています。

では「Unity標準のスクリプトファイルを作成するときと同じ挙動」をする EndNameEditAction を作成してみましょう。

=== EndNameEditAction を継承したクラスの作成

EndNameEditAction を継承したクラスを作成します。そして Action メソッドをオーバーライドし、その中で @<code>{pathName} と @<code>{resourceFile}（テンプレートファイルのパス）を使ってテキストアセットを作成します。

//emlist{
using System.IO;
using System.Text;
using UnityEditor;
using UnityEditor.ProjectWindowCallback;

public class DoCreateScriptAsset : EndNameEditAction
{
    public override void Action (int instanceId, string pathName, string resourceFile)
    {
        var text = File.ReadAllText (resourceFile);
        
        var className = Path.GetFileNameWithoutExtension (pathName);
        
        // 半角スペースを除く
        className = className.Replace (" ", "");

        // 他のパラメータ名について知りたい場合は
        // @<chapref>{scripttemplates}を参照してください
        text = text.Replace ("#SCRIPTNAME#", className);

        text += "\n//コード追加！";

        // UTF8 の BOM 付きで保存
        var encoding = new UTF8Encoding (true, false);

        File.WriteAllText (pathName, text, encoding);

        AssetDatabase.ImportAsset (pathName);
        var asset = AssetDatabase.LoadAssetAtPath<MonoScript> (pathName);
        ProjectWindowUtil.ShowCreatedAsset (asset);
    }
}
//}

=== StartNameEditingIfProjectWindowExists

@<code>{ProjectWindowUtil.StartNameEditingIfProjectWindowExists} を使ってアセットを作成します。この関数は @<code>{ProjectWindowUtil.CreateAsset} の内部でも使われており、正しく使うことができれば同じ動作を実装できます。

StartNameEditingIfProjectWindowExists は 2 つの生成パターンをサポートしています。

@<b>{1つ目は、Unityエディター上にはない外部ファイルを元にしてアセットを生成する方法です。}    

これはスクリプトファイルの生成のような場合に使用します。第5引数が外部ファイル（=リソース）のパスを指定するようになっています。この時、インスタンスIDは必要ないので 0 にします。

例としては以下のコードになります。このコードと先ほど作成した @<code>{DoCreateScriptAsset} クラスを使用することによってスクリプトファイルを作成できます。

//emlist{
[MenuItem ("Assets/Create ExampleAssets")]
static void CreateExampleAssets ()
{
    var resourceFile = Path.Combine (EditorApplication.applicationContentsPath,
     "Resources/ScriptTemplates/81-C# Script-NewBehaviourScript.cs.txt");
    
    // cs は "cs Script Icon"
    // js は "js Script Icon"
    Texture2D csIcon =
                   EditorGUIUtility.IconContent ("cs Script Icon").image as Texture2D;
    
    var endNameEditAction =
                   ScriptableObject.CreateInstance<DoCreateScriptAsset> ();

    ProjectWindowUtil.StartNameEditingIfProjectWindowExists (0, endNameEditAction, 
    	                        "NewBehaviourScript.cs", csIcon, resourceFile);
}
//}


//indepimage[ss03]

@<b>{2つ目は、既にインスタンス化されているオブジェクトをアセットとして生成する方法です。}

これはマテリアルのような、Unity 上で生成できるアセット（他には AnimationClip や ScriptableObject）を生成する時に使用します。第1引数がオブジェクトのインスタンスIDです。第5引数のリソースパスは必要ないので空文字にします。

例としてマテリアルを生成してみます。まずは EndNameEditAction の DoCreateMaterialAsset クラスを作成しましょう。

//emlist{
using UnityEngine;
using UnityEditor;
using UnityEditor.ProjectWindowCallback;

public class DoCreateMaterialAsset : EndNameEditAction
{
    public override void Action (int instanceId, string pathName, string resourceFile)
    {
        var mat = (Material)EditorUtility.InstanceIDToObject (instanceId);

        // 強制的にマテリアルを赤色にする
        mat.color = Color.red;

        AssetDatabase.CreateAsset (mat, pathName);
        AssetDatabase.ImportAsset (pathName);
        ProjectWindowUtil.ShowCreatedAsset (mat);
    }
}
//}

作成した DoCreateMaterialAsset を使って StartNameEditingIfProjectWindowExists を呼び出します。

//emlist{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript
{
    [MenuItem ("Assets/Create ExampleAssets")]
    static void CreateExampleAssets ()
    {
        var material = new Material (Shader.Find ("Standard"));
        var instanceID = material.GetInstanceID ();
        // マテリアルのアイコンを取得
        var icon = AssetPreview.GetMiniThumbnail (material);

        var endNameEditAction = 
                   ScriptableObject.CreateInstance<DoCreateMaterialAsset> ();

        ProjectWindowUtil.StartNameEditingIfProjectWindowExists (instanceID, 
        	                    endNameEditAction, "New Material.mat", icon, "");
    }
}
//}

これでマテリアルを作成することができました。

//indepimage[ss02]
