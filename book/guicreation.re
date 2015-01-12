
= GUIを自作する


例えば@<b>{EditorGUILayout.ObjectField}でSpriteのフィールドを表示した時、Texture系であればプレビューがあるGUIに変化しますがSpriteはプレビューが実装されていません。

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

@<b>{プレビュー対応しているSpriteField}のような自分でGUIを作成する事が可能です。

これから話すことは以下のコードを見ながら読むと理解が深まるかもしれません。


@<href>{https://github.com/anchan828/unitejapan2014/blob/master/SpriteField/Assets/SpriteField/Editor/CustomEditorGUI.cs, CustomEditorGUI.cs - SpriteField}

== GUIを作る

IMGUI（Immediate Mode GUI - Unity4.5までのGUIシステムをこう呼びます）でランタイムでもエディタ上でも自由にGUIを作成することが可能です。
ですがNGUIやUnity4.6から搭載されるuGUIを触る人が多く、ほぼGUIを作成するのはエディタ拡張分野となってしまったため、GUIを作成する技術はそれほど認知されていません。

そこで今回はGUIを作成するときに必要最低限必要な知識を書いていきたいと思います。

実際にGUIを作成するにはいくつかの知識が必要です。

== GUIStyle

GUIStyleはGUIのスタイルを設定するもので皆さんは大抵最初の2つのパターンで使用しているかと思います。

=== 1. GUISkinの中に複数のGUIStyleを設定して使用する

これはGUISkinを作成し、既存スタイルを変更したりカスタムスタイルとして作成することができます。
そして、スクリプト側で@<code>{GUI.skin}に自分で作成したGUISkinを代入すると一度にスタイルが適用されます。

//image[ss02][自分で画像を用意すればボタンやトグルの画像を変更することができる][scale=0.5]{

//}

=== 2. GUIクラスの引数に指定してスタイルを変更する

これは、GUIを描画するときに既存のスタイルを使用せず引数として渡されたGUIStyleを使用します。

//emlist{
// GUIStyleはスタイル名で指定することも可能
GUILayout.Label ("ラベル", "box");
//}

=== 3. スタイルを描画する

2つのパターンを紹介しました。最後にもう1つあり、これがGUIを作成する上で必要な知識となります。

GUIStyleにはGUIStyle.DrawというAPIが存在します。これはGUISkinとして設定されたフォントやテクスチャ、文字の色を使用して描画を行うものです。
皆さんが普段目にしている@<code>{GUILayout.Button}のボタン画像も、<code>{GUILayout.Button}の内部でGUIStyle.Drawによって描かれています。


== Event

Eventクラスはすべてのイベントを管理するために使用されています。

GUIに関するイベントではボタンをマウスでクリックするとき

 * ボタンの上にマウスがある
 * クリックされた
 * ボタンが押しっぱなしである

などのイベントが発生し、このイベントを適切に処理することでボタンの動きを表現することができます。

Eventを使用するとき、例えば「ボタンを押した」というイベントをコードにすると以下のようになります。

//emlist{
if (Event.current.type == EventType.MouseDown) {

    ...ボタンを押した時の処理...

    // ボタン（マウス）を押したというイベントとして処理。以降はイベントは使用されない
    Event.current.Use();
}
//}

「ボタンを押した」というイベントを使用する場合、必ずEvent.Useを呼び出します。そうすると@<b>{今回のイベントは既に使用された}という状態になります。

Event.Useを呼び出した後はEvent.typeは@<b>{used}となります。こうすることにより、他のイベント処理は実行されなくなり他のイベントと競合することがなくなります。

== Control ID

すべてのGUIに対してコントロールIDというものが割り振られています。このIDによりそれぞれのGUIは独立したものとなります。
このIDを適切に割り振らないと、「GUI.Windowをマウスでドラッグした時に範囲選択ツールが動作してしまった」というような意図しないGUIの複数操作（競合？）をしてしまうことになります。


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

現在何のコントロールIDにフォーカスが当てられているかを知るには@<code>{GUIUtility.hotControl}を使用します。またはキーボードのフォーカスを知るには@<code>{GUIUtility.keyboardControl}となります。
また、GUIUtility.hotControlに代入することで強制的にコントロールIDを切り替えることが可能です。

== ObjectPicker

//indepimage[ss03][][scale=0.5]


オブジェクトピッカーは特定のオブジェクトやアセットを選択するためのウィンドウです。ObjectFieldのようなオブジェクトの参照を持つGUIの場合はこのオブジェクトピッカーがあるとユーザーが操作を行うときに便利になります。

オブジェクトピッカーの表示は@<code>{EditorGUIUtility.ShowObjectPicker}で行うことが可能です。

そして、オブジェクトピッカーで選択中のオブジェクト・選択されたオブジェクトを取得するにはEventのコマンド名(Event.commandname)が@<b>{ObjectSelectorUpdated}と@<code>{EditorGUIUtility.GetObjectPickerObject}を組み合わせて使用することになります。

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
DragAndDropではオブジェクトをドラッグする処理と、ドロップする処理の2つにわかれます。ObjectFieldのようなGUIを作成するときは、プロジェクトプラウザからドラッグする処理は既にUnity側で実装されているのでドロップする処理のみを実装すれば良いです。


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
このイベント中以外で描画を変更するような処理を加えた時、Unity側はエラーとなり正しくGUIが描画されなくなります。
これを回避するために@<code>{GUIUtility.ExitGUI}を呼び出し、以降のGUIの描画に関する処理は全て無視する事が可能です。
こうすることで次の「Layout」と「Repaint」処理が発行され、次は正しく描画されることになります。

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

== 最後に

おそらく普段Editor拡張を触っている人たちでもコントロールIDに関する部分は触らないことが多いと思います。難しかったでしょうか？
ここを理解すれば自作のGUIやEventクラスの動きについての理解が深まります。