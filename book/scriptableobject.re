= ScriptableObject

#@# TODO インスペクターに情報を表示するためには ScriptableObject を使うと言う節を書く

== ScriptableObject とは

ScriptableObject は独自のアセットを作成するための仕組みです。また、Unityのシリアライズ機構が扱う形式とも言えます。

Unity には独自のシリアライズ機構を持っており、全てのオブジェクト（UnityEngine.Object）は、そのシリアライズ機構を通してデータのシリアライズ/デシリアライズを行い、ファイルとUnity エディター間でやり取りをしています。シリアライズ機構については@<chapref>{serializedobject}を参照してください。

Unity 内部のアセット（マテリアルやアニメーションクリップ等）は全て UnityEngine.Object の派生クラスです。独自のアセットを作成するために、UnityEngine.Object の派生クラスを作成したいですが、ユーザー側では UnityEngine.Object の派生クラスを作成するのは禁止されています。ユーザーが Unity のシリアライズ機構を利用した、独自のアセットを作成するには @<code>{ScriptableObject} を扱う必要があります。

== ScriptableObject は Unity エディターの要

ScriptableObject は Unity エディターのいたる所で使われています。例えば、シーンビューやゲームビューなどの エディターウィンドウ は、ScriptableObject の派生クラスから生成されており、また、インスペクターに GUI を表示するための Editor オブジェクトも ScriptableObject の派生クラスから生成されています。Unity エディターは ScriptableObject で作成されているといっても過言ではありません。

//image[ss02][アセンブリブラウザーで見ると ScriptableObject が継承されているのが分かる]{

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

アセットの拡張子は、@<b>{必ず} @<code>{.asset} でなくてはいけません。他の拡張子にしてしまうと、Unity が ScriptableObject 派生のアセットとして認識しません。

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

また、@<code>{CreateAssetMenu} 属性を使うことで簡単にアセットを作成できます。

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

MonoBehaviour と同じで、フィールドに SerializeField を付けるだけで表示されるようになります。またPropertyDrawer も適用されます。

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

== ScriptableObject の親子関係

まずは、「親のScriptableObject」と、その親が変数として持つ「子のScriptableObject」をイメージしてください。

下記が、そのイメージをコードに流し込んだものです。

//emlist[親のScriptableObject]{
using UnityEngine;

public class ParentScriptableObject : ScriptableObject
{
    [SerializeField]
    ChildScriptableObject child1;
}
//}

//emlist[子のScriptableObject]{
using UnityEngine;

public class ChildScriptableObject : ScriptableObject
{
  // 何もないとインスペクターが寂しいので変数追加
  [SerializeField]
  string str;

  public ChildScriptableObject ()
  {
    // 初期アセット名を設定
    name = "New ChildScriptableObject";
  }
}

//}

次に、ParentScriptableObjectをアセットとして保存します。引数にある child もインスタンス化した状態にしてみましょう。

//emlist[子のScriptableObject]{
using UnityEngine;
using UnityEditor;

public class ParentScriptableObject : ScriptableObject
{
  const string PATH = "Assets/Editor/New ParentScriptableObject.asset";

  [SerializeField]
  ChildScriptableObject child;

  [MenuItem ("Assets/Create ScriptableObject")]
  static void CreateScriptableObject ()
  {
    //親をインスタンス化
    var parent = ScriptableObject.CreateInstance<ParentScriptableObject> ();

    //子をインスタンス化
    parent.child = ScriptableObject.CreateInstance<ChildScriptableObject> ();

    // 親をアセットとして保存
    AssetDatabase.CreateAsset (parent, PATH);

    // インポート処理を走らせて最新の状態にする
    AssetDatabase.ImportAsset (PATH);
  }
}
//}

ParentScriptableObject をアセットとして保存した後、インスペクターを見てみると、@<img>{ss08}のようにchildプロパティーが@<b>{Type mismatch}になります。

//image[ss08][childプロパティーが@<code>{Type mismatch}となっている]{

//}

