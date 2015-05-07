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
public abstract class OverrideEditor : Editor
{
    readonly MethodInfo methodInfo = typeof(Editor).GetMethod("OnHeaderGUI", BindingFlags.NonPublic | BindingFlags.Instance);

    private Editor m_BaseEditor;
    protected Editor baseEditor
    {
        get { return m_BaseEditor ?? (m_BaseEditor = GetBaseEditor()); }
        set { m_BaseEditor = value; }
    }

    protected abstract Editor GetBaseEditor();

    public override void OnInspectorGUI()
    {
        baseEditor.OnInspectorGUI();
    }
	
	// ... 以下 DrawPreview、GetInfoString、OnPreviewSettings というようにカスタムエディタで使用できるメソッド群が列挙されている
}
//}

先ほど作成したSpriteAnimationClipEditorの派生クラスをEditorからOverrideEditorに変更します。

//emlist[][cs]{
using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(AnimationClip))]
public class SpriteAnimationClipEditor : OverrideEditor
{
    protected override Editor GetBaseEditor()
    {
        Editor editor = null;
        var baseType = Types.GetType("UnityEditor.AnimationClipEditor", "UnityEditor.dll");
        CreateCachedEditor(targets, baseType, ref editor);
        return editor;
    }
}
//}

こうすることで従来通りの表示となります。

//indepimage[ss03][]

=== Spriteの取得

@<chap>{spriteanimationpreview1}の@<hd>{spriteanimationpreview1|get_sprite}と同じ実装でスプライトを取得します。

//emlist[][cs]{
#@maprange(unityprojects/SpriteAnimationClip/Assets/Editor/SpritePreview.cs, getSprites)
#@end
//}
