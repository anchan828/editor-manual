= ReorderbleList


//indepimage[ss01]


ReorderbleList は、インスペクターやEditorWindowで表示されるリストのプロパティーをさらにビジュアライズしたものです。要素の並び替えや、要素の追加方法のカスタマイズなど幅広いことが行えるようになります。

この機能は公式としてドキュメント化されておらず非推奨扱いとなっていますが、Animator や Tag and Layer の設定画面で長い間使われているので、すぐに使えなくなることはないと思います。


== ReorderbleList の使い方

ReorderbleList は IList の派生クラス、または Array や List の SerializedProperty を使用して GUI を描画します。

//emlist{
using UnityEngine;

public class Example : MonoBehaviour
{
    [SerializeField]
    string[] texts;
}
//}

//emlist{
using UnityEngine;
using UnityEditor;
using UnityEditorInternal;

[CustomEditor (typeof(Example))]
public class ExampleInspector : Editor
{
    ReorderableList reorderableList;

    void OnEnable ()
    {
        reorderableList = new ReorderableList (serializedObject,
                                 serializedObject.FindProperty ("texts"));
    }

    public override void OnInspectorGUI ()
    {
        reorderableList.DoLayoutList ();
    }
}
//}

これで GUI が表示されるようになりました。


//indepimage[ss02]

ですがこのまま要素を追加しても@<img>{ss03} のように「Element (数字)」という要素の名前のみ表示されてしまいます。

//image[ss03][右下の + ボタンを押すと要素を追加できる]{

//}

== カスタマイズ

要素の名前ではなく、値を表示するには、カスタマイズを行わなくてはいけません。

=== 要素の描画

要素を変更するには @<code>{drawElementCallback} を使います。
描画する rect、何番目の要素かの index、状態を表す isActive と isFocused を扱うことができます。

rect が描画の最大範囲なので、少し小さくします。

//emlist{
var prop = serializedObject.FindProperty ("texts");

reorderableList = new ReorderableList (serializedObject, prop);

reorderableList.drawElementCallback = (rect, index, isActive, isFocused) => {
    var element = prop.GetArrayElementAtIndex (index);
    rect.height -= 4;
    rect.y += 2;
    EditorGUI.PropertyField (rect, element);
};
//}

//indepimage[ss04]

=== プロパティーの保存

最後にプロパティーの変更されたプロパティーの適用・保存をします。

//emlist{
public override void OnInspectorGUI ()
{
    serializedObject.Update ();
    reorderableList.DoLayoutList ();
    serializedObject.ApplyModifiedProperties ();
}
//}


これで扱えるようになりました。これが最小で扱える構成となっています。

== ヘッダーの描画

ヘッダーは @<code>{drawHeaderCallback} にデリゲートを登録することで実装が可能です。

//emlist{
reorderableList = new ReorderableList (serializedObject, prop);

reorderableList.drawHeaderCallback = (rect) =>
                     EditorGUI.LabelField (rect, prop.displayName);
//}

== フッターの描画はカスタマイズしない

drawFooterCallback がありますが、右下にある「+」「-」ボタンのためにあるので使用しないでください。

== 各要素の背景

各要素の背景を描画するためのコールバックです。ただ、GUI.backgroundColor で背景を変更しようとすると「+」「-」ボタンにまで影響してしまうため注意が必要です。

//indepimage[ss05]

//emlist{
reorderableList.drawElementBackgroundCallback = (rect, index, isActive, isFocused) => {
    GUI.backgroundColor = Color.yellow;
};
//}

想定している使われ方は GUIStyle.Draw を使う方法です。試しに miniButton を描画してみます。

//indepimage[ss06]

//emlist{
reorderableList.drawElementBackgroundCallback = (rect, index, isActive, isFocused) => {
    if (Event.current.type == EventType.Repaint) {
        EditorStyles.miniButton.Draw (rect, false, isActive, isFocused, false);
    }
};
//}

drawElementBackgroundCallback は使い方が難しく、カスタマイズはせずに基本は触らない方がいいかもしれません。

== 要素の追加

onAddCallback を使用することで、「+」ボタンなどを押した時に要素を追加する流れを自分で実装できます。

//indepimage[ss07]

//emlist{
reorderableList.onAddCallback += (list) => {

    //要素を追加
    prop.arraySize++;

    //最後の要素を選択状態にする
    list.index = prop.arraySize - 1;

    // 追加した要素に文字列を追加する（配列が string[] 前提）
    var element = prop.GetArrayElementAtIndex (list.index);
    element.stringValue = "New String " + list.index;
};
//}

== 「+」ボタンにドロップダウンメニューの追加

//indepimage[ss08]

//emlist{
reorderableList.onAddDropdownCallback = (Rect buttonRect, ReorderableList list) => {

    var menu = new GenericMenu ();

    menu.AddItem (new GUIContent ("Example 1"), false, () => {
    });
    menu.AddSeparator ("");

    menu.AddDisabledItem (new GUIContent ("Example 2"));

    menu.DropDown(buttonRect);

};
//}

== 要素を並び替えた直後のコールバック

