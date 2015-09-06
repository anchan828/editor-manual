= EditorWindow


//lead{
エディター拡張に初めて触れるとき、まずウィンドウを表示することから始めるかもしれません。この章では簡単にEditorWindowを表示する方法から目的に合ったEditorWindowの選択、その特性について解説していきます。
//}

== EditorWindowとは

シーンウィンドウ、ゲームウィンドウ、インスペクターウィンドウなど、これら全てはEditorWindowです。
Unity エディターはさまざまな機能を持つEditorWindowの集まりでできています。


//image[ss01][これら全てはEditorWindow]{

//}


== HostView と SplitView と DockArea

EditorWindow は単体で描画されているわけではなく、親として @<b>{DockArea} を持ち、この DockArea によって EditorWindow は描画されています。

//image[ss19][DockAreaの上に EditorWindow があるイメージ]{

//}

DockArea は「Webブラウザーのタブ」と同じ機能を提供します。

例えば、ウィンドウはそれぞれ独立した3つのウィンドウにもできますし、3つのタブにして1つのウィンドウにまとめることもできます。

//image[ss20][Chromeのタブ機能]{
//}

見た目も似ていますし、タブの機能として、タブを摘んで別のウィンドウにすることもできます。

//image[ss21][DockAreaのタブ機能]{

//}

このように、DockArea には1つ以上の EditorWidow を描画するための機能が備わっています。例えば、2つ以上の EditorWindow が DockArea にある場合、タブ機能を使ってそれぞれの EditorWindow を表示するか@<b>{SplitWindow} で DockArea の領域を分割して表示します。


//image[ss22][1つの DockArea にシーンビューとヒエラルキービューを分割して表示]{

//}


さらに DockArea は @<b>{HostView} の役割も持っています。HostView は、さまざまなオブジェクト・イベントとのやり取りを行うための機能があります。ウィンドウの「Update関数」や「OnSelectionChange関数」などを実行するための機能が含まれています。

3つのウィンドウ「HostView」「SplitView」「DockArea」を紹介しました。これらの領域（クラス）には残念ながらアクセスできません。ですが、覚えておくと EditorWindow の仕組みがより早く理解できるかもしれません。

== EditorWindowの作成

まずは@<b>{一般的な}EditorWindowを作成してみましょう。


//image[ss02][表示しただけの何もないEditorWindow]{

//}

=== EditorWindowを表示するまで

EditorWindowを表示するまでの基本的な流れは3つのステップで構成されています。まずはこの3ステップを覚えましょう。

1. EditorWindowを作成するには@<b>{EditorWindowを継承したクラス}を作成します。

//emlist{
using UnityEditor;

public class Example : EditorWindow
{
}
//}

2. 次にEditorWindowを表示するためのトリガーとしてメニューを追加します。


//emlist{
using UnityEditor;

public class Example : EditorWindow
{
    // MenuItemについては@<chapref>{part1-menuitem}をご覧ください。
    [MenuItem("Window/Example")]
    static void Open ()
    {
    }
}
//}

3. 最後にEditorWindowの表示です。EditorWindowは@<b>{ScriptableObject}を継承しているので@<code>{EditorWindow.CreateInstance}でEditorWindowのオブジェクトを生成します。そして@<code>{Show}を呼び出すことによりEditorWindowが表示されます。

//emlist{
using UnityEditor;

public class Example : EditorWindow
{
    [MenuItem("Window/Example")]
    static void Open ()
    {
        var exampleWindow = CreateInstance<Example> ();
        exampleWindow.Show ();
    }
}
//}

//image[ss03]["Window/Example"メニューを実行して表示できる。実行するたびにEditorWindowを新規作成する。]{

//}

== EditorWindow.GetWindow

EditorWindowを作成する場合、「@<b>{複数の存在を許可するEditorWindow}」と「@<b>{単数のみ許可するEditorWindow}」の2種類が考えられます。@<b>{複数の存在を許可するEditorWindow}は先ほど説明した@<code>{EditorWindow.CreateInstance}を使ってEditorWindowを作成し表示していきます。

=== 単数のみ許可するEditorWindow

単数のみになると「既にEditorWindowが存在している場合は生成しない」というチェックを実装しなければいけません。そのチェックを加えたものが以下のコードになります。

