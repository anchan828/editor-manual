= EditorGUI (EdirotGUILayout)

//indepimage[frontispiece]


//lead{
この機能を知らなくてはエディタ拡張では何も出来ないと言っても過言ではありません。この章ではEditorGUIやEditorGUILayoutで出来ることを説明していきます。サンプルコードは簡略化の関係でEditorGUILayoutを使用することが多くなります。
//}

//pagebreak

== EditorGUIとは

ランタイム側にあるGUIクラスと役割は同じですがEditor向けに機能が追加されているクラスです。
逐次配置されるEditorGUILayoutクラスも用意されています。

まずは簡単にEditorWindowの上にEditorGUIで文字を表示してみましょう。

//image[ss01][EditorGUIの第一歩]{

//}

//emlist[][cs]{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript : EditorWindow
{
    [MenuItem("Window/Example")]
    static void Open ()
    {
        GetWindow<NewBehaviourScript> ();
    }

    void OnGUI ()
    {
        EditorGUILayout.LabelField ("Example Label");
    }
}
//}




== EditorGUI.BeginChangeCheck / EndChangeCheck

BeginChangeCheckとEndChangeCheckでの間でGUIが使用されている時、GUIに何らかの変更が生じた場合にEndChangeCheckがtrueを返します。

//emlist[][cs]{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript : EditorWindow
{
    [MenuItem("Window/Example")]
    static void Open ()
    {
        GetWindow<NewBehaviourScript> ();
    }

    bool toggleValue;

    void OnGUI ()
    {
        EditorGUI.BeginChangeCheck ();

        // toggleをマウスでクリックして値を変更する
        toggleValue = EditorGUILayout.ToggleLeft ("Toggle", toggleValue);

        if (EditorGUI.EndChangeCheck ()) {

            // toggleValueの値が変更されるたびに呼び出される

            if (toggleValue) {
                Debug.Log ("toggleValueがtrueになった瞬間呼び出される");
            }

        }
    }
}
//}

=== 実はGUI.changed

ChangeCheckの中身はGUI.changedで実装されています。
GUI.changedのみで実装すると階層のことを考え以下のような実装になります。

//emlist[][cs]{
bool toggleValue;

Stack<bool> stack = new Stack<bool> ();

void OnGUI ()
{
    // BeginChangeCheckの役割
    {
        // 先頭に値をpush
        stack.Push (GUI.changed);
        GUI.changed = false;
    }


    toggleValue = EditorGUILayout.ToggleLeft ("Toggle", toggleValue);


    // EndChangeCheckの役割
    {
        // 変更があればtrueを取得する
        bool changed = GUI.changed;

        // どちらかがtrueであれば以降はすべて変更されているものとする
        GUI.changed |= stack.Pop ();
    }

    if (changed) {

    }
}
//}

== EditorGUI.BeginDisabledGroup / EndDisabledGroup

//image[ss02][下側のGUIが薄くなっていて操作することが出来ない]{

//}

//emlist[][cs]{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript : EditorWindow
{
    [MenuItem("Window/Example")]
    static void Open ()
    {
        GetWindow<NewBehaviourScript> ();
    }

    void OnGUI ()
    {
        Display ();

        EditorGUILayout.Space ();

        EditorGUI.BeginDisabledGroup (true);

        Display ();

        EditorGUI.EndDisabledGroup ();
    }

    void Display ()
    {
        EditorGUILayout.ToggleLeft ("Toggle", false);
        EditorGUILayout.IntSlider (0, 10, 0);
        GUILayout.Button ("Button");
    }
}
//}

=== 実はGUI.enabled

DisabledGroupの中身はGUI.enabledで実装されています。
GUI.enabledのみで実装すると以下のような実装になります。

//emlist[][cs]{
void OnGUI ()
{
    Display ();

    EditorGUILayout.Space ();

    GUI.enabled = false;

    Display ();

    GUI.enabled = true;
}

void Display ()
{
    EditorGUILayout.ToggleLeft ("Toggle", false);
    EditorGUILayout.IntSlider (0, 10, 0);
    GUILayout.Button ("Button");
}
//}

== EditorGUILayout.BeginFadeGroup / EndFadeGroup

//image[ss03][左側がフェード中。右側が通常の状態]{

//}

GUIのグループをフェードイン・フェードアウトさせる場合に使用します。
何かのトリガー、今回はボタンを押したらフェードでGUIが表示されるようにしてみました。

フェード中はGUIを操作することが出来ません。フェードのスピードはある程度早めにしてユーザーを待たせることのないようにしましょう。

//emlist[][cs]{
using UnityEngine;
using UnityEditor;
using UnityEditor.AnimatedValues;
using UnityEngine.Events;

public class NewBehaviourScript : EditorWindow
{
    [MenuItem("Window/Example")]
    static void Open ()
    {
        GetWindow<NewBehaviourScript> ();
    }

    // 初期値が0だとフェードを行わないと判断されるため0.0001fというような0に近い値にする
    AnimFloat animFloat = new AnimFloat (0.0001f);
    Texture tex;
    void OnGUI ()
    {
        bool on = animFloat.value == 1;

        if (GUILayout.Button (on ? "Close" : "Open", GUILayout.Width (64))) {
            animFloat.target = on ? 0.0001f : 1;
            animFloat.speed = 0.05f;

            // 値が変わるごとにEditorWindowを再描画する
            var env = new UnityEvent ();
            env.AddListener (() => Repaint ());
            animFloat.valueChanged = env;
        }

        EditorGUILayout.BeginHorizontal ();
        EditorGUILayout.BeginFadeGroup (animFloat.value);
        Display ();
        EditorGUILayout.EndFadeGroup ();
        Display ();
        EditorGUILayout.EndHorizontal ();
    }

    void Display ()
    {
        EditorGUILayout.BeginVertical ();
        EditorGUILayout.ToggleLeft ("Toggle", false);
        tex = EditorGUILayout.ObjectField (tex, typeof(Texture), false, GUILayout.Width (128), GUILayout.Height (128)) as Texture;
        GUILayout.Button ("Button");
        EditorGUILayout.EndVertical ();
    }
}
//}

== EditorGUI.ObjectField

== EditorGUI.showMixedValue

== EditorGUI.MultiFloatField

== EditorGUI.indentLevel

== EditorGUI.FocusTextInControl

== EditorGUILayout.BeginHorizontal / EditorGUILayout.BeginVertical

== EditorGUILayout.Knob

== EditorGUILayout.Separator

== ○○Scope
