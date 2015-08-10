= データの保存

エディター拡張で何かを作成している場合、値を保存して次回以降も使いまわしたい時があります。その値はエディター拡張の設定に関するパラメーターであったり、ゲームに関係するパラメーターであったりさまざまです。Unity にはデータを保存する手段が大きく分けて 3 パターンあります。本章では、それらを紹介し目的に合った手段を選べるように解説していきます。


== EditorPrefs

@<b>{PC内で共有できるデータの保存方法です。}Unity プロジェクトに縛られず、Unity エディターをまたいだ設定を保存するのに適しています。

=== 影響範囲

保存した値はメジャーバージョンごとの全Unityエディター & 全プロジェクトに影響します。

Unity 4.x で保存した値は、Unity 4.x でのみ扱えます。Unity 5.x も Unity 5.x でのみ扱えます。

//image[ss01][Unity4.x のみ影響。Unity5.x のみ影響というように分かれる]{
PCとUityとプロジェクトの影響が分かるような図
//}


=== 何を保存するか

EditorPrefs で保存すべきものは@<b>{ウィンドウの位置・サイズ}、@<b>{Unityエディターの環境設定（Preferencesにあるような設定）}の値です。EditorPrefs 経由で保存される値は全て平文で保存されます。決して、パスワードなど重要な情報は保存しないようにしてください。


=== EditorPrefsが保存されている場所

//table[tbl2][]{
プラットフォーム    場所
-----------------
@<strong>{Windows}（Unity4.x）    HKEY_CURRENT_USER\Software\Unity Technologies\UnityEditor 4.x
@<strong>{Windows}（Unity5.x）    HKEY_CURRENT_USER\Software\Unity Technologies\UnityEditor 5.x
@<strong>{Mac OS X}（Unity4.x）    ~/Library/Preferences/com.unity3d.UnityEditor4.x.plist
@<strong>{Mac OS X}（Unity5.x）    ~/Library/Preferences/com.unity3d.UnityEditor5.x.plist
//}

EditorPrefs はメジャーバージョンごとに分けて保存されます。特に Windows はレジストリに値を保存します。EditorPrefs 経由「のみ」であれば問題はないのですが、直接レジストリを触ることもできてしまうので、その過程で誤った設定をしてしまい、最悪 Windows が起動しなくなるということもありえます。十分気をつけてください。

=== 使い方

OnEnable など 1度しか呼ばれないメソッド内のタイミングで値を取得します。値の変更タイミングで EditorPrefs に保存するようにしましょう。

//emlist{
using UnityEngine;
using UnityEditor;

public class ExampleWindow : EditorWindow
{
    int intervalTime = 60;
    const string AUTO_SAVE_INTERVAL_TIME = "AutoSave interval time (sec)";


    [MenuItem ("Window/Example")]
    static void Open ()
    {
        GetWindow <ExampleWindow> ();
    }

    void OnEnable ()
    {
        intervalTime = EditorPrefs.GetInt (AUTO_SAVE_INTERVAL_TIME, 60);
    }

    void OnGUI ()
    {
        EditorGUI.BeginChangeCheck ();

        // シーン自動保存間隔（秒）
        intervalTime = EditorGUILayout.IntSlider ("間隔（秒）", intervalTime, 1, 3600);

        if (EditorGUI.EndChangeCheck ())
            EditorPrefs.SetInt (AUTO_SAVE_INTERVAL_TIME, intervalTime);
    }
}
//}


また、ウィンドウのサイズを保存する場合は、それほど値の重要性も高くないので OnDisable で値の保存をするのが適しています。決して OnGUI で毎回保存しないようにしてください。OnGUI の様な、多く呼び出されるメソッドで書き込み作業をやると高負荷となってしまいます。

//emlist{
using UnityEngine;
using UnityEditor;

public class ExampleWindow : EditorWindow
{
    const string SIZE_WIDTH_KEY = "ExampleWindow size width";
    const string SIZE_HEIGHT_KEY = "ExampleWindow size height";
 
    [MenuItem ("Window/Example")]
    static void Open ()
    {
        GetWindow <ExampleWindow> ();
    }

