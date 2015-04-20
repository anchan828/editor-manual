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