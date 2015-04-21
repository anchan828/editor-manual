= SpriteAnimationPreview

//indepimage[frontispiece]
//pagebreak

== この章で得られる知識


== 本章で使用するソースコード

@<href>{https://gist.github.com/anchan828/1a95a3f6b88c81053582}

//table[t1][]{
ファイル	説明
------------------------
AnimationClipEditor.cs	カスタムエディタ
OverrideEditor.cs	Editorクラスのラッパー
SpriteAnimationClipPreview.cs	SpriteAnimationClipのカスタムプレビュー
//}

== 概要

スプライトを再生するアニメーションクリップを作成した場合、このクリップには@<b>{どのスプライトが登録されているか}を確認することはよくあります。

=== AnimationClipに登録されているスプライトの確認方法

AnimationClipに登録されているスプライトの確認方法はいくつかあります。

==== Animationウィンドウで確認する

これがおそらく正規で行える確認方法です。

//image[ss01][Window → Animationからウィンドウを表示することができる]{

//}

ですが、この方法だとアニメーションを行うゲームオブジェクトを選択しないとAnimationウィンドウでは確認することが出来ません。

==== AnimationClipをインスペクターのDebugモードで確認する

//image[ss02][Inspectorウィンドウ右上の ≡ ボタンを押してDebugを選択する]{

//}

すると、@<i>{Pptr Curve Mapping} というプロパティでスプライトを参照している配列を確認することが出来ます。このスプライトがAnimationClipで使用されているスプライトです。

=== もっと手軽にスプライトを確認する方法を確立する

本章では @<kw>{CustomPreview,カスタムプレビュー} を使用してインスペクター上で確認するようにします。

== CustomPreviewの作成

カスタムプレビューを作成するのは簡単です。作り方は、カスタムエディタを作成する方法と同じで、@<b>{ObjectPreview}を継承したクラスを作成し、@<b>{CustomPreview}の属性を付加します。

//emlist{
[CustomPreview(typeof(AnimationClip))]
public class SpritePreview : ObjectPreview
{
}
//}

次に、インスペクターにプレビューを追加するには @<b>{HasPreviewGUI} と @<b>{GetPreviewTitle} を設定する必要があります。

//emlist{
[CustomPreview(typeof(AnimationClip))]
public class SpritePreview : ObjectPreview
{
    private GUIContent previewTitle = new GUIContent("Sprites");

    public override bool HasPreviewGUI()
    {
        return true;
    }

    public override GUIContent GetPreviewTitle()
    {
        return previewTitle;
    }
}
//}

//image[ss03][カスタムプレビューが追加された状態。プレビュータイトルをクリックすことによって他のプレビューへと変更できる]{

//}

=== カスタムプレビューは複数持つことができる

カスタムエディタとは違い、1つのオブジェクトに対して複数のプレビューを持つことが出来ます。

イメージとしては、インスペクターに表示されているエディターオブジェクトには必ず「@<b>{デフォルトプレビュー}」というものが存在します。それらに付属するものとしてカスタムエディタは実装されます。その状態が「fireball」と「Sprites」が表示されている@<img>{ss03}です。

イメージはこのような感じ。

//image[ss04][デフォルトプレビューとカスタムプレビュー。複数のプレビューを持つことができる。]{

//}

=== プレビューの表示

プレビューを行うには  @<code>{OnPreviewGUI} を使用します。試しに下記のコードを追加してプレビューとして使用可能な範囲を確かめてみます。

//emlist{
public override void OnPreviewGUI(Rect r, GUIStyle background)
{
    GUI.Box(r, "表示領域");
}
//}

//image[ss05][赤線で囲まれた部分が使用可能な範囲]{

//}

さて、簡単にではありますが、カスタムプレビューの基礎となる機能を触ることが出来ました。次はAnimationClipに登録されているスプライトを表示していきます。

== AnimationClipが参照しているスプライトを取得する

まずはスプライトを取得します。

=== スプライトはObjectReferenceKeyframeで管理されている

スプライトなどのオブジェクトのアニメーションは @<b>{ObjectReferenceCurve} によって管理されています。そして、オブジェクト1つ1つは @<b>{ObjectReferenceKeyframe} によって管理されます。

===[column] ObjectReferenceKeyframe と Keyframe の違い
AnimationClipには ObjectReferenceKeyframe と Keyframe の2種類のキーフレームがあります。
普段良く目にするのは Keyframe の @<b>{Float型の値を保持するキーフレーム}です。それに対し、
ObjectReferenceKeyframe は @<b>{オブジェクトの参照を保持するキーフレーム}となります。

//image[ss06][Positionを操作するのはKeyframe、Spriteを操作するのはObjectReferenceKeyframe]{

//}

保持する値がFloatか参照かの違いはありますが、他は変わりありません。

===[/column]

ObjectReferenceKeyframe で参照されているスプライト、もう少し詳しく言うと、SpriteRendererのSpriteプロパティのスプライトをアニメーションするために参照されているスプライトを取得するためにはAnimationClip中にある幾つものアニメーションカーブから特定の アニメーションカーブ、つまり ObjectReferenceKeyframe を得なければいけません。そのための機能が @<b>{EditorCurveBinding} です。

=== EditorCurveBinding

EditorCurveBinding は特定のアニメーションカーブを取得するためのキーとなるものです。例えば、本章のようにSpriteRendererのSpriteをアニメーションさせたい場合は、以下になります。

//emlist{
EditorCurveBinding.PPtrCurve("", typeof(SpriteRenderer), "m_Sprite");
//}

これは、ルートのゲームオブジェクトにアタッチされているSpriteRendererコンポーネントの m_Sprite(Sprite)プロパティ、のことを指しています。

===  AnimationUtility.GetObjectReferenceCurve

AnimationClip と EditorCurveBinding を使用して Sprite の参照が格納されている ObjectReferenceKeyframe を取得するために AnimationUtility.GetObjectReferenceCurve を使用します。

//emlist{
private Sprite[] GetSprites(AnimationClip animationClip)
{
    var editorCurveBinding = EditorCurveBinding.PPtrCurve("", typeof(SpriteRenderer), "m_Sprite");

    var objectReferenceKeyframes = AnimationUtility.GetObjectReferenceCurve(animationClip, editorCurveBinding);

    var sprites = objectReferenceKeyframes
        .Select(objectReferenceKeyframe => objectReferenceKeyframe.value)
        .OfType<Sprite>()
        .ToArray();

    return sprites;
}
//}

== スプライトの表示