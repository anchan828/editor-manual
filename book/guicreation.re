
= GUIを自作する

@<b>{注: 本章はObjectFieldがSpriteに対応していない時に執筆されたものです。なので目的となる「Sprite に対応したフィールドの作成」は必要のない拡張になってしまいますが、その過程で使用する技術はためになるので掲載しています。}

例えば@<b>{EditorGUILayout.ObjectField}でSpriteのフィールドを表示した時、Texture系であればプレビュー形式のGUIに変化しますが、Spriteはプレビュー形式にはなりません。（注: 最新バージョンのUnityではプレビュー形式になります）

//image[ss01][Texture2DとSpriteのGUIを並べたもの][scale=0.5]{

//}

//emlist{
void OnGUI ()
{
    GUILayoutOption[] options = new [] {
        GUILayout.Width (96),
        GUILayout.Height (96)
    };
    EditorGUILayout.ObjectField (null, typeof(Texture2D), false, options);
    EditorGUILayout.Space ();
    EditorGUILayout.ObjectField (null, typeof(Sprite), false, options);
}
//}

@<b>{プレビュー形式に対応したSpriteField}を自作してみましょう。


== GUIを作る

IMGUI@<fn>{imgui}でランタイムでもエディター上でも自由にGUIを作成することが可能です。
ですがNGUIやUnity4.6から搭載されるuGUIを触る人が多く、ほぼ GUI クラスを使って GUI を作成するのは、エディター拡張をする時のみとなってしまいました。なので、GUI クラスにある@<code>{IntField}のようなGUIフィールドを作成する技術はそれほど認知されていません。

そこで今回はGUIフィールドを作成するときに必要な、最低限の知識を紹介していきます。

//footnote[imgui][Immediate Mode GUI - Unity4.5まで主流だったGUIシステムをこう呼びます]

== GUIStyle

GUIStyleはGUIのスタイルを設定するもので皆さんは大抵最初の2つのパターンで使用しているかと思います。

=== 1. GUISkinの中に複数のGUIStyleを設定して使用する

これはGUISkinを作成し、既存スタイルを変更したりカスタムスタイルとして作成できます。
そして、スクリプト側で@<code>{GUI.skin}に自分で作成したGUISkinを代入すると一度にスタイルが適用されます。

//image[ss02][自分で画像を用意すればボタンやトグルの画像を変更できる][scale=0.5]{

//}

=== 2. GUIクラスの引数に指定してスタイルを変更する

これは、GUIを描画するときに既存のスタイルを使用せず引数として渡されたGUIStyleを使用します。

//emlist{
// GUIStyleはスタイル名で指定することも可能
GUILayout.Label ("ラベル", "box");
//}

=== 3. スタイルを描画する

2つのパターンを紹介しました。最後にもう1つあり、これがGUIを作成する上で必要な知識となります。

GUIStyleにはGUIStyle.DrawというAPIが存在します。これは GUIStyle に設定されたフォントやテクスチャ、文字の色を使用して描画を行うものです。
皆さんが普段目にしている@<code>{GUILayout.Button}のボタン画像も、@<code>{GUILayout.Button}の内部でGUIStyle.Drawが呼び出されています。


== Event

Eventクラスはすべてのイベントを管理するための機能です。

GUIに関するイベントは、さまざまな種類があり、例えばボタンをマウスでクリックするとき

 * ボタンの上にマウスがある
 * クリックされた
 * ボタンが押しっぱなしである

のイベントが発生します。この時に、イベントを適切に処理することによってボタンの動きを表現できます。

例えば「ボタンを押した」というイベントをコードにした時は、以下のようになります。

//emlist{
if (Event.current.type == EventType.MouseDown) {

    ...ボタンを押した時の処理...

    // ボタンを押した（使用済み）として処理。
    Event.current.Use();
}
//}

「ボタンを押した」というイベントを使用する場合、必ずEvent.Useを呼び出します。そうすると@<b>{今回のイベントは既に使用された}という状態になります。

Event.Useを呼び出した後はEvent.typeは@<b>{used}となります。こうすることにより、他のイベント処理は実行されなくなり、他のイベントと競合することがなくなります。

== Control ID

すべてのGUIに対してコントロールIDというものが割り振られています。このIDによりそれぞれのGUIは独立したものとなります。
このIDを適切に割り振らないと、「GUI.Windowをマウスでドラッグした時に範囲選択ツールが動作してしまった」というような意図しないGUIの複数操作（競合）をしてしまうことになります。