//emlist{
reorderableList.onReorderCallback = (list) => {
    // 何かしら配列プロパティーに絡んだ要素の更新
    Debug.Log ("onReorderCallback");
};
//}

== まだまだ他にも

全てを説明してもいいのですが、説明する程でもない分かりやすいコールバックが用意されているので必要に合わせて使ってみてください。

 * 要素を削除してもいいか判断する @<b>{onCanRemoveCallback}
 * 配列に何かしらの変化(要素の並び替え、追加、削除...)があった時に呼び出される @<b>{onChangedCallback}
 * @<b>{要素を並び替えず}クリックした後に呼び出される @<b>{onMouseUpCallback}
 * 配列から要素を削除した時に呼び出される @<b>{onRemoveCallback}
 * 要素を選択した時に呼び出される @<b>{onSelectCallback}

== 要素の GUI をカスタマイズ

このようなクラスがあったとします。

//emlist{
[Serializable]
public class Character
{
    [SerializeField]
    Texture icon;

    [SerializeField]
    string name;

    [SerializeField]
    int hp;

    [SerializeField]
    int power;
}
//}

このままだと@<img>{ss09}の様な表示になってしまいます。

//image[ss09][foldout の表示になっている]{

//}


そこで @<b>{PropertyDrawer} を使って要素の描画を変更します。PropertyDrawer 自体の説明については @<chapref>{property_drawer} をご覧ください。
要素の GUI を drawElementCallback 内でカスタマイズしてもいいのですが、汎用性を考えてできるだけ PropertyDrawer を使用するようにしましょう。

//image[ss10][PropertyDrawer でカスタマイズ。その1]{

//}


//emlist[ゲームオブジェクトにアタッチする Example コンポーネント]{
using UnityEngine;

public class Example : MonoBehaviour
{
  [SerializeField]
  Character[] characters;
}
//}

//emlist[ReorderbleList の要素として描画する Character]{
using UnityEngine;
using System;

[Serializable]
public class Character
{
  [SerializeField]
  Texture icon;

  [SerializeField]
  string name;

  [SerializeField]
  int hp;

  [SerializeField]
  int power;
}
//}
//emlist[Character の PropertyDrawer]{
using UnityEngine;
using UnityEditor;

[CustomPropertyDrawer (typeof(Character))]
public class CharacterDrawer : PropertyDrawer
{
  private Character character;


  public override void OnGUI (Rect position,
    SerializedProperty property, GUIContent label)
  {
    //元は 1 つのプロパティーであることを示すために PropertyScope で囲む
    using (new EditorGUI.PropertyScope (position, label, property)) {

      //サムネの領域を確保するためにラベル領域の幅を小さくする
      EditorGUIUtility.labelWidth = 50;

      position.height = EditorGUIUtility.singleLineHeight;

      var halfWidth = position.width * 0.5f;

      //各プロパティーの Rect を求める
      var iconRect = new Rect (position) {
        width = 64,
        height = 64
      };

      var nameRect = new Rect (position) {
        width = position.width - 64,
        x = position.x + 64
      };

      var hpRect = new Rect (nameRect) {
        y = nameRect.y + EditorGUIUtility.singleLineHeight + 2
      };

      var powerRect = new Rect (hpRect) {
        y = hpRect.y + EditorGUIUtility.singleLineHeight + 2
      };

      //各プロパティーの SerializedProperty を求める
      var iconProperty = property.FindPropertyRelative ("icon");
      var nameProperty = property.FindPropertyRelative ("name");
      var hpProperty = property.FindPropertyRelative ("hp");
      var powerProperty = property.FindPropertyRelative ("power");

      //各プロパティーの GUI を描画
      iconProperty.objectReferenceValue =
        EditorGUI.ObjectField (iconRect,
          iconProperty.objectReferenceValue, typeof(Texture), false);

      nameProperty.stringValue =
        EditorGUI.TextField (nameRect,
          nameProperty.displayName, nameProperty.stringValue);

      EditorGUI.IntSlider (hpRect, hpProperty, 0, 100);
      EditorGUI.IntSlider (powerRect, powerProperty, 0, 10);

    }
  }
}
//}

//emlist[Example の CustomEditor]{
using UnityEngine;
using UnityEditor;
using UnityEditorInternal;

[CustomEditor (typeof(Example))]
public class ExampleInspector : Editor
{
  ReorderableList reorderableList;

  void OnEnable ()
  {
    var prop = serializedObject.FindProperty ("characters");

    reorderableList = new ReorderableList (serializedObject, prop);
    reorderableList.elementHeight = 68;
    reorderableList.drawElementCallback =
      (rect, index, isActive, isFocused) => {
        var element = prop.GetArrayElementAtIndex (index);
        rect.height -= 4;
        rect.y += 2;
        EditorGUI.PropertyField (rect, element);
    };

    var defaultColor = GUI.backgroundColor;

    reorderableList.drawHeaderCallback = (rect) =>
      EditorGUI.LabelField (rect, prop.displayName);

  }

  public override void OnInspectorGUI ()
  {
    serializedObject.Update ();
    reorderableList.DoLayoutList ();
    serializedObject.ApplyModifiedProperties ();
  }
}
//}
