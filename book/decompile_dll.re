= UnityEditor.dll の中身を見る

//indepimage[frontispiece]

//lead{
エディター拡張をしていると「標準機能と（ほぼ）同じものを作成したい」「内部の API を使いたい」「どのようなコードになっているのか知りたい」ということはよくあります。本章では、UnityEditor.dll をデコンパイルして得たソースコードをエディター拡張に役立てるための手順を紹介します。
//}

== 本章で使用するソフト

本章では、DLLをデコンパイルするためのソフトとして@<icon>{dotpeek}
 @<href>{https://www.jetbrains.com/decompiler/,DotPeek}@<fn>{1} を使用します。また、@<href>{http://ilspy.net/,ILSpy}@<fn>{2}でも同様のことが可能です。


== DotPeek で DLL を見る

最初に、DotPeek に DLL を登録します。

//image[ss01][UnityEditor.dll を DotPeek の Assembly Explorer にドラッグ&ドロップ]{
//}

そうすると、Namespaceを階層としたツリー構造が表示されます。適当なクラスをダブルクリックすると、デコンパイルが開始されます。

//image[ss04][ActiveEditorTracker クラスを見ることができる]{
//}

このように、簡単に UnityEditor.dll のソースコードを見ることが可能です。ただし、デコンパイルしたソースコードは十分取り扱いに注意してください。また、Unity の DLL 以外（つまりユーザーのソースコードを含むDLLなど）をデコンパイルする行為は控えたほうが賢明です。ソフトウェア使用許諾などを考慮し、自己責任でお願いします。

== DLL を .cs ファイルとしてエクスポート

また、デコンパイルしたソースコードを cs ファイルとして書き出すことができます。

//image[ss02][DLL を選択した状態で @<code>{File -> Export to Project…} を選択]{
//}

保存先フォルダーを指定します。このとき、プロジェクトとして開くことができるように .sln ファイルも作成することができます。

//image[ss03][キャプション]{
//}

デコンパイルしたものは Monodevelop や  Visual Studio で .sln を開いて見る事ができます。

//image[ss05][UnityEditor の名前空間をエクスプローラーで見た図]{
//}


筆者はいつも、 Atom の検索機能を使って知りたい API などを探したり Unity 内部の動きを勉強しています。
Monodevelop や VisualStudio のほうがソースコードの検索性に優れているかもしれませんが、特に決めつける必要もないので自分の得意なエディターで行ってください。

//indepimage[ss06]

== デコンパイルを利用したエディター拡張

ここで例として、公式 API としては用意されていない「Final Gather」へのアクセスするまでの手順を紹介します。

//image[ss07][Lighting ウィンドウにある Final Gather]{
//}

=== 1. Lighting ウィンドウのクラスを探す

大抵のウィンドウのクラス名は「ウィンドウ名 + Window」となっています。なので「LightingWindow」で検索を行います。

=== 2. プロパティー名で検索する

次に、プロパティー名「FinalGather」で検索します。この時、プロパティーに空白がある場合は、削除してください（ Final Gather ->  FinalGather）。スペースのないほうがヒット率が上がります。すると LightingWindow クラスの中で以下のコードがヒットします。

//emlist[BakedGUI メソッド]{
SerializedProperty property7 = 
    so.FindProperty("m_LightmapEditorSettings.m_FinalGather");
//}

これにより、BakedGUI メソッド内にて SerializedObject 経由で m_FinalGather を取得していることがわかりました。次は、どのオブジェクトの SerializedObject なのかを把握します。

=== 3. 使われ方を知る

BakedGUI メソッドが使用されている場所を探します。BakedGUI で検索すると EnlightenBakeSettings メソッドで使用されていることがわかりました。

//emlist[EnlightenBakeSettings メソッド]{
private void EnlightenBakeSettings()
{
  SerializedObject so = 
      new SerializedObject(LightmapEditorSettings.GetLightmapSettings());
  SerializedProperty property1 = 
      so.FindProperty("m_GISettings.m_EnableRealtimeLightmaps");
  SerializedProperty property2 = 
      so.FindProperty("m_GISettings.m_EnableBakedLightmaps");
  this.RealtimeGUI(so, property1, property2);
  this.BakedGUI(so, property1, property2);
  this.GeneralSettingsGUI(so, property1, property2);
  so.ApplyModifiedProperties();
}
//}

LightmapEditorSettings.GetLightmapSettings() が目的のオブジェクトであることがわかりました。これをどうにか取得すれば目的が達成できそうです。

=== 4. 使う

しかし、GetLightmapSettings は internal なメソッドであるためアクセスすることができません。

//emlist[LightmapEditorSettings クラス]{
[WrapperlessIcall]
[MethodImpl(MethodImplOptions.InternalCall)]
internal static extern UnityEngine.Object GetLightmapSettings();
//}

そこで、リフレクションを使って解決します。

//emlist{
var flags =  BindingFlags.NonPublic | BindingFlags.Static;
var getLightmapSettings = 
    typeof(LightmapEditorSettings).GetMethod("GetLightmapSettings", flags);
var lightmapSettings = getLightmapSettings.Invoke(null, null) as Object;
var so = new SerializedObject(lightmapSettings);
so.Update();
var finalGatherProperty = so.FindProperty("m_LightmapEditorSettings.m_FinalGather");

//あとは好きなように
finalGatherProperty.boolValue = false;

so.ApplyModifiedProperties();
//}

これで、通常はアクセス出来ないプロパティーへとアクセスすることができました。

== エディター拡張でデコンパイル・リフレクションを使うこと

エディター拡張において、凝ったことをするのであれば、デコンパイルとリフレクションを行うのは必然と筆者は考えます。ソースコード以上に仕様を把握できるものはないですし、Unityが内部で使っているものと同じものが使えるのであれば、品質も保証されます。
「積極的に」とは言えませんが、本章で紹介したことを実践してみるとプロジェクトのワークフローが劇的に改善するかもしれません。

//footnote[1][DotPeek: @<href>{https://www.jetbrains.com/decompiler/}]
//footnote[2][ILSpy: @<href>{http://ilspy.net/,ILSpy}]
