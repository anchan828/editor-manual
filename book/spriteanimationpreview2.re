= SpriteAnimationPreview(スプライトアニメーション)

//indepimage[frontispiece]

スプライトを再生するアニメーションクリップを作成した場合に、インスペクターのプレビュー画面でスプライトアニメーションの再生を行う仕組みを実装します。

//pagebreak


=== デフォルトのAnimationClipのプレビュー

AnimationClipのインスペクターは3Dモデルのためのアニメーションが基準となっており、プレビュー画面も3Dアニメーションしか再生できず、2Dアニメーションでは使用することができません。

//image[ss01][3Dアニメーションのプレビューがデフォルト][]{

//}

インスペクターをカスタマイズして、2D のアニメーションにも対応できるようにしていきます。

== カスタムエディター

既存のAnimationClipのカスタムエディター、@<code>{AnimationClipEditor}をオーバーライドする形で新たにカスタムエディターを作成します。
今回作成するカスタムエディターの名前は@<b>{SpriteAnimationClipEditor}とします。

まずは以下のようなクラスを作成します。複数選択した場合でも動作するように@<code>{CanEditMultipleObjects}属性をつけましょう。

//emlist{
using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(AnimationClip), CanEditMultipleObjects)]
public class SpriteAnimationClipEditor : Editor
{

}
//}

するとインスペクターの表示が@<img>{ss02}のようになります。

//image[ss02][インスペクターで表示できる全てのものが表示されている状態][]{

//}

これは既にUnity側のAnimationClipEditorで実装されていたOnInspectorGUIメソッドが適用されなくなったためです。カスタマイズされていない@<b>{素}の状態で表示されます。

=== インスペクターの表示はデフォルトのままにしたい

今回はプレビュー画面のみを変更したいのでOnInspectorGUIの部分が変更されてしまうのは不本意です。なので、メソッドをオーバーライドしない限りはベースとなるEditorオブジェクト（カスタムエディターで使用するもの）を流用するための@<b>{OverrideEditor}クラスを作成してみましょう。

//emlist{
#@warn(ココもうちょっと説明した方がいい気がする)
#@maprange(unityprojects/SpriteAnimationClip/Assets/Editor/OverrideEditor.cs,OverrideEditor)
#@end
}
//}

先ほど作成したSpriteAnimationClipEditorの派生クラスをEditorからOverrideEditorに変更します。

//emlist{
#@maprange(unityprojects/SpriteAnimationClip/Assets/Editor/SpriteAnimationClipEditor.cs,first)
#@end
}
//}

こうすることで従来通りの表示となります。

//indepimage[ss03][]

=== Spriteの取得

@<chap>{spriteanimationpreview1}の@<hd>{spriteanimationpreview1|get_sprite}と同じ実装でスプライトを取得します。

//emlist{
#@maprange(unityprojects/SpriteAnimationClip/Assets/Editor/SpriteAnimationClipEditor.cs,getSprites)
#@end
//}

この取得したスプライトをアニメーション再生ボタンを押すことで、プレビュー画面でスプライトアニメーションを行うようにします。

まずは1つのスプライトをプレビュー画面に表示してみましょう。

//emlist{
#@maprange(unityprojects/SpriteAnimationClip/Assets/Editor/SpriteAnimationClipEditor.cs,draw_sample_preview)
#@end
//}

//image[ss04][スプライトが表示されるようになった][]{

//}

=== スプライトアニメーションの再生ボタンを作成する

3Dの時と同じくプレビュー描画内のところに再生ボタンを作成すると見栄えは良くなりそうですが、今回は@<b>{OnPreviewSettings}内に再生ボタンを作成します。

//image[ss05][右上の部分がOnPreviewSettingsで実装できる部分][]{

//}

OnPreviewSettingsではGUILayoutを使用することができます。早速再生ボタンを表示してみましょう。再生ボタンは 再生 @<icon>{ss07} と一時停止 @<icon>{ss08} というように「オン/オフ」の状態が存在するので @<code>{GUILayout.Button}ではなく @<code>{GUILayout.Toggle}を使用します。

//emlist{
#@maprange(unityprojects/SpriteAnimationClip/Assets/Editor/SpriteAnimationClipEditor.cs,sample_OnPreviewSettings)
#@end
//}

//image[ss06][右端に再生ボタンができた][]{

//}

次は時間を管理する「TimeControl」クラスを作成します。

=== 時間を管理する「TimeControl」クラスを作成する

