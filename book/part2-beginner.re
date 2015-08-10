= エディター拡張で使用するフォルダー


//lead{
エディター拡張を行うためには知っておかなければならないことが2点あります。この章ではエディター拡張の世界へ入る第一歩としてエディター拡張で重要な「フォルダー」について説明します。
//}

== Editorフォルダー

EditorフォルダーはエディターAPIを使用するための特別なフォルダーです。
通常エディターAPIは、ランタイム@<fn>{runtime}で動作することはできません。

試しに以下のコードをAssetsフォルダー直下に作成しビルドをしてみましょう。


//emlist{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript : MonoBehaviour
{
}
//}

//image[ss01][ビルドエラー。名前空間「UnityEditor」が見つからない]{

//}

これはビルド時に生成される@<b>{Assembly-CSharp.dll}では@<b>{UnityEditor.dllの参照}を行わないためです。

ですが、開発時のUnityエディターで生成されるAssembly-CSharp.dllにはUnityEditor.dllの参照が行われているためスクリプトのコンパイルエラーは発生しません。@<b>{UnityEditor.dllの参照が行われないのはビルド時に生成されるAssembly-CSharp.dllのみ}ということを覚えておきましょう。この仕様を知らないと「突然ビルドが通らなくなった」原因になってしまいます。

//image[ss02][Monodevelopで見るとUnityEditor.dllの参照を行っている。][scale=0.5]{

//}

さて、このままではエディターAPIを使用することができません。ビルドごとにエディターAPIを使用しているスクリプトファイルを除去するということも考えられますがそれは手間です。

UnityEditorでは@<b>{Assembly-CSharp-Editor.dll}を生成し、エディターAPIとランタイムAPIの住み分けを行うことで解決しています。このAssembly-CSharp-Editor.dllはビルド時には含まれないのでビルドエラーも発生しません。

@<b>{Assembly-CSharp-Editor.dll}は@<b>{Editor}フォルダー内のスクリプトファイルを含めるよう設計されています。

Editorフォルダーの場所は特に制限はありません。どこにでも@<b>{複数}作成できます。

 * Assets/Editor
 * Assets/MyFolder/Scripts/Editor
 * Assets/Standard Assets/Editor

//image[ss03][フォルダー名がEditorであれば場所は特に指定はない]{

//}

ただし、「Standard Assets」「Pro Standard Assets」「Plugins」フォルダー内にEditorフォルダーを作成した場合は、スクリプトは@<b>{Assembly-CSharp-Editor-firstpass.dll}へとコンパイルされます。

//image[ss04][firstpassが参照されているのが分かる][scale=0.5]{

//}

Assembly-CSharp-Editor からは firstpass を参照することができますが、逆に、@<b>{firstpass からは Assembly-CSharp-Editor.dll を参照できないので気をつけてください。}

===[column] Editorフォルダーに含めずに動作させる方法

ランタイムで動作するスクリプトと一緒にエディターAPIを記述することはよくあります。
その場合は@<b>{#define}を利用します。UnityEditor上のみで@<b>{UNITY_EDITOR}シンボルが定義されています。これにより、ビルド時のスクリプトコンパイルではUNITY_EDITORが定義されていないので@<b>{#if UNITY_EDITOR}で囲ったコードは除去されます。

//emlist{
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
#endif

public class NewBehaviourScript : MonoBehaviour
{
    void OnEnable ()
    {
        #if UNITY_EDITOR
        EditorWindow.GetWindow<ExampleWindow> ();
        #endif
    }
}
//}

===[/column]

== Editor Default Resourcesフォルダー

「Resources」フォルダーのようにエディター拡張のみで使用するリソースを格納しておくフォルダーです。「Editor Default Resources」フォルダー内にあるアセットは@<b>{EditorGUIUtility.Load}でアクセスすることが可能です。

//image[ss05][logo.pngの画像ファイルがある]{

//}

//emlist{
var tex = EditorGUIUtility.Load ("logo.png") as Texture;
//}


=== ビルトインのリソース

元々、Editor Default Resources フォルダーは、ビルトインのリソースを扱うためのフォルダーであり、ユーザーが「このフォルダーにリソースを置く」目的で使うものではありません。

このフォルダーの目的は「ビルトインのリソースを置き換える」目的で使います。

==== ビルトインのリソースをロードする

エディターで使用するビルトインのリソースは、1つのアセットバンドルにまとめられています。
確認のために、@<code>{EditorGUIUtility.GetEditorAssetBundle} でアセットバンドルを取得します。


//image[ss06][AssetBundle.GetAllAssetNames でパスを確認]{

//}

//emlist{
[InitializeOnLoadMethod]
static void GetBultinAssetNames ()
{
    var flags = BindingFlags.Static | BindingFlags.NonPublic;
    var info = typeof(EditorGUIUtility).GetMethod ("GetEditorAssetBundle", flags);
    var bundle = info.Invoke (null, new object[0]) as AssetBundle;

    foreach (var n in bundle.GetAllAssetNames()) {
        Debug.Log (n);
    }
}
//}

パスを確認したら後は @<code>{EditorGUIUtility.Load} でアセットをロードします。必ず、Load 関数でアセットをロードしてください。先ほど取得したアセットバンドルからロードしたものを使ってはいけません。

@<code>{EditorGUIUtility.Load} は、まず @<b>{Unity プロジェクトの「"Assets/Editor Default Resources/" + path」にアセットがあるか}探します。その時にアセットがあれば、そのアセットを使用し、なければビルトインのアセットバンドルからアセットをロードします。この仕様により、「ビルトインのリソースを置き換える」ことが可能です。




//footnote[runtime][ビルドして実機上で動かすことを指します]