=== コントロールIDの生成/取得

//emlist{
int id = GUIUtility.GetControlID(FocusType.Passive, rect);
//}

この場合はキーボード以外のフォーカスを受け付け、特定の範囲(rect)内のコントロールIDを生成/取得しています。

=== コントロールIDと描画部分の紐付け

//emlist{
EditorStyles.objectFieldThumb.Draw(rect, content, id);
//}

GUIStyle.DrawにコントロールIDを渡すことによって、スタイルの描画は渡したコントロールIDによって制御されることになります。

=== 現在フォーカス中のコントロールID

現在、どのコントロールIDにフォーカスが当てられているかを知るには@<code>{GUIUtility.hotControl}を使用します。またはキーボードのフォーカスを知るには@<code>{GUIUtility.keyboardControl}となります。
また、GUIUtility.hotControlに代入することで強制的にコントロールIDを切り替えることが可能です。

== ObjectPicker

//indepimage[ss03][][scale=0.5]


オブジェクトピッカーは特定のオブジェクトやアセットを選択するためのウィンドウです。ObjectFieldのようなオブジェクトの参照を持つGUIの場合はこのオブジェクトピッカーがあるとユーザーが操作を行うときに便利になります。

オブジェクトピッカーの表示は@<code>{EditorGUIUtility.ShowObjectPicker}で行うことが可能です。

そして、オブジェクトピッカーで選択中のオブジェクト・選択されたオブジェクトを把握するために、Eventのコマンド名(Event.commandname)が@<b>{ObjectSelectorUpdated}と@<code>{EditorGUIUtility.GetObjectPickerObject}を組み合わせて使用することになります。

//emlist{
// コマンド名がObjectSelectorUpdatedでオブジェクトピッカーが
// 現在コントロール中のGUIによるものであった場合
if (evt.commandName == "ObjectSelectorUpdated" 
    && id == EditorGUIUtility.GetObjectPickerControlID())
{
    // オブジェクトピッカーで選択中のオブジェクトを取得
    sprite = EditorGUIUtility.GetObjectPickerObject() as Sprite;
    // GUIを再描画
    HandleUtility.Repaint();
}

if (GUI.Button(buttonRect, "select", EditorStyles.objectFieldThumb.name + "Overlay2"))
{
    // 現在のコントロールIDに対するオブジェクトピッカーを表示する
    EditorGUIUtility.ShowObjectPicker<Sprite>(sprite, false, "", id);
    // オブジェクトピッカーを表示するイベントを発行したのでイベントのUseを実行する
    evt.Use();
}
//}


== DragAndDrop

ObjectFieldのようにドラッグ＆ドロップしてオブジェクトの参照を格納したい場合があります。その時にはDragAndDropを使用します。
DragAndDropではオブジェクトをドラッグする処理と、ドロップする処理の2つにわかれます。ObjectFieldと似たGUIを作成するときは、ヒエラルキーやプロジェクトブラウザーでは既にドラッグする処理が実装済みなので、GUIへドロップする処理のみを実装すればよいです。


//emlist{
// マウス位置がGUIの範囲内であれば
if (rect.Contains(evt.mousePosition))
{
    switch (evt.type)
    {
        // ドラッグ中
        case EventType.DragUpdated:
        case EventType.DragPerform:

            // ドラッグしているのが参照可能なオブジェクトの場合
            if (DragAndDrop.objectReferences.Length == 1)
        
                // オブジェクトを受け入れる
                DragAndDrop.AcceptDrag();
        
            // ドラッグしているものを現在のコントロールIDと紐付ける
            DragAndDrop.activeControlID = id;
        
            // このオブジェクトを受け入れられるという見た目にする
            DragAndDrop.visualMode = DragAndDropVisualMode.Generic;
            break;
        
        // ドラッグ終了 = ドロップ
        case EventType.DragExited:
        
            // ドロップしているのが参照可能なオブジェクトの場合
            if (DragAndDrop.objectReferences.Length == 1)
            {
                var reference = DragAndDrop.objectReferences[0] as Sprite;
                if (reference != null)
                {
                    sprite = reference;
                    HandleUtility.Repaint();
                }
            }
            break;
    }
}
//}

== HandleUtility.Repaint

@<code>{HandleUtility.Repaint}は現在のビューを再描画するものです。

