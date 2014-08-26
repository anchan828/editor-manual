= EditorWindow

//lead{
エディタ拡張に初めて触れるとき、まずウィンドウを表示することから始めるかもしれません。この章では簡単にEditorWindowを表示する方法から目的に合ったEditorWindowの選択、その特性について解説していきます。
//}

== EditorWindowとは

シーンウィンドウ、ゲームウィンドウ、インスペクターウィンドウなど、これら全てはEditorWindowです。
Unityエディタは様々な機能を持つEditorWindowの集まりで出来ています。


//image[ss01][これら全てはEditorWindow]{

//}

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

2. 次にEditorWindowを表示するためのトリガーとしてメニューを追加します。MenuItemについては@<chapref>{part1-menuitem}を御覧ください。


//emlist{
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

//image[ss03]["Window/Example"メニューを実行するたびにEditorWindowを作成し表示できる]{

//}

== EditorWindow.GetWindow

EditorWindowを作成する場合、「@<b>{複数の存在を許可EditorWindow}」と「@<b>{単数のみ許可するEditorWindow}」の2種類が考えられます。@<b>{複数の存在を許可EditorWindow}は先ほど説明した@<code>{EditorWindow.CreateInstance}を使ってEditorWindowを作成し表示していきます。

=== 単数のみ許可するEditorWindow

単数のみになると「既にEditorWindowが存在している場合は生成しない」というチェックを入れなければいけません。そのチェックを加えたものが以下のコードになります。

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

これでも良いのですが、「既にEditorWindowが存在すればそのインスタンスを取得する。なければ生成する。ついでにShowを実行する。」というAPIが存在します。それが@<code>{EditorWindow.GetWindow}です。

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

更にGetWindowでは便利な機能があり、特定のEditorWindowにタブウィンドウとして表示する事が可能です。

//image[ss04][シーンウィンドウにタブウィンドウとして追加することができる]{

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

== 特殊なEditorWindow

今までこの章で扱ってEditorWidowはデフォルト状態のタブウィンドウです。この他にもUnityで作成できるEditorWindowは複数存在します。

どの@<code>{Show○○}関数を呼び出すかでどのタイプのEditorWindowを表示するかが決定します。

=== Show

デフォルト状態のタブウィンドウとして扱えます。@<code>{EditorWindow.GetWindow}を使用している場合、内部で@<code>{Show}が呼び出されています。

=== ShowUtility

タブウィンドウとして扱えず、必ず手前に表示し続けるウィンドウです。
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

ウィンドウタイトルと閉じるボタンが無いウィンドウです。
たとえ他のWindowにフォーカスを当ててもそのWindow自体が裏側に回り込むことがありません。
閉じるボタンが無いため、ウィンドウを閉じる処理は自前で実装する必要があります。

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

Popupと同じウィンドウタイトルと閉じるボタンが無いウィンドウです。
ただし、PCの画面サイズを考慮し表示させようとしている位置で十分な広さを確保できなかった場合、ウィンドウの表示領域を守るためにX/Y軸の位置が自動的にズレるようになります。言い換えると、画面の隅っこでウィンドウを出したとしても必ずPCの表示領域にすべて表示されます。

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
      
        exampleWindow.ShowAsDropDown (new Rect (100, 100, 300, 100), new Vector2 (300, 100));
    }
}
//}

これ以外はPopupと同じ機能です。

=== ScriptableWizard

 * GameObjectを作る
 * Prefabを作る
 * アセットを作る

このように何かを「作る」時に使用するウィンドウです。
ScriptableWizardは今まで紹介してきたウィンドウとは少し異なります。

==== ScriptableWizardの作り方

1.  ScriptableWizardを継承したクラスを作成します。

//emlist{
using UnityEditor;

public class Example : ScriptableWizard
{
}
//}


2. 次にScriptableWizardを表示するためのトリガーとしてメニューを追加します。MenuItemについては@<chapref>{part1-menuitem}を御覧ください。

//emlist{
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

他のEditorWindowではGUIの表示にEditorGUIクラスを使用しますがScriptableWizardでは使用することが出来ません。ScriptableWizardではインスペクターで表示されるような「publicなフィールド」「シリアライズ可能なフィールド」がウィンドウに表示されます。

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

@<b>{Create}ボタンの他にもう1つボタンを追加することが出来ます。作成に関して2つのパターンを作りたい場合に使用してください。
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

=== PreferenceWindow


== IHasCustomMenu

== EditorWindowのサイズを変更できないようにする

== メニューを追加する

#@# IHasCustomMenu

== GetWindowを使わずに既にあるEditorWindowを取得するには？

#@# Resources.FindObjectsOfTypeAll


== なにか作ってみよう