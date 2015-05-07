= SpriteAnimationPreview(スプライト一覧の表示)

//indepimage[frontispiece]
//pagebreak

== この章で得られる知識


== 本章で使用するソースコード

@<href>{https://gist.github.com/anchan828/1a95a3f6b88c81053582}

//table[t1][]{
ファイル	説明
------------------------
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

//emlist[][cs]{
[CustomPreview(typeof(AnimationClip))]
public class SpritePreview : ObjectPreview
{
}
//}

次に、インスペクターにプレビューを追加するには @<b>{HasPreviewGUI} と @<b>{GetPreviewTitle} を設定する必要があります。

//emlist[][cs]{
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

//emlist[][cs]{
public override void OnPreviewGUI(Rect r, GUIStyle background)
{
    GUI.Box(r, "表示領域");
}
//}

//image[ss05][赤線で囲まれた部分が使用可能な範囲]{

//}

さて、簡単にではありますが、カスタムプレビューの基礎となる機能を触ることが出来ました。次はAnimationClipに登録されているスプライトを表示していきます。

=={get_sprite} AnimationClipが参照しているスプライトを取得する

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

//emlist[][cs]{
EditorCurveBinding.PPtrCurve("", typeof(SpriteRenderer), "m_Sprite");
//}

これは、ルートのゲームオブジェクトにアタッチされているSpriteRendererコンポーネントの m_Sprite(Sprite)プロパティ、のことを指しています。

===  AnimationUtility.GetObjectReferenceCurve

AnimationClip と EditorCurveBinding を使用して Sprite の参照が格納されている ObjectReferenceKeyframe を取得するために AnimationUtility.GetObjectReferenceCurve を使用します。

//emlist[][cs]{
private Sprite[] GetSprites(AnimationClip animationClip)
{
    var sprites = new Sprite[0];
    
    if (animationClip != null)
    {
        var editorCurveBinding = EditorCurveBinding.PPtrCurve("", typeof(SpriteRenderer), "m_Sprite");
        
        var objectReferenceKeyframes = AnimationUtility.GetObjectReferenceCurve(animationClip, editorCurveBinding);
        
        sprites = objectReferenceKeyframes
           .Select(objectReferenceKeyframe => objectReferenceKeyframe.value)
           .OfType<Sprite>()
           .ToArray();
    }
    
    return sprites;
}
//}

== スプライトの表示（その1）

//image[ss07][各スプライトとスプライト名が表示されている]{

//}

スプライトを表示していきます。
限られた範囲の中で複数のスプライトを表示するには、1つのGUI要素に使用できる大きさ（Rect）を求めなければいけません。ですがそれは面倒なので @<b>{GUI.SelectionGrid} を使用しましょう。

//emlist[][cs]{
public override void OnPreviewGUI(Rect r, GUIStyle background)
{
    var sprites = GetSprites(target as AnimationClip);
    
    var guiContents = sprites
       .Select(s => new GUIContent(s.name, AssetPreview.GetAssetPreview(s)))
       .ToArray();
    
    GUI.SelectionGrid(r, -1, guiContents, 2, EditorStyles.whiteBoldLabel);
}
//}

GUI.SelectionGrid はかなり優れもので、@<b>{決められたRect値の範囲内で均等になるようにGUIContentを配置することが出来ます。}

== スプライトの表示（その2）

//image[ss08][Unity標準の表示機能でスプライトを表示]{

//}

@<img>{ss08}のような表示の仕方は見たことがあるかもしれません。この表示はスプライトを複数選択した時にプレビューとして表示されるものとほぼ同じものです。（@<img>{ss09}）

//image[ss09][スプライトを複数選択した状態]{

//}

今回はAnimationClipを選択した時に、@<b>{複数のスプライトを選択した}と判断させる実装を施します。

=== Initializeメソッド

CustomPreviewクラスには初期化を行うInitializeメソッドが存在します。オーバーライドが可能な作りになっているので少し手を加えます。

//emlist[][cs]{
public override void Initialize(Object[] targets)
{
    base.Initialize(targets);
    
    var sprites = new Object[0];
    
    foreach (AnimationClip animationClip in targets)
    {
        ArrayUtility.AddRange(ref sprites, GetSprites(animationClip));
    }
    
    m_Targets = sprites;
}
//}

通常の初期化を行った後、ターゲットとなるオブジェクトを差し替えます。@<code>{m_Targets}がプレビューで扱うUnityEngine.Objectの配列です。

あとは@<code>{OnPreviewGUI}を実装するだけです。@<code>{OnPreviewGUI}は@<code>{m_Targets}の要素の数だけ呼び出されます。OnPreviewGUIが呼び出された時の@<code>{m_Targets}の要素はそれぞれtargetに格納され、取得できる r (Rect) はプレビュー範囲全体ではなく既に計算された1要素分だけの範囲となります。

//image[ss10][m_Targetsに要素が1つの時と、6つの時の引数rで得られる範囲]{

//}

次にスプライトを描画しますが、GUIクラスにスプライトを描画する機能は存在しません。
そこで、オブジェクトに対してプレビュー用のテクスチャを生成＆取得できる@<b>{AssetPreview.GetAssetPreview}を使用します。

//emlist[][cs]{
public override void OnPreviewGUI(Rect r, GUIStyle background)
{
    var previewTexture = AssetPreview.GetAssetPreview(target);
    EditorGUI.DrawTextureTransparent(r, previewTexture);
}
//}

===[column] 初回だけプレビュー画像が表示されない





ここで @<code>{AssetPreview.GetAssetPreview} の特徴を掴んでおきましょう。GetAssetPreviewで作成するプレビュー用のテクスチャはキャッシュされています。もし、オブジェクトに対するテクスチャがキャッシュされていない場合（つまり初回）はテクスチャの生成を行うためにGetAssetPreviewは@<b>{null}を返します。nullを返した時のその状態が@<img>{ss11}です。

//image[ss11][テクスチャがnullなためスプライト名だけが表示されている]{

//}

すぐにインスペクターの再描画処理が走ればテクスチャは描画されますが、インスペクターウィンドウはインスペクターを操作しない限りは自動で再描画を行わないので、初回だけテクスチャが表示されないことに困る場面が出てくるかもしれません。

この問題を回避するには簡単で、プレビュー前にプレビュー用のテクスチャをキャッシュさせればいいので適当なところで @<code>{AssetPreview.GetAssetPreview} を実行します。

//emlist[][cs]{
public override void Initialize(Object[] targets)
{
    base.Initialize(targets);

    var sprites = new Object[0];

    foreach (AnimationClip animationClip in targets)
    {
        ArrayUtility.AddRange(ref sprites, GetSprites(animationClip));
    }

    // ここでスプライトのプレビュー用テクスチャを生成＆キャッシュさせる
    foreach (var sprite in sprites)
    {
        AssetPreview.GetAssetPreview(sprite);
    }

    m_Targets = sprites;
}
//}

これで、テクスチャがnullでプレビューが表示されないという問題は解決しました。

===[/column]