現在の「ビュー」を少し言い換えると、「EditorWindowやScriptableWizardなどGUIを描画するウィンドウ」となり、それらの再描画になります。

もっと詳しく言い換えると「GUIViewを継承しているビュー」を再描画します。

機能的には@<code>{EditorWindow.Repaint}と変わりません。


プレビュー表示のようなGUIの見た目を変更する場合、何も対策を行わなければEditorWindowなどのGUIViewが再描画処理に頼ることになりGUIは最新の描画を保つことができません。これをGUI側から再描画をリクエストするのが@<code>{HandleUtility.Repaint}となります。

まとめると、再描画処理が行われるのは以下の3パターンです。

 * GUIView上でEventのRepaintが発行された時
    * GUIView上にマウスを載せる
    * GUIViewにフォーカスを当てる
 * GUIViewのRepaintをリクエストした時
    * EditorWindow.Repaintなど
 * HandleUtility.Repaintを発行した時


== GUIUtility.ExitGUI

GUIでは描画に関するEventを2種類発行しています。GUIのレイアウトを構成する「Layout」と再描画を行う「Repaint」です。
これらのイベント以外で描画を変更するような処理を加えた場合、GUIのレイアウトが崩れたとこになりUnity側はエラーと判断し、GUIが正しく描画されなくなります。

この現象は、「GUI関数の引数で使う要素を配列で保持しているときに、特定の要素を配列から削除」するときによく起こります。

これを回避するためには、@<code>{GUIUtility.ExitGUI}を呼び出し、以降のGUIの描画に関する処理は全て無視する事ができます。
こうすることで問題なく次フレームの「Layout」と「Repaint」処理が発行され、エラーは発生しません。

== GUILayout対応

GUILayoutの仕組みは@<code>{GUILayoutUtility.GetRect}でRect情報を取得し描画することになります。

ですが一番手っ取り早い方法があります。

//emlist{
public static Sprite SpriteField(Sprite sprite, params GUILayoutOption[] options)
{
    EditorGUILayout.LabelField("", "", options);
    var rect = GUILayoutUtility.GetLastRect();
    return CustomEditorGUI.SpriteField(rect, sprite);
}
//}

このように既に実装されているGUILayoutのAPIを発行した後、@<code>{GUILayoutUtility.GetLastRect}を行うことで簡単にRect情報を取得することが可能です。

これで、@<code>{SpriteField} を作成するための知識が揃いました。細かな説明を加えながらコードを組み立てていきましょう。


== SpriteField 関数を作成する



まずは @<code>{CustomEditorGUI} クラスを作成し、その中に SpriteField のメソッドを追加します。

//emlist{
using UnityEditor;
using UnityEngine;

public class CustomEditorGUI
{
    public static Sprite SpriteField (Rect rect, Sprite sprite)
    {
    }
}
//}

つぎに、@<code>{GUIStyle.Draw} を使って背景を描画します。@<code>{GUIStyle.Draw} は EventType.Repaint の時のみ実行なので気をつけてください。

//emlist{
using UnityEditor;
using UnityEngine;

public class CustomEditorGUI
{
    public static Sprite SpriteField (Rect rect, Sprite sprite)
    {
        var evt = Event.current;
        if (evt.type == EventType.Repaint) {
            // サムネ形式の背景を描画
            EditorStyles.objectFieldThumb.Draw (rect, GUIContent.none, id, false);

            if (sprite) {
                // スプライトからプレビュー用のテクスチャを取得
                var spriteTexture = AssetPreview.GetAssetPreview (sprite);

                if (spriteTexture) {
                    spriteStyle.normal.background = spriteTexture;
                    // スプライトを描画
                    spriteStyle.Draw (rect, false, false, false, false);
                } 
            }
        }
    }
}
//}

つぎはドラッグ＆ドロップの処理です。 GUI 上にマウスがあるときのみ処理を行うので @<code>{Rect.Contains} でマウスの位置を監視します。

//emlist{
using UnityEditor;
using UnityEngine;