//emlist{
using UnityEditor;

public class Example : EditorWindow
{
    static Example exampleWindow;

    [MenuItem("Window/Example")]
    static void Open ()
    {
        if (exampleWindow == null) {
            exampleWindow = CreateInstance<Example> ();
        }
        exampleWindow.Show ();
    }
}
//}

これでも良いのですが、「既にEditorWindowが存在すればそのインスタンスを取得する。なければ生成する。最後にShow関数を実行する。」という複数の機能を1つにまとめたAPIが存在します。それが@<code>{EditorWindow.GetWindow}です。

//emlist{
using UnityEditor;

public class Example : EditorWindow
{
    [MenuItem("Window/Example")]
    static void Open ()
    {
        GetWindow<Example> ();
    }
}
//}

GetWindowを実行すると内部でインスタンスがキャッシュされます。

さらにGetWindow関数には便利な機能があり、特定のEditorWindowにタブウィンドウとして表示する事が可能です。（DockAreaにEditorWindowを追加）

//image[ss04][シーンウィンドウにタブウィンドウとして追加できる]{

//}

//emlist{
using UnityEditor;

public class Example : EditorWindow
{
    [MenuItem("Window/Example")]
    static void Open ()
    {
        GetWindow<Example> (typeof(SceneView));
    }
}
//}

== どのShow関数を呼ぶかで変わる特殊なEditorWindow

今までこの章で扱ったEditorWidowはデフォルト状態のタブ機能が使えるウィンドウです。他にも EditorWindow は様々な種類のウィンドウを作成することができます。

@<code>{Show} 関数は複数用意されており、どの@<code>{Show}関数を呼び出すかで表示される EditorWindow が変化します。

=== Show

デフォルト状態のタブウィンドウとして扱えます。@<code>{EditorWindow.GetWindow}を使用している場合、内部で@<code>{Show}が呼び出されています。

=== ShowUtility

タブウィンドウとして扱えず、常に手前に表示し続けるウィンドウです。
たとえ他のウィンドウにフォーカスを当ててもそのWindow自体が裏側に回り込むことがありません。設定ウィンドウのような頻繁に他のウィンドウを操作してても最前面に表示したい場合に使用してください。

//image[ss05][ヒエラルキーのMain Cameraを選択してもウィンドウは最前面に表示されている]{

//}

@<code>{GetWindow}は使用できないので代わりに@<code>{CreateInstance}を使用します。

//emlist{
using UnityEditor;

public class Example : EditorWindow
{
    static Example exampleWindow;

    [MenuItem("Window/Example")]
    static void Open ()
    {
        if (exampleWindow == null) {
            exampleWindow = CreateInstance<Example> ();
        }
        exampleWindow.ShowUtility ();
    }
}
//}

=== ShowPopup

ウィンドウタイトルと閉じるボタンがないウィンドウです。
たとえ他のWindowにフォーカスを当ててもそのWindow自体が裏側に回り込むことがありません。
閉じるボタンがないため、ウィンドウを閉じる処理は自前で実装する必要があります。

//image[ss06][シーンウィンドウの上に表示されている]{

//}

//emlist{
using UnityEditor;
using UnityEngine;

public class Example : EditorWindow
{
    static Example exampleWindow;

    [MenuItem("Window/Example")]
    static void Open ()
    {
        if (exampleWindow == null) {
            exampleWindow = CreateInstance<Example> ();
        }
        exampleWindow.ShowPopup ();
    }


    void OnGUI ()
    {
        // エスケープを押した時に閉じる
        if (Event.current.keyCode == KeyCode.Escape) {
            exampleWindow.Close();
        }
    }
}
//}

使いドコロとしては@<b>{SpriteEditor}のような@<b>{スライスメニューボタン}からPopupを表示します。

//image[ss07][これもEditorWindowの1種]{

//}

=== PopupWindow

直前で紹介した@<code>{ShowPopup}とほぼ同じものでPopupを表示するための機能です。PopupWindowはPopupを汎用的に扱えるようにしたものと考えてください。

//image[ss08][ボタンを押すと真下にPopupが表示される]{

//}