    void OnEnable ()
    {
        var width = EditorPrefs.GetFloat (SIZE_WIDTH_KEY, 600);
        var height = EditorPrefs.GetFloat (SIZE_HEIGHT_KEY, 400);
        position = new Rect (position.x, position.y, width, height);
    }
    
    void OnDisable ()
    {
        EditorPrefs.SetFloat (SIZE_WIDTH_KEY, position.width);
        EditorPrefs.SetFloat (SIZE_HEIGHT_KEY, position.height);
    }
}
//}

== EditorUserSettings.Set/GetConfigValue

@<b>{プロジェクト内で共有できるデータの保存方法です。}ここで保存された値は暗号化されています。個人情報であるパスワードなどを保存するのに適しています。

=== 影響範囲 と 保存場所

この API で保存したデータは、プロジェクト内のみ影響します。データの保存先が @<code>{Library/EditorUserSettings.asset} であるため、Library フォルダーを他人と共有しない限りは情報が他人と共有されることはありません。@<fn>{1}


=== 何を保存するか

さまざまなツールを使っていると、ログインのためのメールアドレスやパスワードが必要になってきたりします。Oauth のアクセストークンなどもその1つです。

EditorUserSettings.asset はバイナリ形式で保存されており簡単には中身を見れないようになっています。とはいっても Unity が提供している @<b>{binary2text} を使用することでバイナリをテキスト形式に変換し、見ることができてしまうので注意してください。

=== 使い方

試しにデータを保存してみます。

//emlist{
using UnityEditor;

public class NewBehaviourScript
{
    [InitializeOnLoadMethod]
    static void SaveConfig ()
    {
        EditorUserSettings.SetConfigValue ("Data 1", "text");        
    }
}
//}

保存できたかを確かめてみましょう。EditorUserSettings.asset はバイナリ形式なのでテキスト形式にします。

//cmd{
cd /Applications/Unity/Unity.app/Contents/Tools
./binary2text /path/to/unityproject/Library/EditorUserSettings.asset
//}

値が、暗号化されて保存されていることがわかりました。

//emlist{
External References


ID: 1 (ClassID: 162) EditorUserSettings
    m_ObjectHideFlags 0 (unsigned int)
    m_ConfigValues  (map)
        size 2 (int)
        data  (pair)
            first "Data 1" (string)
            second "17544c12" (string)
        data  (pair)
            first "vcSharedLogLevel" (string)
            second "0a5f5209" (string)

    m_VCAutomaticAdd 1 (bool)
    m_VCDebugCom 0 (bool)
    m_VCDebugCmd 0 (bool)
    m_VCDebugOut 0 (bool)
    m_SemanticMergeMode 2 (int)
    m_VCShowFailedCheckout 1 (bool)
//}

== ScriptableObject

@<b>{プロジェクト内で共有できるデータの保存方法。}さまざまな応用が効く保存方法です。チーム内で共有したい設定や、大量のデータを保存したい場合にこの方法を選択します。


=== 影響範囲

ScriptableObject が Unity プロジェクト内でデータを保存するための主役となる形式です。Unity プロジェクト内に@<b>{アセット}として保存することでいつでもデータを保存でき、いつでもスクリプトからデータを読み込むことができます。

//image[ss02][インスペクターで値の編集を行うことができる]{

//}

//emlist{
using UnityEngine;
using UnityEditor;

[CreateAssetMenu]
public class NewBehaviourScript : ScriptableObject
{
    [Range(0,10)]
    public int number = 3;

    public bool toggle = false;

    public string[] texts = new string[5];
}
//}

=== 何を保存するか

エディター拡張で作成したアセットのデータや設定ファイル、ビルド後にゲームデータとして使うデータベースとしての役割を持たせることができます。

=== ScriptableObjectを保存する場所

Projectの Assets フォルダー以下であればどこにでも保存することができます。エディター拡張専用の ScriptableObject であれば Editor フォルダー以下に作成するのが好ましいです。

=== 使い方

情報量が多いので詳しくは @<chapref>{scriptableobject} で紹介します。


//footnote[1][他人との共有・バージョン管理では Library フォルダーは共有してはいけません。.meta ファイルを共有するようにしてください。]