今回の用途にあった時間を管理するクラスを作成します。時間の更新処理は@<code>{EditorApplication.update}を使って行います。

//emlist{
#@maprange(unityprojects/SpriteAnimationClip/Assets/Editor/SpriteAnimationClipEditor.cs,TimeControl)
#@end
//}

==== 再生する

TimeControlを使用して再生を行うためのトリガーは以下のように実装します。

//emlist{
#@maprange(unityprojects/SpriteAnimationClip/Assets/Editor/SpriteAnimationClipEditor.cs,DrawPlayButton)
#@end
//}


==== 再生するスプライトを取得し表示する

現在どのスプライトを再生すべきかはAnimationClipの @<kw>{frameRate,フレームレート} とAnimationClipSettingsにあるstopTimeを使って導きます。

//emlist{
#@maprange(unityprojects/SpriteAnimationClip/Assets/Editor/SpriteAnimationClipEditor.cs,currentSpriteNum)
#@end
//}

Spriteの保持の仕方などは省いていますが、実際に使用すると以下のようになります。

//emlist{
#@maprange(unityprojects/SpriteAnimationClip/Assets/Editor/SpriteAnimationClipEditor.cs,OnInteractivePreviewGUI)
#@end
//}

==== 時間スピードを調整するスライダーを作成する

再生ボタンと同じ所に時間をn倍速させるスライダーを作成します。

//emlist{
#@maprange(unityprojects/SpriteAnimationClip/Assets/Editor/SpriteAnimationClipEditor.cs,DrawSpeedSlider)
#@end
//}

//image[ss09][再生ボタンの隣にスライダーが追加された][]{

//}


=== 完成

プレビュー画面でスプライトアニメーションの再生を確認できるようになりました。

//image[ss10][再生ボタンを押すとスプライトアニメーションが再生される][]{

//}

@<code>{CanEditMultipleObjects}を実装すれば複数のスプライトアニメーションを同時に再生可能です。

//indepimage[ss11][]


== さらに機能を追加していく

さらに改良し、Sprite Animation Preview@<fn>{1}	としてアセットストアで配布しています。配布するまでに行った細かな実装について説明していきます。

//indepimage[ss12]


=== AssetPreview.GetAssetPreview のテクスチャはサイズが小さい

元々は、@<code>{AssetPreview.GetAssetPreview} はプロジェクトビューなどで表示するための機能です。プロジェクトビューではそこまで高解像度なテクスチャサイズは必要としていません。なので強制的に 128x128 へとリサイズされます。

「Sprite Animation Preview」では、品質面でプレビューの表示に @<code>{AssetPreview.GetAssetPreview} を使用せず、@<b>{SpriteEditorから直接プレビュー画像を取得}するようにしました。

==== Editor オブジェクトを動的に作成

Editorオブジェクトは、任意のタイミングでユーザーが作成することができます。

Unity標準で使用されている Sprite オブジェクトのための @<code>{SpriteInspector} を作成します。

//emlist{
private List<Editor> GetSpriteEditors(params Sprite[] sprites)
{
    var type = Types.GetType("UnityEditor.SpriteInspector", "UnityEditor.dll");
    var editors = new List<Editor>();
                
	foreach (var sprite in sprites)
    {
		Editor _editor = Editor.CreateEditor(sprite, type);

        if (_editor != null)
            editors.Add(_editor);
    }

    return editors;
}
//}

==== プレビュー用テクスチャの取得

プレビューのテクスチャは @<b>{RenderStaticPreview} で取得することができます。

//emlist{
var editor = spriteEditors[i];
var previewTexture = editor.RenderStaticPreview("", null, 
                        (int)previewRect.width,
                        (int)previewRect.height);
//}

==== 動的に作成したオブジェクトの破棄を忘れないこと

必ず、作成した Editor オブジェクトを破棄するコードを実装するようにしてください。破棄するタイミングは @<b>{OnDisable} など、オブジェクトが必要なくなったタイミングで呼び出します。

//emlist{
public void OnDisable()
{
    foreach (var spriteEditor in spriteEditors)
    {
        Object.DestroyImmediate(spriteEditor);
    }
}
//}

//image[ss14][破棄しないと作成した Editor オブジェクトが残り続けてしまい、メモリを圧迫する]{

//}

また、プレビューで使用したテクスチャも破棄するようにします。

//image[ss13][テクスチャが残り続けてしまい、メモリを圧迫する]{

//}

//footnote[1][@<href>{https://www.assetstore.unity3d.com/jp/#!/content/37611}]