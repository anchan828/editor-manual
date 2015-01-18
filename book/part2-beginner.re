= エディタ拡張で使用するフォルダ

//indepimage[frontispiece]


//lead{
エディタ拡張を行うためには知っておかなければならないことが2点あります。この章ではエディタ拡張の世界へ入る第一歩としてエディタ拡張で重要な「フォルダ」について説明します。
//}

//pagebreak

== Editorフォルダ

EditorフォルダはエディタAPIを使用するための特別なフォルダです。
通常エディタAPIは、ランタイム@<fn>{runtime}で動作することは出来ません。

試しに以下のコードをAssetsフォルダ直下に作成しビルドをしてみましょう。


//emlist{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript : MonoBehaviour
{
}
//}

//image[ss01][ビルドエラー。名前空間「UnityEditor」が見つからない]{

//}

これはスクリプトファイルのコンパイルによって自動生成される@<b>{Assembly-CSharp.dll}で@<b>{UnityEditor.dllの参照}を行っていないためです。

ですが、開発中に生成されるAssembly-CSharp.dllにはUnityEditor.dllの参照が行われているためスクリプトのコンパイルエラーは発生しません。@<b>{UnityEditor.dllの参照が行われないのはビルド時に生成されるAssembly-CSharp.dllのみ}ということを覚えておきましょう。この仕様を知らないと「突然ビルドが通らなくなった」原因になってしまいます。

//image[ss02][Monodevelopで見るとUnityEditor.dllの参照を行っている。]{

//}

さて、このままではエディタAPIを使用することが出来ません。ビルドごとにエディタAPIを使用しているスクリプトファイルを除去するということも考えられますがそれは手間です。

UnityEditorでは@<b>{Assembly-CSharp-Editor.dll}を生成し、エディタAPIとランタイムAPIの住み分けを行うことで解決しています。このAssembly-CSharp-Editor.dllはビルド時には含まれないのでビルドエラーも発生しません。

@<b>{Assembly-CSharp-Editor.dll}は@<b>{Editor}フォルダ内のスクリプトファイルを含めるよう設計されています。

Editorフォルダの場所は特に制限はありません。どこにでも@<b>{複数}作成できます。

 * Assets/Editor
 * Assets/MyFolder/Scripts/Editor
 * Assets/Standard Assets/Editor

//image[ss03][フォルダ名がEditorであれば場所は特に指定はない]{

//}

ただし、「Standard Assets」「Pro Standard Assets」「Plugins」フォルダ内にEditorフォルダを作成した場合は、スクリプトは@<b>{Assembly-CSharp-Editor-firstpass.dll}へとコンパイルされます。

//image[ss04][firstpassからはAssembly-CSharp-Editor.dllを参照できないので気をつける]{

//}

===[column] Editorフォルダに含めずに動作させる方法

ランタイムで動作するスクリプトと一緒にエディタAPIを記述することはよくあります。
その場合は@<b>{#define}を利用します。UnityEditor上のみで@<b>{UNITY_EDITOR}シンボルが定義されています。これにより、ビルド時のスクリプトコンパイルではUNITY＿EDITORが定義されていないので@<b>{#if UNITY_EDITOR}で囲ったコードは除去されます。

//emlist{
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
#endif

public class NewBehaviourScript : MonoBehaviour
{
}
//}

===[/column]

== Editor Default Resourcesフォルダ

「Resources」フォルダのようにエディタ拡張のみで使用するリソースを格納しておくフォルダです。「Editor Default Resources」フォルダ内にあるアセットは@<b>{EditorGUIUtility.Load}でアクセスすることが可能です。

//image[ss05][logo.pngの画像ファイルがある]{

//}

//emlist{
var tex = EditorGUIUtility.Load ("logo.png") as Texture;
//}


//footnote[runtime][ビルドして実機上で動かすことを指します]