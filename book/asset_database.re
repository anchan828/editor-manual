= AssetDatabase

AssetDatabase は、Unity エディター上で扱うアセットを管理するための重要なクラスです。オブジェクトをアセットとして保存したり、アセットの一覧を取得したり、
するための重要なクラスです。このクラスを知らなければ、満足にプロジェクト内のアセットを操作することができませんし、独自のアセットを作成することもできません。本章では、特に重要なアセットの保存について解説します。

== オブジェクトをアセットとして保存

アセットの作成自体は、とても簡単です。

//emlist[AnimationClip をメニューから作成する例]{
[MenuItem ("Assets/Create AnimationClip")]
static void Example ()
{
  //まだアセットとして存在しないオブジェクトを作成
  var clip = new AnimationClip ();

  //アセットとして保存
  //AssetDatabase で扱うパスはUnityプロジェクトから相対パスで指定できる
  AssetDatabase.CreateAsset (clip, "Assets/New AnimationClip.anim");
}
//}

これで、Project ウィンドウを見るとアセットが作成されていることがわかります。

//image[ss01][Assets フォルダー直下に保存された]{

//}


== アセットが更新されるタイミング

先ほど保存した AnimationClip のプロパティーをスクリプトで変更してみます。変更がインスペクターで見れる frameRate の値を変更します。

//emlist{
[MenuItem ("Assets/Change FrameRate")]
static void ChangeFrameRate ()
{
  //アセットパスからアセットをオブジェクトとしてロード
  var clip = AssetDatabase.LoadAssetAtPath<AnimationClip>
                                      ("Assets/New AnimationClip.anim");
  clip.frameRate++;
}
//}

//image[ss02][@<code>{Assets/Change FrameRate}を実行するごとにフレームレートが +1 されていく]{

//}

フレームレートを変更したら、アセットのシリアライズ設定を「Force Text」@<fn>{2}にしてアセットをテキストエディターで開いてみましょう。

//emlist{
%YAML 1.1
%TAG !u! tag:unity3d.com,2011:
--- !u!74 &7400000
AnimationClip:
　... 略 ...
  m_PPtrCurves: []
  m_SampleRate: 60 <- ここがフレームレート
  m_WrapMode: 0
//}

フレームレートを表す@<code>{m_SampleRate}が、変更したにも関わらず 60 のままであることがわかります。

この状態は、@<code>{AssetDatabase.LoadAssetAtPath} によってアセットからロードされたオブジェクトのプロパティーが変更されただけであり、アセットにはまだ変更点が適用されていない状態です。

Unity エディター上では、オブジェクトは（SerializedObjectとして）キャッシュされているため、「スクリプトや Unity エディター上で触る限りは」常に最新のデータを扱うことができます。ですが、この状態はまだアセットとして保存されていません。

アセットとして保存するには @<code>{AssetDatabase.SaveAssets} を使用します。この関数は、メニューの @<code>{File -> Save Project} と同じ機能です。@<code>{SaveAssets} を呼び出すと、@<b>{ダーティーフラグ}が立っているオブジェクト、かつ、アセットのオブジェクトである場合、アセットを更新します。@<fn>{1}

//emlist{
[MenuItem ("Assets/Change FrameRate")]
static void ChangeFrameRate ()
{
  //アセットパスからアセットをオブジェクトとしてロード
  var clip = AssetDatabase.LoadAssetAtPath<AnimationClip>
                                      ("Assets/New AnimationClip.anim");
  clip.frameRate++;

  //アセットを最新に更新
  AssetDatabase.SaveAssets ();
}
//}

上記のコードにより、変更が AnimationClip アセットに適用されます。

//emlist{
%YAML 1.1
%TAG !u! tag:unity3d.com,2011:
--- !u!74 &7400000
AnimationClip:
　... 略 ...
  m_PPtrCurves: []
  m_SampleRate: 62 <- ここが変更された
  m_WrapMode: 0
//}

== System.IO を使う

C# には標準で、主にファイルとフォルダーを扱う System.IO 名前空間が存在します。この名前空間を使ってファイル（アセット）を作成する場合は、少し注意が必要です。

Unityには本章で紹介している AssetDatabase クラスは、あくまで アセットと認識できるファイルのみ & Unity プロジェクトの Assets フォルダー内のみを管理します。よって、UnityEngine.Object 派生ではないファイルの作成や、 Assets フォルダー外にファイルを作成することは許可されていません。このときに System.IO が活躍します。