使い方はとても簡単です。まず@<b>{PopupWindowContentを継承したクラス}を作成します。そして@<code>{PopupWindow.Show}でPopupを表示します。

//emlist{
using UnityEditor;
using UnityEngine;

public class Example : EditorWindow
{
    [MenuItem("Window/Example")]
    static void Open ()
    {
        GetWindow<Example> ();
    }

    // インスタンス化
    ExamplePupupContent popupContent = new ExamplePupupContent ();

    void OnGUI ()
    {
        if (GUILayout.Button ("PopupContent",GUILayout.Width(128))) {
            var activatorRect = GUILayoutUtility.GetLastRect ();
            // Popupを表示する
            PopupWindow.Show (activatorRect, popupContent);
        }
    }
}

public class ExamplePupupContent : PopupWindowContent
{
    public override void OnGUI (Rect rect)
    {
        EditorGUILayout.LabelField ("Lebel");
    }

    public override void OnOpen ()
    {
        Debug.Log ("表示するときに呼び出される");
    }

    public override void OnClose ()
    {
        Debug.Log ("閉じるときに呼び出される");
    }

    public override Vector2 GetWindowSize ()
    {
        // Popupのサイズ
        return new Vector2 (300, 100);
    }
}
//}

=== ShowAuxWindow

タブウィンドウとして扱えないウィンドウを作成します。見た目は@<code>{ShowUtility}と同じですが他のウィンドウにフォーカスを当てるとウィンドウは削除されます。
ウィンドウの消し忘れが起こらないので数を増やすことがなく、ちょっとした設定・操作で使用することをオススメします。

//image[ss09][見た目だけではShowUtilityかShowAuxWindowなのか判断は難しい]{

//}

=== ShowAsDropDown

Popupと同じで、「ウィンドウのタイトル」「閉じるボタン」がないウィンドウです。
ただし、PCの画面サイズを考慮して、ウィンドウを表示する位置で@<b>{十分な広さを確保できなかった場合}、ウィンドウの表示領域を画面内に収めるために X/Y 軸の位置が自動的に補正されるようになります。言い換えると、画面の隅っこでウィンドウを出したとしても必ずPCの表示領域にすべて表示されます。

//image[ss10][黒いのがShowAsDropDownで表示したウィンドウだとする]{

//}

//emlist{
using UnityEditor;
using UnityEngine;

public class Example : EditorWindow
{
    static Example exampleWindow;

    [MenuItem("Window/Example")]
    static void Open ()
    {
        if (exampleWindow == null) {
            exampleWindow = CreateInstance<Example> ();
        }

        var buttonRect = new Rect (100, 100, 300, 100);
        var windowSize = new Vector2 (300, 100);
        exampleWindow.ShowAsDropDown (buttonRect, windowSize);
    }
}
//}

これ以外はPopupと同じ機能です。

=== ScriptableWizard

「GameObjectを作る」「Prefabを作る」「アセットを作る」、このように何かを「作る」時に使用するウィンドウです。
ScriptableWizardは今まで紹介してきたウィンドウとは少し異なります。

==== ScriptableWizardの作り方

1.  ScriptableWizardを継承したクラスを作成します。

//emlist{
using UnityEditor;

public class Example : ScriptableWizard
{
}
//}


2. 次にScriptableWizardを表示するためのトリガーとしてメニューを追加します。

//emlist{
using UnityEditor;

public class Example : ScriptableWizard
{
    // MenuItemについては@<chapref>{part1-menuitem}をご覧ください。
    [MenuItem("Window/Example")]
    static void Open ()
    {
    }
}
//}

3. ScriptableWizardを表示します。表示は@<code>{ScriptableWizard.DisplayWizard}で行います。

//emlist{
using UnityEditor;

public class Example : ScriptableWizard
{
    [MenuItem("Window/Example")]
    static void Open ()
    {
        DisplayWizard<Example> ("Example Wizard");
    }
}
//}

//image[ss11][標準で右下にCreateボタンが表示される]{

//}

==== ScriptableWizardにはクラスのフィールドが表示される

他のEditorWindowではGUIの表示にEditorGUIクラスを使用しますがScriptableWizardでは使用できません。ScriptableWizardではインスペクターで表示されるような「publicなフィールド」「シリアライズ可能なフィールド」がウィンドウに表示されます。

