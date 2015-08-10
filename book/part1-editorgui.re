= EditorGUI (EdirotGUILayout)


//image[frontispiece][頑張ればこのような GUI を作ることもできる]{

//}

//lead{
この機能を知らなくてはエディター拡張では何もできないと言っても過言ではありません。この章では全てを紹介することはできませんが、知っておくと EditorGUI や EditorGUILayout でできることの幅が広がるものを中心に説明していきます。サンプルコードは簡略化の関係でEditorGUILayoutを使用することが多くなります。
//}

//pagebreak

== EditorGUIとは

ランタイム側にあるGUIクラスと役割は同じですがEditor向けに機能が追加されているクラスです。
逐次配置されるEditorGUILayoutクラスも用意されています。

まずは簡単にEditorWindowの上にEditorGUIで文字を表示してみましょう。

//image[ss01][EditorGUIの第一歩]{

//}

//emlist{
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




== ChangeCheck

BeginChangeCheckとEndChangeCheckでの間でGUIが使用されている時、GUIに何らかの変更が生じた場合にEndChangeCheckがtrueを返します。

//emlist{
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

//emlist{
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

== DisabledGroup

//image[ss02][下側のGUIが薄くなっていて操作することができない]{

//}

//emlist{
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

//emlist{
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

== FadeGroup

//image[ss03][左側がフェード中。右側が通常の状態]{

//}

GUIのグループをフェードイン・フェードアウトさせる場合に使用します。
何かのトリガー、今回はボタンを押したらフェードでGUIが表示されるようにしてみました。

フェード中はGUIを操作することができません。フェードのスピードはある程度早めにしてユーザーを待たせることのないようにしましょう。

//emlist{
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

        var options = new GUILayout[]{GUILayout.Width (128), GUILayout.Height (128)};

        tex = EditorGUILayout.ObjectField (
                tex, typeof(Texture), false, options) as Texture;

        GUILayout.Button ("Button");
        EditorGUILayout.EndVertical ();
    }
}
//}

== EditorGUI.ObjectField

オブジェクトの参照を扱うフィールドです。引数に受け付けるオブジェクトの型を指定できます。

また、テクスチャ系（Texture2DやSprite）は特殊なサムネイル形式のフィールドになります。

//image[ss11][通常はオブジェクトのアイコンと受け付けるオブジェクトの型名][scale=0.7]{

//}

//emlist{
void OnGUI ()
{
    EditorGUILayout.ObjectField (null, typeof(Object), false);
    
    EditorGUILayout.ObjectField (null, typeof(Material), false);

    EditorGUILayout.ObjectField (null, typeof(AudioClip), false);

    var options = new []{GUILayout.Width (64), GUILayout.Height (64)};

    EditorGUILayout.ObjectField (null, typeof(Texture), false, options);

    EditorGUILayout.ObjectField (null, typeof(Sprite), false, options);
}
//}


== EditorGUI.MultiFloatField

複数の float 値を編集するフィールドを1行で描画するためのものです。

インスペクターで Vector3 の値を編集するような描画になります。

//image[ss12][決められた Rect 内ですべての要素を均等に描画する]{

//}

//emlist{
float[] numbers = new float[] {
    0,
    1,
    2
};

GUIContent[] contents = new GUIContent[] {
    new GUIContent ("X"),
    new GUIContent ("Y"),
    new GUIContent ("Z")
};

void OnGUI ()
{
    EditorGUI.MultiFloatField (
        new Rect (30, 30, 200, EditorGUIUtility.singleLineHeight),
        new GUIContent ("Label"), 
        contents, 
        numbers);
}
//}

== EditorGUI.indentLevel

インデントのレベルを管理します。以下のコードのように部分的にインデントを増減させることにより、インスペクターやヒエラルキーで見るような階層構造を構成できます。

//image[ss13][ヒエラルキーでよく見る親子関係]{

//}

//emlist{
void OnGUI ()
{
    EditorGUILayout.LabelField ("Parent");

    EditorGUI.indentLevel++;

    EditorGUILayout.LabelField ("Child");
    EditorGUILayout.LabelField ("Child");

    EditorGUI.indentLevel--;

    EditorGUILayout.LabelField ("Parent");

    EditorGUI.indentLevel++;

    EditorGUILayout.LabelField ("Child");
}
//}

これは EditorGUI と EditorGUILayout の両方に効果があります。

== EditorGUILayout.Knob

角度や、決められた範囲内で値を設定するための「つまみ（ノブ）」を作成します。マウスでドラッグしたり、表示されているラベルをクリックすることで値を直接入力できます。