試しに、@<code>{Type mismatch}の部分をダブルクリックしてみると、ChildScriptableObject の情報がインスペクターに表示され、問題なく正しい挙動をしているように見えます。

//image[ss09][ChildScriptableObjectのプロパティーを操作することが出来る]{

//}

=== UnityEngine.Object をアセットとして扱うにはディスクに保存しなければいけない

@<code>{Type mismatch}状態の child を持つ ParentScriptableObject を作成したら、このまま Unity を再起動してみましょう。再度、ParentScriptableObject のインスペクターを見ると child の部分が None（null） になっているのがわかります。

//image[ss10][アセットにする前に child のインスタンスを作成したのに null となっている]{

//}

このは理由は@<b>{ScriptableObject の基底クラスである UnityEngine.Object}をシリアライズデータとして扱うには、ディスク上にアセットとして保存しなければいけません。@<code>{Type mismatch}状態は、@<b>{インスタンスは存在するが、ディスク上にアセットとして存在しない状態}を指しています。つまり、そのインスタンスが何らかの状況（Unity再起動など）で破棄されてしまうとデータにアクセスができなくなります。

=== ScriptableObject は全てアセットとして保存すること

@<code>{Type mismatch}の状況を回避するのはとても簡単です。ScriptableObject を全てアセットとして保存して、その参照をシリアライズ可能なフィールドに持たせることで解決します。

//image[ss11][テクスチャなどのアセットのように普段行うようなドラッグ＆ドロップの操作で参照を持たせることが出来る]{

//}

ただし、今回のように「親」と「子」の関係がある状態で、それぞれ独立したアセットを作成してしまうのは管理面で見ても得策ではありません。子の数が増えたり、リストを扱うことになった時にその分アセットを作成するのは非常にファイル管理が面倒になってきます。

そこで @<b>{サブアセット} という機能を使って親子関係であるアセットを1つにまとめ上げることができます。

=== サブアセット

親となるメインアセットにアセット情報を付加することで UnityEngine.Object がサブアセットとして扱われます。このサブアセットの例で一番わかりやすいのがモデルアセットです。

モデルアセットの中には、メッシュやアニメーションなどのアセットが含まれています。これらは普段、独立したアセットとして存在しなければいけませんが、サブアセットとして扱うことで、メッシュやアニメーションクリップのアセットを、メインのアセット情報の中に包括してディスク上に保存することなく使用することが出来ます。

//image[ss12][モデルアセットの中にはメッシュやアバター、アニメーションなどが含まれる]{

//}

ScriptableObject もサブアセットの機能を使うことで、ディスク上に余計なアセットを増やすことなく親子関係の ScriptableObject を構築することが出来ます。

==== AssetDatabase.AddObjectToAsset

UnityEngine.Object をサブアセットとして登録するには、メインとなるアセットにオブジェクトを追加します。

//emlist[子のScriptableObject]{
using UnityEngine;
using UnityEditor;

public class ParentScriptableObject : ScriptableObject
{
  const string PATH = "Assets/Editor/New ParentScriptableObject.asset";

  [SerializeField]
  ChildScriptableObject child;

  [MenuItem ("Assets/Create ScriptableObject")]
  static void CreateScriptableObject ()
  {
    //親をインスタンス化
    var parent = ScriptableObject.CreateInstance<ParentScriptableObject> ();

    //子をインスタンス化
    parent.child = ScriptableObject.CreateInstance<ChildScriptableObject> ();

    //親に child オブジェクトを追加
    AssetDatabase.AddObjectToAsset (parent.child, PATH);

    //親をアセットとして保存
    AssetDatabase.CreateAsset (parent, PATH);

    //インポート処理を走らせて最新の状態にする
    AssetDatabase.ImportAsset (PATH);
  }
}
//}


@<img>{ss13}をみるとわかりますが、親である ParentScriptableObject が 2つあるように見えたり、実質データを持っているのは階層的にはサブアセットの ParentScriptableObject であったりと少し特殊な階層構造となっていることがわかります。