//image[ss12][インスペクターで表示されるようなものがそのまま表示されるイメージ]{

//}

//emlist{
using UnityEditor;

public class Example : ScriptableWizard
{
    public string gameObjectName;

    [MenuItem("Window/Example")]
    static void Open ()
    {
        DisplayWizard<Example> ("Example Wizard");
    }
}
//}


==== OnWizardCreate

ScriptableWizardの右下にある@<b>{Create}ボタンを押した時に呼び出されるメソッドです。

//emlist{
using UnityEditor;
using UnityEngine;

public class Example : ScriptableWizard
{
    public string gameObjectName;

    [MenuItem("Window/Example")]
    static void Open ()
    {
        DisplayWizard<Example> ("Example Wizard");
    }

    void OnWizardCreate ()
    {
        new GameObject (gameObjectName);
    }
}
//}

==== OnWizardOtherButton

@<b>{Create}ボタンの他にもう1つボタンを追加できます。作成に関して2つのパターンを作りたい場合に使用してください。
ボタンを追加するには@<code>{ScriptableWizard.DisplayWizard}の第3引数でボタン名を指定する必要があります。

//emlist{
using UnityEditor;
using UnityEngine;

public class Example : ScriptableWizard
{
    public string gameObjectName;

    [MenuItem("Window/Example")]
    static void Open ()
    {
        DisplayWizard<Example> ("Example Wizard", "Create", "Find");
    }

    void OnWizardCreate ()
    {
        new GameObject (gameObjectName);
    }

    void OnWizardOtherButton ()
    {
        var gameObject = GameObject.Find (gameObjectName);

        if (gameObject == null)
        {
            Debug.Log ("ゲームオブジェクトが見つかりません");
        }
    }
}
//}


==== OnWizardUpdate

すべてのフィールドの値を対象に、値の変更があった場合に呼び出されるメソッドです。


//emlist{
using UnityEditor;
using UnityEngine;

public class Example : ScriptableWizard
{
    [MenuItem("Window/Example")]
    static void Open ()
    {
        DisplayWizard<Example> ("Example Wizard");
    }

    void OnWizardUpdate ()
    {
        Debug.Log ("Update");
    }
}
//}

==== DrawWizardGUI

ウィザート内のGUIを描画するためのメソッドです。このメソッドをオーバーライドすることによりGUIをカスタマイズできます。

ただし、戻り値として @<code>{true} を返すようにしてください。trueを返さなければ @<code>{OnWizardUpdate} が呼び出されなくなってしまいます。


//image[ss23][今まで表示されていたプロパティーがなくなりラベルが表示された]{

//}

//emlist{
using UnityEditor;
using UnityEngine;

public class Example : ScriptableWizard
{
    public string gameObjectName;

    [MenuItem ("Window/Example")]
    static void Open ()
    {
        DisplayWizard<Example> ("Example Wizard");
    }

    protected override bool DrawWizardGUI ()
    {
        EditorGUILayout.LabelField ("Label");
        // false を返すことでOnWizardUpdateが呼び出されなくなる
        return true;
    }
}
//}

==== OnGUIメソッドは使用しないこと

ScriptableWizardクラスはEditorWindowを継承しています。なのでOnGUIメソッドを使用すると通常のEditorWindowとしての表示となってしまい、フィールドの値やCreateボタンが表示されなくなってしまいます。

//image[ss13][OnGUIメソッドを記述するとCreateボタンが消えてしまう]{

//}

=== PreferenceItem

PreferenceItemはUnity Preferencesにメニューを追加するための機能です。
Unity PreferencesにはUnity エディター全体に影響のある設定を行うためにあります。


//image[ss16][追加されるメニューは一番最後の位置に追加される]{

//}


//emlist{
using UnityEditor;

public class Example
{
    [PreferenceItem("Example")]
    static void OnPreferenceGUI ()
    {

    }
}
//}




== メニューを追加するIHasCustomMenu

タブ上で右クリック、または ≡ をクリックすることで表示されるコンテキストメニューにメニューを追加します。

//image[ss14][exampleとexample2が追加された]{

//}

IHasCustomMenuはインターフェースとして実装されています。

