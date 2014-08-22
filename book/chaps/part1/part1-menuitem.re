= MenuItem

//lead{
知らないとエディタ拡張では何も出来ないと言っても過言ではないほど重要なMenuItemについて解説します。この章ではMenuItemで出来ることを紹介していきます。「実際にどう使っていくか」というのはこの章以降で@<b>{嫌というほど}扱っていきます。
//}

== MenuItemとは

MenuItemは「Unityエディタの上側にあるメニューバーやコンテキストメニューに項目を追加するための機能」です。


//image[ss01][「独自に追加した〜」という部分が追加されたメニュー項目]{

//}

//image[ss02][「スクリプトに追加した項目」という部分が追加されたコンテキストメニュー項目]{

//}

== MenuItemの使いドコロ

メニューは何らかのトリガーとなっています。標準では

 * ゲームオブジェクトの生成 ( @<i>{GameObject/Create Empty} )
 * シーンの作成 ( @<i>{File/New Scene} )
 * ウィンドウの表示 ( @<i>{Window/Inspector} )
 
などがメニューとして既に登録されています。

MenuItemを使用して独自のメニューを追加するタイミングは@<b>{Editorスクリプトを実行する時}です。目的別に取り上げるなら

 * 独自ウィンドウの表示
 * AssetBundleの作成
 * アセットの作成
 * アセットのインポート設定変更

などが上げられます。これらを実行するための入口となるのがMenuItemで、す。

== MenuItemを使ってメニューを表示してみよう

まずは簡単にメニューを追加してみましょう。
MenuItemはAttributeとして提供されており、@<b>{static}メソッドに付加することで機能します。

//emlist{
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

//emlist{
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

//emlist{
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

MenuItemの第二引数に@<code>{isValidateFunction}が存在し、これは@<b>{メニューを表示した時に実行可能かどうかのチェックを行う}ためのものです。isValidateFunctionにtrueを設定することでメソッドはValidateメソッドになります。さらにValidateメソッドは戻り値がbool型のメソッドとなり、戻り値としてtrueを返すと実行可能、falseを返すと実行不可となります。

また、Validateメソッドは単独では動作しません。必ずisValidateFunctionがfalseのメソッドを用意する必要があります。isValidateFunctionのデフォルトがfalseなので引数を省略しても問題ありません。

//emlist{
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

== MenuItemにチェックを入れてみよう

#@# Menu.SetChecked

== ショートカットを実装してみよう

== 何かつくろう

=== Assets/Create/Folder/EditorやAssets/Create/Folder/Plugins/iOS
