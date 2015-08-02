= EditorWindow

//indepimage[frontispiece]


//lead{
エディター拡張に初めて触れるとき、まずウインドウを表示することから始めるかもしれません。この章では簡単にEditorWindowを表示する方法から目的に合ったEditorWindowの選択、その特性について解説していきます。
//}
//pagebreak

== EditorWindowとは

シーンウインドウ、ゲームウインドウ、インスペクターウインドウなど、これら全てはEditorWindowです。
Unityエディターは様々な機能を持つEditorWindowの集まりで出来ています。


//image[ss01][これら全てはEditorWindow]{

//}

== EditorWindowの作成

まずは@<b>{一般的な}EditorWindowを作成してみましょう。


//image[ss02][表示しただけの何もないEditorWindow]{

//}

=== EditorWindowを表示するまで

EditorWindowを表示するまでの基本的な流れは3つのステップで構成されています。まずはこの3ステップを覚えましょう。

1. EditorWindowを作成するには@<b>{EditorWindowを継承したクラス}を作成します。

//emlist[][cs]{
using UnityEditor;

public class Example : EditorWindow
{
}
//}

2. 次にEditorWindowを表示するためのトリガーとしてメニューを追加します。MenuItemについては@<chapref>{part1-menuitem}を御覧ください。


//emlist[][cs]{
using UnityEditor;

public class Example : EditorWindow
{
    [MenuItem("Window/Example")]
    static void Open ()
    {
    }
}
//}

3. 最後にEditorWindowの表示です。EditorWindowは@<b>{ScriptableObject}を継承しているので@<code>{EditorWindow.CreateInstance}でEditorWindowのオブジェクトを生成します。そして@<code>{Show}を呼び出すことによりEditorWindowが表示されます。

//emlist[][cs]{
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

//image[ss03]["Window/Example"メニューを実行するたびにEditorWindowを作成し表示できる]{

//}

== EditorWindow.GetWindow

EditorWindowを作成する場合、「@<b>{複数の存在を許可するEditorWindow}」と「@<b>{単数のみ許可するEditorWindow}」の2種類が考えられます。@<b>{複数の存在を許可EditorWindow}は先ほど説明した@<code>{EditorWindow.CreateInstance}を使ってEditorWindowを作成し表示していきます。

=== 単数のみ許可するEditorWindow

単数のみになると「既にEditorWindowが存在している場合は生成しない」というチェックを実装しなければいけません。そのチェックを加えたものが以下のコードになります。

//emlist[][cs]{
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

これでも良いのですが、「既にEditorWindowが存在すればそのインスタンスを取得する。なければ生成する。ついでにShowを実行する。」というAPIが存在します。それが@<code>{EditorWindow.GetWindow}です。

//emlist[][cs]{
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

更にGetWindowでは便利な機能があり、特定のEditorWindowにタブウインドウとして表示する事が可能です。

//image[ss04][シーンウインドウにタブウインドウとして追加することができる]{

//}

//emlist[][cs]{
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

今までこの章で扱ったEditorWidowはデフォルト状態のタブウインドウです。この他にもUnityで作成できるEditorWindowは複数存在します。

どの@<code>{Show}関数を呼び出すかでどのタイプのEditorWindowを表示するかが決定します。

=== Show

デフォルト状態のタブウインドウとして扱えます。@<code>{EditorWindow.GetWindow}を使用している場合、内部で@<code>{Show}が呼び出されています。

=== ShowUtility

タブウインドウとして扱えず、必ず手前に表示し続けるウインドウです。
たとえ他のウインドウにフォーカスを当ててもそのWindow自体が裏側に回り込むことがありません。設定ウインドウのような頻繁に他のウインドウを操作してても最前面に表示したい場合に使用してください。

//image[ss05][ヒエラルキーのMain Cameraを選択してもウインドウは最前面に表示されている]{

//}

@<code>{GetWindow}は使用できないので代わりに@<code>{CreateInstance}を使用します。

//emlist[][cs]{
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

ウインドウタイトルと閉じるボタンが無いウインドウです。
たとえ他のWindowにフォーカスを当ててもそのWindow自体が裏側に回り込むことがありません。
閉じるボタンが無いため、ウインドウを閉じる処理は自前で実装する必要があります。

//image[ss06][シーンウインドウの上に表示されている]{

//}

//emlist[][cs]{
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

//emlist[][cs]{
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

タブウインドウとして扱えないウインドウを作成します。見た目は@<code>{ShowUtility}と同じですが他のウインドウにフォーカスを当てるとウインドウは削除されます。
ウインドウの消し忘れが起こらないので数を増やすことがなく、ちょっとした設定・操作で使用することをオススメします。

//image[ss09][見た目だけではShowUtilityかShowAuxWindowなのか判断は難しい]{