//image[ss14][マウスでドラッグすることで値を調整できる]{

//}

//emlist{
float angle = 0;

void OnGUI ()
{
    angle = EditorGUILayout.Knob (Vector2.one * 64,
        angle, 0, 360, "度", Color.gray, Color.red, true);
}
//}

== ○○Scope

EditorGUILayout.BeginHorizontal/EndHorizontal というように Begin/End で始まるGUIグループのヘルパー機能になります。
標準で@<code>{HorizontalScope}、@<code>{VerticalScope}、@<code>{ScrollViewScope} などのスコープが用意されています。
スコープ自体は@<b>{IDisposable}オブジェクトで実装されており、usingを使用できます。

//emlist{
#@maprange(unityprojects/part1/editorgui/Assets/Editor/ExampleScope.cs,HorizontalScope)
#@end
//}

//image[ss04][][]{
//}

=== Scopeの自作 - BackgroundColorScope

HorizontalScopeのように、スコープはGUI.Scopeクラスを継承して作成されています。

//emlist{
#@maprange(unityprojects/part1/editorgui/Assets/Editor/ExampleScope.cs,HorizontalScopeCode)
#@end
//}

CloseScopeメソッドはDispose時に呼び出されるメソッドです。コンストラクター内でBegin、CloseScopeメソッド内でEndを呼び出しています。

同じようにして GUI.Scope を継承したクラスでスコープを作成することが可能です。今回は試しに@<img>{ss05}のようなスコープ内のみGUIの背景を変更するBackgroundColorScopeを作成してみましょう。

//image[ss05][1つのボタンごとに色が変更されている][scale=0.8]{

//}

//emlist{
#@maprange(unityprojects/part1/editorgui/Assets/Editor/ExampleScope.cs,BackgroundColorScope)
#@end
//}


このように@<code>{GUI.backgroundColor}にColor情報を渡す前に変数として保持し、CloseScopeで元の色に戻します。


//emlist{
#@maprange(unityprojects/part1/editorgui/Assets/Editor/ExampleScope.cs,BackgroundColorScope_Example)
#@end
//}

== 見た目はボタン、中身はトグル


エディターには見た目（GUIStyle）はボタンだが、動作はトグルのようなオン/オフとなる部分、またはボタンの群があり切り替わっていくものがいくつかあります。

//image[ss07][ツール、再生ボタン、PlayerSettingsのプラットフォーム別設定][]{

//}

これらのボタン？トグル？たちが一体どうやって実装されているか、数パターン紹介していきます。

=== スタイルがボタンなトグル（シングル）

//indepimage[ss08][]

//emlist{
#@maprange(unityprojects/part1/editorgui/Assets/Editor/ButtonToggle.cs,ButtonToggle_Pattern1)
#@end
//}

=== スタイルがボタンなトグル（マルチプル）

複数の選択肢の中から1つを選ばせる時に@<img>{ss06}のようなトグル群を作成します。

//indepimage[ss06][]


//emlist{
#@maprange(unityprojects/part1/editorgui/Assets/Editor/ButtonToggle.cs,ButtonToggle_Pattern2)
#@end
//}

おそらく、複数のトグルを配置しようとすると上記のようなコードになるかもしれません。ですがこれは悪手です。これだとbool変数がトグルの数だけ増えることになりますし、その管理も面倒です。
これらは@<code>{GUILayout.Toolbar}を使って解決できます。

//emlist{
#@maprange(unityprojects/part1/editorgui/Assets/Editor/ButtonToggle.cs,ButtonToggle_Pattern3)
#@end
//}

何が選択されているかをint変数で管理し、表示するトグル（の文字列）はstring配列で管理します。

また、GUIStyleを変更することでさまざまな表現が可能になります。

@<code>{EditorStyles.toolbarButton} を使用することでツールバーや、PlayerSettingsのプラットフォーム別設定にあるようなトグル群を表現できます。

//indepimage[ss09][]

//emlist{
#@maprange(unityprojects/part1/editorgui/Assets/Editor/ButtonToggle.cs,ButtonToggle_Pattern4)
#@end
//}

1列に表示した@<code>{GUILayout.SelectionGrid}で、スタイルを@<b>{PreferencesKeysElement}（Unity内部で実装されているGUIStyle）にするとPreferenceｓウィンドウで表現されている選択メニューとなります。

//indepimage[ss10][]

//emlist{
#@maprange(unityprojects/part1/editorgui/Assets/Editor/ButtonToggle.cs,ButtonToggle_Pattern5)
#@end
//}