//image[ss13][メインアセットにサブアセットが追加された状態]{

//}

この状態は、ユーザーが（サブアセットを作成したことによって）特殊なアセットを作成したと Unity が判断し、メインアセットを何にも属さない DefaultAsset として表示した状態です。

メインアセットとして扱いたいアセットがサブアセット側に移動してしまう状況はとても気持ちのいいものではありません。このように、ユーザーの手でサブアセットを作成することはできますが、これをモデルのように最大限活用することは未だ想定されていません。

==== HideFlags.HideInHierarchy でサブアセットを隠す

サブアセット自体を隠すことによって、メインアセットのみが存在するような見た目を作成することが出来ます。

//emlist{
[MenuItem ("Assets/Create ScriptableObject")]
static void CreateScriptableObject ()
{
  var parent = ScriptableObject.CreateInstance<ParentScriptableObject> ();
  parent.child = ScriptableObject.CreateInstance<ChildScriptableObject> ();

　//サブアセットとなる child を非表示する
  parent.child.hideFlags = HideFlags.HideInHierarchy;

  AssetDatabase.AddObjectToAsset (parent.child, PATH);

  AssetDatabase.CreateAsset (parent, PATH);
  AssetDatabase.ImportAsset (PATH);
}
//}

このように、階層表示はなくなりましたが2つのアセットを1つにまとめて扱うことが出来るようになりました。

//image[ss14][ParentScriptableObject のみ表示されているがサブアセットの ChildScriptableObject の参照が正しく行われている]{

//}

このサブアセットを隠す方法は、AnimatorControllerでも行われています。確かめてみましょう。

//emlist{
[MenuItem ("Assets/Set to HideFlags.None")]
static void SetHideFlags ()
{
  //AnimatorController を選択した状態でメニューを実行
  var path = AssetDatabase.GetAssetPath (Selection.activeObject);

  //サブアセット含め全て取得
  foreach (var item in AssetDatabase.LoadAllAssetsAtPath(path)) {
    //フラグを全て None にして非表示設定を解除
    item.hideFlags = HideFlags.None;
  }
  //再インポートして最新にする
  AssetDatabase.ImportAsset (path);
}
//}

上記のコードを実行すると、HideFlagsが解除され、サブアセットが表示されます。

//image[ss15][LayerやBrendTreeなどがサブアセットとして追加されていることが分かる]{

//}


==== メインアセットからサブアセットを削除する

サブアセットの削除方法は簡単です。@<code>{Object.DestroyImmediate} を使うことによってサブアセットを削除することが出来ます。

//emlist{
[MenuItem ("Assets/Remove ChildScriptableObject")]
static void Remove ()
{
  var parent = AssetDatabase.LoadAssetAtPath<ParentScriptableObject> (PATH);

  //アセットの CarentScriptableObject を破棄
  Object.DestroyImmediate (parent.child, true);

  //破棄したら Missing 状態になるので null を代入
  parent.child = null;

  //再インポートして最新の情報に更新
  AssetDatabase.ImportAsset (PATH);
}
//}


== アイコンの変更

デフォルトのアイコンは @<icon>{ScriptableObject} となっています。特に重要ではありませんがこのアイコンを変更する方法があります。

=== スクリプトにアイコンを設定

スクリプトアセットを選択してアイコンの部分を選択すると、アイコン変更パネルが表示されます。ここで「Other」ボタンをクリックして変更したいアイコンのテクスチャを選んでください。

//indepimage[ss04]

//image[ss05][わかりにくいがスクリプトと ScriptableObject にアイコンが設定された]{

//}

=== Gizmos にアイコンを置く

もう1つアイコンを変更する方法として、@<b>{Gizmos}フォルダーに@<code>{[クラス名] Icon} という名前でアイコン画像を置くと変更されるようになります。Gizmos フォルダーが Assets フォルダー直下のみという仕様で少し使いづらいかもしれませんが、同じアイコン画像が3つ並ばなくても良くなる点で、この方法も覚えておくと便利かもしれません。

//image[ss06][]{

//}