//}

=== ShowAsDropDown

Popupと同じウインドウタイトルと閉じるボタンが無いウインドウです。
ただし、PCの画面サイズを考慮し表示させようとしている位置で十分な広さを確保できなかった場合、ウインドウの表示領域を守るためにX/Y軸の位置が自動的にズレるようになります。言い換えると、画面の隅っこでウインドウを出したとしても必ずPCの表示領域にすべて表示されます。

//image[ss10][黒いのがShowAsDropDownで表示したウインドウだとする]{

//}

//emlist[][cs]{
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
      
        exampleWindow.ShowAsDropDown (new Rect (100, 100, 300, 100), new Vector2 (300, 100));
    }
}
//}

これ以外はPopupと同じ機能です。

=== ScriptableWizard

 * GameObjectを作る
 * Prefabを作る
 * アセットを作る

このように何かを「作る」時に使用するウインドウです。
ScriptableWizardは今まで紹介してきたウインドウとは少し異なります。

==== ScriptableWizardの作り方

1.  ScriptableWizardを継承したクラスを作成します。

//emlist[][cs]{
using UnityEditor;

public class Example : ScriptableWizard
{
}
//}


2. 次にScriptableWizardを表示するためのトリガーとしてメニューを追加します。MenuItemについては@<chapref>{part1-menuitem}を御覧ください。

//emlist[][cs]{
using UnityEditor;

public class Example : ScriptableWizard
{
    [MenuItem("Window/Example")]
    static void Open ()
    {
    }
}
//}

3. ScriptableWizardを表示します。表示は@<code>{ScriptableWizard.DisplayWizard}で行います。

//emlist[][cs]{
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

他のEditorWindowではGUIの表示にEditorGUIクラスを使用しますがScriptableWizardでは使用することが出来ません。ScriptableWizardではインスペクターで表示されるような「publicなフィールド」「シリアライズ可能なフィールド」がウインドウに表示されます。

//image[ss12][インスペクターで表示されるようなものがそのまま表示されるイメージ]{

//}

//emlist[][cs]{
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

//emlist[][cs]{
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

@<b>{Create}ボタンの他にもう1つボタンを追加することが出来ます。作成に関して2つのパターンを作りたい場合に使用してください。
ボタンを追加するには@<code>{ScriptableWizard.DisplayWizard}の第3引数でボタン名を指定する必要があります。

//emlist[][cs]{
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


//emlist[][cs]{
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

    void OnWizardUpdate ()
    {
        Debug.Log ("Update");
    }
}
//}

==== OnGUIメソッドは使用しないこと

ScriptableWizardクラスはEditorWindowを継承しています。なのでOnGUIメソッドを使用すると通常のEditorWindowとしての表示となってしまい、フィールドの値やCreateボタンが表示されなくなってしまいます。

//image[ss13][OnGUIメソッドを記述するとCreateボタンが消えてしまう]{

//}

=== PreferenceItem

PreferenceItemはUnity Preferencesにメニューを追加するための機能です。
Unity PreferencesにはUnityエディター全体に影響のある設定を行うためにあります。


//image[ss16][追加されるメニューは一番最後の位置に追加される]{

//}


//emlist[][cs]{
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

//emlist[][cs]{
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

@<code>{EditorWindow.minSize}と@<code>{EditorWindow.maxSize}によってEditorWindowの大きさの制限を行うことが出来ます。最小値と最大値が同じであればEditorWindowの大きさを変更する必要がないと判断され右下に表示されていた三角マークが非表示となります。

//emlist[][cs]{
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

== GetWindowを使わずに既にあるEditorWindowを取得するには？

自前でシングルトンを実装するか、GetWindowによって内部にキャッシュしておく方法でEditorWindowへとアクセスすることが出来ます。しかし、先ほど上げた2つの方法が使えない状況も出てきます。その時は@<b>{Resources}クラスにある@<b>{Resources.FindObjectsOfTypeAll}を使用します。

@<code>{FindObjectsOfTypeAll}は@<b>{現在ロードされている全てのオブジェクトから特定のオブジェクトを検索し、取得}します。これはランタイムで使用するオブジェクトだけではなく、エディターで使用するオブジェクトも検索対象となります。

//emlist[][cs]{
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

//image[ss17][アセンブリブラウザで見るとScriptableObjectが継承されていることが分かる]{

//}

通常のScriptableObjectの動作に則って、アセットとしてウインドウを保存することも出来ますし、シリアライズプロパティを作成してインスペクターに表示することも可能です。

//image[ss18][]{

//}

//emlist[][cs]{
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

ウインドウ位置やサイズなども保存されています。それらのデータは直接YAML形式のファイルをテキストエディターで見れば確認できます。

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