public class CustomEditorGUI
{
    public static Sprite SpriteField (Rect rect, Sprite sprite)
    {
        var evt = Event.current;
        
        // ... 背景描画のコードは略 ...


        if (rect.Contains (evt.mousePosition)) {
        
            switch (evt.type) {
        
            case EventType.DragUpdated:
            case EventType.DragPerform:
        
                if (DragAndDrop.objectReferences.Length == 1)
                    DragAndDrop.AcceptDrag ();
        
                DragAndDrop.visualMode = DragAndDropVisualMode.Generic;
        
                break;
            case EventType.DragExited:
        
                if (DragAndDrop.objectReferences.Length == 1) {
                    var reference = DragAndDrop.objectReferences [0] as Sprite;
                    if (reference != null) {
                        sprite = reference;
                        HandleUtility.Repaint ();
                    }
                }
                break;
            }
        }
    }
}
//}


つぎに、描画範囲に対してのコントロールIDを取得します。今回は Tab キーを押すことで別の GUI 要素へ移動する実装を行います。その時には、FocusType.Keyboard を使って ID を作成します。


//emlist{
using UnityEditor;
using UnityEngine;

public class CustomEditorGUI
{
    public static Sprite SpriteField (Rect rect, Sprite sprite)
    {
          var id = GUIUtility.GetControlID (FocusType.Keyboard, rect);
    }
}
//}

また、Drawの引数にコントロールIDが使用できます。これは、ドラッグ&ドロップと関係があります。現在ドラッグ中で、マウスが GUI 上にある場合は on は true となり、@<b>{GUI にフォーカスがあたっている状態}を表現することができます。

//emlist{
var on = DragAndDrop.activeControlID == id;
EditorStyles.objectFieldThumb.Draw (rect, GUIContent.none, id, on);

.
.
.

switch (evt.type)
{
case EventType.DragUpdated:
case EventType.DragPerform:
    DragAndDrop.activeControlID = id;
//}

最後に オブジェクトピッカー を実装します。


まずは、オブジェクトピッカーを表示するための小さなボタンを右下に表示します。

ただ、ボタンを表示するのではなく、キーボード操作でもボタンを押せるようにします。

//emlist{
var buttonRect = new Rect (rect);
// 加工
buttonRect.x += buttonRect.width * 0.5f;
buttonRect.width *= 0.5f;
buttonRect.y += rect.height - 16;
buttonRect.height = 16;

// キーをおした時
// エンターキーである時
// そして操作しているのがこの GUI であるとき
var hitEnter = evt.type == EventType.KeyDown 
            && (evt.keyCode == KeyCode.Return || evt.keyCode == KeyCode.KeypadEnter) 
            && EditorGUIUtility.keyboardControl == id;

// ボタンをおした時、またはエンターキーを押した時にオブジェクトピッカーを表示
if (GUI.Button (buttonRect, "select", EditorStyles.objectFieldThumb.name + "Overlay2")
     || hitEnter) {
    // どの GUI で表示したか判断できるようにコントロールIDを渡す
    EditorGUIUtility.ShowObjectPicker<Sprite> (sprite, false, "", id);
    evt.Use ();
    GUIUtility.ExitGUI ();
}
//}

最後にオブジェクトピッカーで選択したものを取得して実装は完了です。

//emlist{
// オブジェクトピッカーがこの GUI のためのものであるか判断
if (evt.commandName == "ObjectSelectorUpdated"
    && id == EditorGUIUtility.GetObjectPickerControlID ()) {
   
    sprite = EditorGUIUtility.GetObjectPickerObject () as Sprite;

    // 描画するスプライトが変更されたので再描画
    HandleUtility.Repaint ();
}
//}

GUILayout ようにもクラスを作成します。

//emlist{
public class CustomEditorGUILayout
{
    public static Sprite SpriteField (Sprite sprite, params GUILayoutOption[] options)
    {
        EditorGUILayout.LabelField ("", "", options);
        var rect = GUILayoutUtility.GetLastRect ();
        return CustomEditorGUI.SpriteField (rect, sprite);
    }
}
//}

作成したものは以下のように使用します。

//emlist{
using UnityEditor;
using UnityEngine;

public class NewBehaviourScript : EditorWindow
{
    [MenuItem("Window/SpriteEditor")]
    static void Open()
    {
        GetWindow<NewBehaviourScript>();
    }

    private Sprite sprite1,sprite2;
    void OnGUI()
    {
        sprite1 = CustomEditorGUI.SpriteField(new Rect(134, 1, 128, 128), sprite1);
        sprite2 = CustomEditorGUILayout.SpriteField(sprite2, 
                                      GUILayout.Width(128), GUILayout.Height(128));
    }
}
//}

//indepimage[ss04]