= SpriteAnimationPreview(スプライトアニメーション)

//indepimage[frontispiece]
//pagebreak

== この章で得られる知識

カスタムエディタ
プレビュー画面オーバーライド

== 本章で使用するソースコード

@<href>{https://gist.github.com/anchan828/1a95a3f6b88c81053582}

//table[t1][]{
ファイル	説明
------------------------
AnimationClipEditor.cs	カスタムエディタ
OverrideEditor.cs	Editorクラスのラッパー
SpriteAnimationPreview.cs	スプライトアニメーションを行うカスタムプレビュー
//}

== 概要

スプライトを再生するアニメーションクリップを作成した場合、スプライトアニメーションの再生を確認したいことがよくあります。
本章ではスプリトアニメーションの確認をインスペクターのプレビュー画面で行えるようにデフォルトのプレビュー画面をオーバーライドして実装します。

=== デフォルトのAnimationClipのプレビュー

AnimationClipのインスペクターは3Dアニメーションが基準となっており、プレビュー画面も3Dアニメーションしか再生できず2Dアニメーションでは使用することが出来ません。

//image[ss01][3Dアニメーションのプレビューがデフォルト][]{

//}

== カスタムエディタ

既存のAnimationClipのカスタムエディタ、AnimationClipEditorをオーバーライドする形で新たにカスタムエディタを作成します。
今回作成するカスタムエディタの名前は@<b>{SpriteAnimationClipEditor}とします。

まずは以下のようなクラスを作成します。

//emlist[][cs]{
using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(AnimationClip))]
public class SpriteAnimationClipEditor : Editor
{

}
//}

するとインスペクターの表示が@<img>{ss02}のようになります。

//image[ss02][インスペクターで表示できる全てのものが表示されている状態][]{

//}

これは既にUnity側のAnimationClipEditorで実装されていたOnInspectorGUIメソッドが適用されなくなったためです。カスタマイズされていない@<b>{素}の状態で表示されます。

=== インスペクターの表示はデフォルトのままにしたい

今回はプレビュー画面のみを変更したいのでOnInspectorGUIの部分が変更されてしまうのは不本意です。なので、メソッドをオーバーライドしない限りはベースとなるEditorオブジェクト（カスタムエディタで使用するもの）を流用するための@<b>{OverrideEditor}クラスを作成してみましょう。

//emlist[コードは@<href>{https://gist.github.com/anchan828/8bd4fb7f7460eea1630a,ここ}から入手][cs]{
#@warn(ココもうちょっと説明した方がいい気がする)
#@maprange(unityprojects/SpriteAnimationClip/Assets/Editor/OverrideEditor.cs,OverrideEditor)
#@end
//}

先ほど作成したSpriteAnimationClipEditorの派生クラスをEditorからOverrideEditorに変更します。

//emlist[][cs]{
#@maprange(unityprojects/SpriteAnimationClip/Assets/Editor/SpriteAnimationClipEditor.cs,first)
#@end
}
//}

こうすることで従来通りの表示となります。

//indepimage[ss03][]

=== Spriteの取得

@<chap>{spriteanimationpreview1}の@<hd>{spriteanimationpreview1|get_sprite}と同じ実装でスプライトを取得します。

//emlist[][cs]{
#@maprange(unityprojects/SpriteAnimationClip/Assets/Editor/SpriteAnimationClipEditor.cs,getSprites)
#@end
//}

この取得したスプライトをアニメーション再生ボタンを押すことで、プレビュー画面でスプライトアニメーションを行うようにします。

まずは1つのスプライトをプレビュー画面に表示してみましょう。

//emlist[][cs]{
#@maprange(unityprojects/SpriteAnimationClip/Assets/Editor/SpriteAnimationClipEditor.cs,draw_sample_preview)
#@end
//}

//image[ss04][スプライトが表示されるようになった][]{

//}

=== スプライトアニメーションの再生ボタンを作成する

3Dの時と同じくプレビュー描画内のところに再生ボタンを作成すると見栄えは良くなりそうですが、今回は@<b>{OnPreviewSettings}内に再生ボタンを作成します。

//image[ss05][右上の部分がOnPreviewSettingsで実装できる部分][]{

//}

OnPreviewSettingsではGUILayoutを使用することが出来ます。早速再生ボタンを表示してみましょう。

//emlist[][cs]{
#@maprange(unityprojects/SpriteAnimationClip/Assets/Editor/SpriteAnimationClipEditor.cs,sample_OnPreviewSettings)
#@end
//}

//image[ss06][右端に再生ボタンが出来た][]{

//}