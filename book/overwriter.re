= Overwriter

//indepimage[frontispiece]
//pagebreak

== この章で得られる知識

 * Unityの管理外でファイルの変更が非推奨なワケ
 * AssetDatabase.CopyAssetの不具合？
 * AssetPostprocessor.OnPostprocessAllAssets
 * EditorUtility.DisplayDialog
 * EditorUtility.DisplayDialogComplex
 * AssetDatabase.CopyAsset
 * FileUtil.ReplaceFile
 * AssetDatabase.DeleteAsset
 * AssetDatabase.ImportAsset

== 本章で使用するソースコード

@<href>{https://gist.github.com/anchan828/1a95a3f6b88c81053582}

//table[t1][]{
ファイル	説明
------------------------
OverwriteAsset.cs	アセット管理
Overwriter.cs	AssetPostprocessorを継承したクラス
//}

== 概要

ファイルをドラッグ＆ドロップによって上書きすることはよくあります。みなさんも必ずと言っていいほど経験しているのではないでしょうか。

//image[ss01][Finderで同じ名前のファイルがある時に表示されるダイアログ（Mac）]{

//}


Unityでもこのような操作を行いたいところですが、Unityにはファイルの上書き機能はなく、ファイル名の最後に数字を付けて別ファイル扱いとされます。


//image[ss02][hoge 1.pngというように別ファイル扱い]{

//}

どうしても上書きしたい場合は、Finder経由で上書きを行わなければなりません。しかし、Finder上で操作するというようなUnityの管理外でアセットの変更を行うことは非推奨とされています。

これをUnity上で解決したものが本章の@<b>{Overwriter}です。

===[column] Unityの管理外でファイルの変更が非推奨なワケ

　これは、@<b>{Unityが管理しているデータベースと実際のアセットとの整合性が取れなくなってしまうため}です。

　各アセットにはGUIDが割り振られ、これをkeyとしてデータベースで管理されています。このGUIDは@<b>{.meta}ファイルに記述されています。


//emlist{
fileFormatVersion: 2
guid: 4498d464658a84c7c8998b6b66709951
TextureImporter:
  fileIDToRecycleName: {}
  serializedVersion: 2
//}

このGUIDが変更されると@<img>{ss03}のように参照が「Missing」状態になります。

//image[ss03][コンポーネントでのオブジェクト参照もGUIDで行われています]{

//}

意図的にMissingを発生させるには、Finder上で対象ファイルを移動させます。

//image[ss04][hoge.pngをFolderに移動させます]{

//}

こうするとUnityは

 * hoge.png が削除された
 * hoge.png が新しくFolderの中にインポートされた

と判断され、同じhoge.pngでも全く別物として扱われます。

ファイルを別物として扱わないようにするには.metaファイルも一緒に移動させれば良い話なのですが、Unity独特の仕様なため、.metaファイルを一緒に移動させるのを忘れがちです。（忘れたままUnityエディタに戻った場合、.metaファイルが自動で削除/生成されて悲惨なことに...）

===[/column]

== Overwriteの使い方

//image[ss05][同じ名前のアセットが存在するとダイアログが出る]{

//}

説明するまでもないと思いますが、アセットをインポートしようとした時、既に同じファイル名が存在した場合にダイアログが表示されます。「置き換える」を選択した場合は上書きされ、「両方とも残す」を選択した場合は@<img>{ss06}のように別アセットとしてインポートされます。

//image[ss06][hoge 1.pngとしてインポートされている]{

//}

== ファイルのインポート監視

　まず、上書きするアセットがインポートされるかの監視をしなければいけません。Unityエディタに何のアセットがインポートされようとしているか、インポートされたかを知るには@<b>{AssetPostprocessor}クラスを使います。詳しいAssetPostprocessorの使い方については、@<chapref>{assetpostprocessor}を御覧ください。


//emlist[][cs]{
using UnityEditor;

public class Overwriter : AssetPostprocessor
{
    static void OnPostprocessAllAssets (
        string[] importedAssets, 
        string[] deletedAssets, 
        string[] movedAssets, 
        string[] movedFromPath)
    {
        foreach (var assetPath in importedAssets) {
            // インポートされたアセットを監視
        }
    }
}
//}

今回は@<b>{OnPostprocessAllAssets}を使用します。鋭い人はもうお気づきかもしれませんが、アセットを上書きすると言っていますが処理的には「@<b>{別アセットとしてインポートし、置き換える}」となります。これは（エディタ拡張でアクセスできる範囲で）インポート前のアセットにアクセスするとき、同じファイル名があった場合にはインポートできるように@<b>{既にリネームされた}状態になってしまうためです。hoge.pngをインポートする時に、同じ名前のアセットがあればhoge 1.pngとなります。


この仕様を受けて、Overwriteの処理の順番は以下になります。

 1. とりあえずインポートする。ここでhoge.pngというアセットが既にある場合はhoge 1.pngとなる。
 2. 別アセットとしてインポートされた時に末尾についた「<半角スペース><数字>」が存在する場合、アセットを上書きしたいものとして判断する。
 3. 上書きするかどうかのダイアログを出し、ユーザーの確認が取れれば上書きする。


== ドラッグした時のみ処理を実行する


Unity上でアセットのインポートが行われた時に、再インポートが行われるため、実装したOnPostprocessAllAssetsが呼び出されてしまいます。
そこでマウスでドラッグしてインポートした時のみ処理を行うようにします。

//emlist[][cs]{
static void OnPostprocessAllAssets ( 
    string[] importedAssets,                                      
    string[] deletedAssets, 
    string[] movedAssets,                                      
    string[] movedFromPath)
{

    // スクリプトの編集によるインポート時はEvent.currentはnullなので、nullチェックをする
    if (Event.current == null || 
        Event.current.type != EventType.DragPerform)
        return;

    foreach (var assetPath in importedAssets) {

    }
}

//}

== ダイアログの表示

ダイアログの表示には@<b>{EditorUtility.DisplayDialogComplex}を使用します。


//image[ss08][今回は「置き換える」「両方とも残す」「中止」の3択が必要となるのでDisplayDialogComplexとなります。]{

//}

//image[ss07][同じ機能として@<b>{EditorUtility.DisplayDialog}がありますが、こちらはボタンが2つになります。「はい」「いいえ」の2択で表現できるときのみ使用しましょう。]{

//}


//emlist[][cs]{
var result = EditorUtility.DisplayDialogComplex (
    asset.originalAssetPath, 
    overwriteMessage, 
    "置き換える",    // 選択時の戻り値は 0
    "両方とも残す",  // 選択時の戻り値は 1
    "中止");        // 選択時の戻り値は 2

if (result == 0) {
    asset.Overwrite ();
} else if (result == 2) {
    asset.Delete ();
}
//}

== 上書き

AssetDatabaseクラスにはCopyAsset関数がありますが、上書きするという機能はありません。厳密に言うと上書きできてしまうのですが、Unityのデータベースに不具合が生じます。詳しくは「@<column>{copy}」で紹介します。 今回はアセットの上書きに@<b>{FileUtil.ReplaceFile}関数を使用します。
上書きコピーが完了した後、別アセットとしてインポートされたものは@<b>{AssetDatabase.DeleteAsset}で削除します。

@<b>{重要:} 最後に、@<b>{System.IO.File}によって外部からのデータ変更されたものをUnityが把握するために@<b>{AssetDatabase.ImportAsset}を実行します。

//emlist[][cs]{
public void Overwrite ()
{
    FileUtil.ReplaceFile (assetPath, originalAssetPath);
    Delete ();
    AssetDatabase.ImportAsset (originalAssetPath);
}

public void Delete ()
{
    AssetDatabase.DeleteAsset (assetPath);
}
//}

===[column]{copy}AssetDatabase.CopyAssetの不具合？

アセットをコピーして複製するにはこの関数を使用します。ですがこの関数はアセットの上書きを考慮しておらず、Unity側のデータベースに不具合が生じます。

//image[ss09][同じ名前のhogeアセットが2つ表示されている。この画像では正しくインスペクターに情報が表示されているが...]{

//}

//image[ss10][こちらではインスペクターに情報が表示されていない]{

//}

//image[ss11][Finderで見ると片方の画像しか無いことがわかる]{

//}

この不具合から元に戻すには、@<code>{ReImport All}をしてLibraryにあるのデータベースを再生成しなければいけません。

===[/column]

//emlist[][cs]{
using System.IO;
using System.Text.RegularExpressions;
using UnityEditor;

public class OverwriteAsset
{
    public string originalAssetPath {
        get {
            return Path.Combine (directoryName, filename + "." + extension);
        }
    }
    
    public bool exists { get; private set; }
    
    public string filename { get; private set; }
    
    public string extension { get; private set; }
    
    private string directoryName;
    private string assetPath;
    const string pattern = "^(?<name>.*)\\s\\d+\\.(?<extension>.*)$";
    
    public OverwriteAsset (string assetPath)
    {
        this.assetPath = assetPath;
        directoryName = Path.GetDirectoryName (assetPath);
        var match = Regex.Match (Path.GetFileName (assetPath), pattern);
        
        exists = match.Success;
        
        if (exists) {
            filename = match.Groups ["name"].Value;
            extension = match.Groups ["extension"].Value;
        }
    }
    
    public void Overwrite ()
    {
        FileUtil.ReplaceFile (assetPath, originalAssetPath);
        Delete ();
        AssetDatabase.ImportAsset (originalAssetPath);
    }
    
    public void Delete ()
    {
        AssetDatabase.DeleteAsset (assetPath);
    }
}
//}

//emlist[][cs]{
using UnityEditor;
using UnityEngine;

public class Overwriter : AssetPostprocessor
{

    const string message = "\"{0}.{1}\"という名前のアセットが既にこの場所にあります。アセットを置き換えますか？";

    static void OnPostprocessAllAssets (string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromPath)
    {
        if (Event.current == null || Event.current.type != EventType.DragPerform)
            return;

        foreach (var assetPath in importedAssets) {

            var asset = new OverwriteAsset (assetPath);

            if (asset.exists) {

                var overwriteMessage = string.Format (message, asset.filename, asset.extension);

                var result = EditorUtility.DisplayDialogComplex (asset.originalAssetPath, overwriteMessage, "置き換える", "両方とも残す", "中止");

                if (result == 0) {
                    asset.Overwrite ();
                } else if (result == 2) {
                    asset.Delete ();
                }

            }
        }
    }
}
//}