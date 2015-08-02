= MenuItem

//indepimage[frontispiece]


//lead{
知らないとエディター拡張では何も出来ないと言っても過言ではないほど重要なMenuItemについて解説します。この章ではMenuItemで出来ることを紹介していきます。「実際にどう使っていくか」というのはこの章でも軽く説明はしますがこの章以降で@<b>{嫌というほど}扱っていきますので自然と理解していくでしょう。
//}

//pagebreak

== MenuItemとは

MenuItemは「Unityエディターの上側にあるメニューバーやコンテキストメニューに項目を追加するための機能」です。


//image[ss01][「独自に追加した〜」という部分が追加されたメニュー項目]{

//}

//image[ss02][「スクリプトに追加した項目」という部分が追加されたコンテキストメニュー項目]{

//}

== MenuItemの使いドコロ

メニューは何らかのトリガーとなっています。標準では

 * ゲームオブジェクトの生成 ( @<i>{GameObject/Create Empty} )
 * シーンの作成 ( @<i>{File/New Scene} )
 * ウインドウの表示 ( @<i>{Window/Inspector} )
 
などがメニューとして既に登録されています。

MenuItemを使用して独自のメニューを追加するタイミングは@<b>{Editorスクリプトを実行する時}です。目的別に取り上げるなら

 * 独自ウインドウの表示
 * AssetBundleの作成
 * アセットの作成
 * アセットのインポート設定変更

などが上げられます。これらを実行するための入口となるのがMenuItemです。

== MenuItemを使ってメニューを表示してみよう

まずは簡単にメニューを追加してみましょう。
MenuItemはAttributeとして提供されており、@<b>{static}メソッドに付加することで機能します。

//emlist[][cs]{
using UnityEditor;

public class NewBehaviourScript
{
    [MenuItem("CustomMenu/Example")]
    static void Example ()
    {
    }
}
//}

//image[ss03][CustomMenuのメニュー項目とExampleの子メニュー項目が追加された]{

//}


もちろん既存のメニューに子メニューを追加することも出来ます。

//emlist[][cs]{
using UnityEditor;

public class NewBehaviourScript
{
    [MenuItem("Assets/Example")]
    static void Example ()
    {
    }
}
//}

//image[ss04][AssetsメニューにExampleメニューを追加][scale=0.4]{

//}

子メニューから更に子メニューを作成することも可能です。

//emlist[][cs]{
using UnityEditor;

public class NewBehaviourScript
{
    [MenuItem("CustomMenu/Example/Child1/Grandchild")]
    static void Example1 ()
    {
    }

    [MenuItem("CustomMenu/Example/Child2/Grandchild")]
    static void Example2 ()
    {
    }
}

//}

//image[ss05][階層の深さは特に制限はありませんが操作性を考えて3階層位までにしましょう]{

//}

== 実行できないMenuItemを作成しよう

MenuItemで追加したメニューが@<b>{実行されてしまうと都合が悪い}場合もあります。そこでMenuItemで追加したメニューを実行できないようにする機能があります。


//image[ss06][Child2メニューの文字が灰色になっており実行できないようになっている]{

//}

MenuItemの第2引数に@<code>{isValidateFunction}が存在し、これは@<b>{メニューを表示した時に実行可能かどうかのチェックを行う}ためのものです。isValidateFunctionにtrueを設定することでメソッドはValidateメソッドになります。さらにValidateメソッドは戻り値がbool型のメソッドとなり、戻り値としてtrueを返すと実行可能、falseを返すと実行不可となります。

また、Validateメソッドは単独では動作しません。必ずisValidateFunctionがfalseのメソッドを用意する必要があります。isValidateFunctionのデフォルトがfalseなので引数を省略しても問題ありません。

//emlist[][cs]{
using UnityEditor;

public class NewBehaviourScript
{
    [MenuItem("CustomMenu/Example/Child1")]
    static void Example1 ()
    {
    }

    // isValidateFunctionがfalse
    [MenuItem("CustomMenu/Example/Child2")]
    static void Example2 ()
    {
    }

    // isValidateFunctionがtrue
    [MenuItem("CustomMenu/Example/Child2", true)]
    static bool ValidateExample2 ()
    {
        return false; // 今回はfalse固定
    }
}
//}

== MenuItemの表示順を変更してみよう

メニューの表示順を指定することが可能です。MenuItemの第3引数@<code>{priority}で指定します。

//image[ss07][Exampleメニューが一番下から一番上に移動している][scale=0.7]{

//}

//emlist[][cs]{
using UnityEditor;

public class NewBehaviourScript
{
    [MenuItem("Assets/Example", false, 1)]
    static void Example ()
    {
    }
}
//}

=== priorityの仕様

priorityの数値を小さく設定するほど上部に表示されるようになります。

//image[ss08][priorityが左から@<b>{1}, @<b>{20}, @<b>{40}, @<b>{1000}の時]{

//}

またpriorityの値を@<b>{11飛ばし}にするとメニュー項目の間にセパレート（区切り線）を設けることが出来ます。

//image[ss09][Example2とExample3の間に区切り線ができている]{

//}

//emlist[][cs]{
using UnityEditor;

public class NewBehaviourScript
{
    [MenuItem("CustomMenu/Example1", false, 1)]
    static void Example1 ()
    {
    }