=== Unity の管理外の操作であることを把握する

AssetDatabase を使ったアセットの操作は、@<b>{アセットの状態を把握する}のをUnity が自動で行ってくれます。ですが、System.IO によってアセット操作をした場合は、Unity はアセットの状態を把握できなくなってしまいます。

試しに、下記のコードを実行してみます。

//emlist[System.IO 経由で TextAsset を作成]{
[MenuItem ("Assets/Create TextAsset")]
static void CreateTextAsset ()
{
  var text = "text";
  File.WriteAllText ("Assets/New TextAsset.txt", text);
}
//}

すると、Finder ではファイルが作成されますが、Unity エディター上の Project ウィンドウではアセットとして表示されていません。@<fn>{3}

//image[ss03][左が Finder、右が Project ウィンドウ。それぞれ、.meta と TextAsset がないことがわかる]{
//}

=== プロジェクトのリフレッシュとアセットのインポート

Unity エディターが、自動でアセットの状態を把握できないのであれば、ユーザー自身でUnity エディターにアセットの状態を把握させる実装を行わなくてはいけません。

アセットの状態を把握させる方法は、2 種類あります。@<code>{AssetDatabase.Refresh} と @<code>{AssetDatabase.ImportAsset} です。

=== AssetDatabase.Refresh

Refresh 機能は、Unity プロジェクトを最新の状態へ更新する機能です。Refreshによって、プロジェクト全体のファイル構成がチェックされます。そして、最新の構成と Unityエディターに差異があった場合は、該当するアセットを（再）インポートします。

//image[ss04][Refresh によってプロジェクト状態がチェックされた後、未インポートのアセットをインポートする]{
//}

//emlist{
[MenuItem ("Assets/Create TextAsset")]
static void CreateTextAsset ()
{
  var text = "text";
  File.WriteAllText ("Assets/New TextAsset.txt", text);

  //リフレッシュすることによりアセットがインポートされる
  AssetDatabase.Refresh ();
}
//}

=== AssetDatabase.ImportAsset

AssetDatabase.ImportAsset は、対象のアセットのみをインポートするときに使用します。（巨大なプロジェクトでやっと実感できるくらいだと思いますが）Refresh 機能は、プロジェクト全体の状態をチェックするため多少重たい動作になります。特定のアセットのみである場合は、リフレッシュ経由のインポートではなく、直接 AssetDatabase.ImportAsset を呼び出してインポート処理を実行します。

//emlist{
[MenuItem ("Assets/Create TextAsset")]
static void CreateTextAsset ()
{
  var path = "Assets/New TextAsset.txt";
  var text = "text";

  File.WriteAllText (path, text);

  //パスを指定してインポートする
  AssetDatabase.ImportAsset (path);
}
//}


=== FileUtil

System.IO クラスを使用してアセットを操作するときに、便利な FileUtil クラスがあります。ファイル操作を行うときには、このクラスをチェックしてください。

@<href>{http://docs.unity3d.com/ScriptReference/FileUtil.html}


== アセットはパスではなく GUID で管理されている

全てのアセットは、ユニークなIDで管理するために GUID で管理されています。特に強制的なものはありませんが、自作のアセットとしてファイル管理周りを作成する場合は、パスではなく GUID を保持すると想定外のバグが発生しなくなるかもしれません。

//image[ss05][Library/metadata では、GUID でアセット情報が管理されている]{
//}

//emlist[.meta ファイルに guid がある]{
fileFormatVersion: 2
guid: 615b87de8357a4cba95fa8a92033e831
timeCreated: 1441513548
licenseType: Pro
TextScriptImporter:
  userData:
  assetBundleName:
  assetBundleVariant:
//}


//footnote[1][AnimationClip.frameRate は値を設定すると自動でダーティーフラグがセットされます。]
//footnote[2][@<code>{Edit -> Project Settings -> Editor} にある @<b>{Asset Serialization} で変更できます。]
//footnote[3][Unity エディター自体のアプリケーションのフォーカスを外して、再度フォーカスされるとアセットが表示されます。これは、フォーカスによってエディターのリフレッシュ起動が働く仕様のためです。]
