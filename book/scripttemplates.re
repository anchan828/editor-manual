= ScriptTemplates


EditorWindowを生成するスクリプトを作成する時、みなさんはどうしていますか？

ここで、長くなりますが「EditorWindowを作成してGUI描画部分を実装する準備が整うまで」を順番に説明していきます。


 * NewBehaviourScript.csを作成

//emlist[][cs]{
using UnityEngine;
using System.Collections;

public class NewBehaviourScript : MonoBehaviour {

    // Use this for initialization
    void Start () {

    }

    // Update is called once per frame
    void Update () {

    }
}
//}

 * StartとUpdateメソッド、System.Collectionsを消して...

//emlist[][cs]{
using UnityEngine;

public class NewBehaviourScript : MonoBehaviour {
}
//}


 * @<code>{using UnityEditor;}を付けて...

//emlist[][cs]{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript : MonoBehaviour {
}
//}

 * @<code>{EditorWindow}を継承するようにして...

//emlist[][cs]{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript : EditorWindow {
}
//}

 * EditorWindowを開くためにMenuItemを追加して...

//emlist[][cs]{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript : EditorWindow
{
    [MenuItem("Window/NewBehaviourScript")]
    static void Open ()
    {

    }
}
//}

 * GetWindowとOnGUIを追加する

//emlist[][cs]{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript : EditorWindow
{
    [MenuItem("Window/NewBehaviourScript")]
    static void Open ()
    {
        GetWindow<NewBehaviourScript> ();
    }

    void OnGUI ()
    {

    }
}
//}

ここまでやって、やっとEditorWindowのGUI部分を作成する準備が整いました。
実際に手を動かすときは手順が少なく感じますが、MenuItemの追加がかなり手間と感じます。


 * @<b>{必ず}EditorWindowを継承したい
 * @<b>{必ず}MenuItemを追加したい
 * @<b>{必ず}OnGUIを追加したい

というような場合、@<b>{ScriptTemplates}を作成することで解決できます。

== ScriptTemplatesとは

スクリプトファイルを作成する時には、テンプレートファイルと呼ばれるものをベースにスクリプトファイルを作成します。
皆さんがよく使う、メニューの@<code>{Assets/Create/C# Script}もテンプレートファイルを使用してNewBehaviourScript.csが作成されています。


==={place} テンプレートファイルが保存されている場所

テンプレートファイルは、以下の場所に保存されています。

//table[t6][ビルトインのScriptTemplates]{
OS	保存場所
-----------------
Mac	/Applications/Unity/Unity.app/Contents/Resources/ScriptTemplates
Windows	C:\Program Files\Unity\Editor\Data\Resources\ScriptTemplates
//}

//image[ss01][]{
//}

=== テンプレートファイルの中身

@<code>{81-C# Script-NewBehaviourScript.cs.txt}の中身を見てみましょう。

//emlist[][cs]{
using UnityEngine;
using System.Collections;

public class #SCRIPTNAME# : MonoBehaviour {

    // Use this for initialization
    void Start () {

    }

    // Update is called once per frame
    void Update () {

    }
}
//}

@<code>{#SCRIPTNAME#}というのがあるのがわかります。これはUnity上でスクリプト名を決定した時に置換するためのパラメータとなっています。
他にも専用のパラメータが存在します。全てのパラメータを@<table>{t2}にまとめます。

//table[t2][ScriptTemplatesで使用できるパラメータ]{
パラメータ名	説明
-----------------
#NAME#	そのままのスクリプトファイル名（拡張子なし）
#SCRIPTNAME#	スクリプト名。スペースは削除されます（拡張子なし）@<br>{}例: New Script -> NewScript
#SCRIPTNAME_LOWER#	スクリプト名。スペースは削除されます（拡張子なし）@<br>{}もし入力した最初の文字が小文字の場合先頭に「my」をつけます@<br>{}例: new Script -> myNewScript
//}

== テンプレートファイルの作成

テンプレートのファイル名には規則があります。この規則は必ず守るようにしましょう。

@<code>{{priority\}-{メニュー名\}-{ファイル名\}.{拡張子\}.txt}

//table[t3][パラメータの説明]{
パラメータ名	説明
-----------------
priority	メニューの表示順。値が高いほどメニューの下側に表示されます
メニュー名	Unityで表示するメニュー名になるもの
ファイル名	新規作成時にデフォルトで設定されているファイル名
拡張子	作成したいファイルの拡張子
//}

例として、EditorWindowを作成するスクリプトテンプレートは以下になります。

作成するファイル:@<br>{}
@<code>{121-C# EditorWindow Script-NewEditorWindow.cs.txt}

//emlist[][cs]{
using UnityEngine;
using UnityEditor;

public class #SCRIPTNAME# : EditorWindow
{
    [MenuItem("Window/#SCRIPTNAME#")]
    static void Open()
    {
        GetWindow<#SCRIPTNAME#> ();
    }

    void OnGUI ()
    {

    }
}
//}

== テンプレートファイルの配置

テンプレートファイルを配置できる場所は2箇所あります。

1つはUnityがインストールされている場所

//table[t5][ビルトインのScriptTemplates]{
OS	保存場所
-----------------
Mac	/Applications/Unity/Unity.app/Contents/Resources/ScriptTemplates
Windows	C:\Program Files\Unity\Editor\Data\Resources\ScriptTemplates
//}

もう1つは、Unityプロジェクトの@<code>{Assets/ScriptTemplates}です。

今回は@<code>{Assets/ScriptTemplates}に配置してみることにしましょう。

//image[ss02][]{

//}

配置した後は、@<b>{必ずUnityエディターを再起動します。}このScriptTemplatesが読み込まれるのはUnityエディターの起動時のみだからです。

再起動した後、@<code>{Assets/Create}メニューに自作したテンプレートファイルのメニューが追加されているのがわかります。

//image[ss03][「C# EditorWindow Script」が追加されている]{

//}

実際にテンプレートを使ってスクリプトを作成してみましょう。

//image[ss04][スクリプトのデフォルト名が「NewEditorWindow」となっている]{

//}

デフォルト名「NewEditorWindow」で作成するとスクリプトの中身は以下になります。

//emlist[][cs]{
using UnityEngine;
using UnityEditor;

public class NewEditorWindow : EditorWindow
{
    [MenuItem("Window/NewEditorWindow")]
    static void Open()
    {
        GetWindow<NewEditorWindow> ();
    }

    void OnGUI ()
    {

    }
}
//}

== メニューの表示位置

メニューの位置はpriorityにより決まります。priorityについて詳しくは@<chapref>{part1-menuitem}を御覧ください。


//image[ss05][@<code>{Assets/Create}の各priority]{

//}

//table[t4][@<code>{Assets/Create}の各priority]{
パラメータ名	priority
-----------------
Folder	53
C# Script	81
Javascript	82
Shader	83
//}

priorityが86のC# Scriptが2つありますが、仕様としてpriorityが低いものがテンプレートとして優先されます。

自作のテンプレートとして表示する時には、デフォルトのテンプレートの近くにあったほうがいいので、priorityは@<b>{64-70}、または@<b>{95-190} が好ましいです。

== ProjectWindowUtil との組み合わせ

ProjectWindowUtil クラスの @<code>{StartNameEditingIfProjectWindowExists} と組み合わせると @<code>{Assets/ScriptTemplates} にテンプレートファイルを置かずとも、スクリプトを作成できるようになります。詳しくは @<chapref>{projectWindowUtil} をご覧ください。