    [MenuItem("CustomMenu/Example2", false, 2)]
    static void Example2 ()
    {
    }

    [MenuItem("CustomMenu/Example3", false, 13)]
    static void Example3 ()
    {
    }
}
//}




== MenuItemにチェックを入れてみよう

@<code>{Menu.GetChecked}と@<code>{Menu.SetChecked}を使用することで子メニューにチェックを入れることが出来ます。

//image[ss10][チェックをつけることで@<b>{有効中}であることがわかる]{

//}

//emlist[][cs]{
using UnityEditor;

public class NewBehaviourScript
{
    [MenuItem("CustomMenu/Example")]
    static void Example ()
    {
        var menuPath = "CustomMenu/Example";
        var @checked = Menu.GetChecked (menuPath);
        Menu.SetChecked (menuPath, !@checked);
    }
}
//}

== ホットキー（ショートカットキー）を実装してみよう

MenuItemで追加したメニューはホットキーで実行することが可能です。
第1引数のメニューパスの最後に「半角スペース + 修飾子キー + 任意の文字」の文字列をつけることにより実装できます。

//emlist[][cs]{
using UnityEditor;
using UnityEngine;

public class NewBehaviourScript
{
    // command(ctrl) + shift + g で実行
    [MenuItem("CustomMenu/Example %#g")]
    static void Example ()
    {
        Debug.Log ("実行されました");
    }
}
//}

//table[shortcut][修飾子キーを表す特殊文字]{
特殊文字				修飾＆特殊キー
----------------------------------------
%					Ctrl(Windows)または command(MacOSX)
#					Shift
&					Alt(Windows)または option(Mac OS X)
_					修飾子キー無し
F1 ... F12			ファンクションキー
HOME				Homeキー
END					Endキー
PGUP				PageUpキー
PGDN				PageDownキー
KP0 ... KP9			0から9までの数字キー
KP.					..
KP+					+
KP-					-
KP*					*
KP/					/
KP=					=
//}


== CONTEXT

各コンポーネントの歯車で表示されるコンテキストメニューにメニュー項目を追加することが出来ます。

//image[ss11][歯車をクリックすると表示されるコンテキストメニュー]{

//}

表示するには決まりがあり、メニューパスの冒頭に「@<b>{CONTEXT/}」を追加します。
そして「@<b>{CONTEXT/コンポーネント名/メニュー名}」とすることでコンテキストメニューに表示されるようになります。

//emlist[][cs]{
using UnityEditor;

public class NewBehaviourScript
{
    // Transformにメニューを追加
    [MenuItem("CONTEXT/Transform/Example1")]
    static void Example1 ()
    {
    }

    // コンポーネント（全て）にメニューを追加
    [MenuItem("CONTEXT/Component/Example2")]
    static void Example2 ()
    {
    }

    // ExampleScript（スクリプト）にメニューを追加
    [MenuItem("CONTEXT/ExampleScript/Example3")]
    static void Example3 ()
    {
    }
}
//}

コンテキストメニューに対してもホットキーは適用できますが、コンテキストメニューが表示されていないと実行されません。

=== MenuCommand

コンテキストメニューのみ@<code>{MenuCommand}を引数としてコンポーネント情報を取得することが出来ます。

//emlist[][cs]{
using UnityEditor;
using UnityEngine;

public class NewBehaviourScript
{
    [MenuItem("CONTEXT/Transform/Example1")]
    static void Example1 (MenuCommand menuCommand)
    {
        // 実行したTransformの情報が取得できる
        Debug.Log (menuCommand.context);
    }
}
//}

//image[ss12][上記コードをMain CameraのTransformで実行したもの]{

//}


== 何かつくろう

=== EditorフォルダやResourcesフォルダを作成する

//image[ss13][]{

//}

//emlist[][cs]{
using UnityEditor;
using UnityEngine;
using System.IO;

public class NewBehaviourScript
{
    [MenuItem("Assets/Create/TemplateFolder/Editor", false, 1)]
    static void CreateEditorFolder ()
    {
        CreateFolder ("Editor");
    }

    [MenuItem("Assets/Create/TemplateFolder/Resources", false, 2)]
    static void CreateResourcesFolder ()
    {
        CreateFolder ("Resources");
    }

    [MenuItem("Assets/Create/TemplateFolder/Scripts", false, 3)]
    static void CreateScriptsFolder ()
    {
        CreateFolder ("Scripts");
    }

    [MenuItem("Assets/Create/TemplateFolder/Prefabs", false, 4)]
    static void CreatePrefabsFolder ()
    {
        CreateFolder ("Prefabs");
    }
    
    [MenuItem("Assets/Create/TemplateFolder/Shaders", false, 5)]
    static void CreateShadersFolder ()
    {
        CreateFolder ("Shaders");
    }

    static void CreateFolder (string name)
    {
        string path = "Assets";

        if (Selection.activeObject != null) {
            var assetPath = AssetDatabase.GetAssetPath (Selection.activeObject);
            path = Directory.Exists (assetPath) ? assetPath : Path.GetDirectoryName (assetPath);
        }
    
        AssetDatabase.CreateFolder (path, name);
    }

}
//}