//emlist{
using UnityEditor;
using UnityEngine;

public class Example : EditorWindow, IHasCustomMenu
{

    public void AddItemsToMenu (GenericMenu menu)
    {
        menu.AddItem (new GUIContent ("example"), false, () => {

        });

        menu.AddItem (new GUIContent ("example2"), true, () => {

        });
    }

    [MenuItem("Window/Example")]
    static void Open ()
    {
        GetWindow<Example> ();
    }
}
//}

== EditorWindowのサイズを変更できないようにする

//image[ss15][右下にあるリサイズするための三角マークが消えている]{

//}

@<code>{EditorWindow.minSize}と@<code>{EditorWindow.maxSize}によってEditorWindowの大きさの制限を行うことができます。最小値と最大値が同じであればEditorWindowの大きさを変更する必要がないと判断され右下に表示されていた三角マークが非表示となります。

//emlist{
using UnityEditor;
using UnityEngine;

public class Example : EditorWindow
{
    [MenuItem("Window/Example")]
    static void Open ()
    {
        var window = GetWindow<Example> ();
        window.maxSize = window.minSize = new Vector2 (300, 300);
    }
}
//}

== ウィンドウにアイコンを追加する

アイコンを追加するには@<code>{EditorWindow.titleContent}にアイコンを持つ GUIContent を代入します。

//image[ss24][アイコンはとても小さいので分かりやすいものにすること]{

//}

//emlist{
using UnityEditor;
using UnityEngine;

public class Example : EditorWindow
{
    [MenuItem ("Window/Example")]
    static void SaveEditorWindow ()
    {
        var window = GetWindow<Example> ();

        var icon = AssetDatabase.LoadAssetAtPath<Texture> ("Assets/Editor/Example.png");

        window.titleContent = new GUIContent ("Hoge", icon);
    }
}
//}


== GetWindowを使わずに既にあるEditorWindowを取得するには？

自前でシングルトンを実装するか、GetWindowによって内部にキャッシュしておく方法でEditorWindowへとアクセスできます。しかし、先ほど上げた2つの方法が使えない状況も出てきます。その時は@<b>{Resources}クラスにある@<b>{Resources.FindObjectsOfTypeAll}を使用します。

@<code>{FindObjectsOfTypeAll}は@<b>{現在ロードされている全てのオブジェクトから特定のオブジェクトを検索し、取得}します。これはランタイムで使用するオブジェクトだけではなく、エディターで使用するオブジェクトも検索対象となります。

//emlist{
using UnityEditor;
using UnityEngine;

public class Example : EditorWindow
{
    [MenuItem("Window/Example")]
    static void Open ()
    {
        // 全てのシーンビューを取得する
        var sceneViews = Resources.FindObjectsOfTypeAll<SceneView> ();
    }
}
//}


== EditorWindowもScriptableObjectであることを知る

//image[ss17][アセンブリブラウザーで見るとScriptableObjectが継承されていることが分かる]{

//}

EditorWindow は、ScriptableObjectの動作にしたがい、EditorWindowオブジェクトをアセットとして保存することができます。その時に、インスペクターにはシリアライズされたプロパティーも表示されます。

//image[ss18][Example ウインドウをアセットとして保存した時のインスペクター]{

//}

//emlist{
using UnityEditor;
using UnityEngine;
public class Example : EditorWindow
{
    [MenuItem ("Assets/Save EditorWindow")]
    static void SaveEditorWindow ()
    {
        AssetDatabase.CreateAsset (CreateInstance<Example> (), "Assets/Example.asset");
        AssetDatabase.Refresh ();
    }

    [SerializeField]
    string text;

    [SerializeField]
    bool boolean;
}
//}

ウィンドウ位置やサイズなども保存されています。それらのデータは直接YAML形式のファイルをテキストエディターで見れば確認できます。

//emlist{
  m_MinSize: {x: 100, y: 100}
  m_MaxSize: {x: 4000, y: 4000}
  m_TitleContent:
    m_Text: Example
    m_Image: {fileID: 0}
    m_Tooltip:
  m_DepthBufferBits: 0
  m_AntiAlias: 0
  m_Pos:
    serializedVersion: 2
    x: 0
    y: 0
    width: 320
    height: 240
//}
