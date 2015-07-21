= データの保存

//indepimage[frontispiece]
//pagebreak


== データ保存の種類

 * EditorPrefs
 * EditorUserSettings.Set/GetConfigValue
 * ScriptableObject

=== データを保存する

エディター拡張で何かを作成している時、値を保存して次回以降も使いまわしたいと言う時はありませんか？そのために用意された、データを保存するAPIが複数存在します。今回はそれらをすべて紹介し、どの場面で使えばいいかのアドバイスが出来ればと思います。


=== EditorPrefs

@<b>{PC内で共有できるデータの保存方法。}

==== 影響範囲

保存した値はメジャーバージョンごとの全Unity & 全プロジェクトに影響します（もちろんその値を使用するコードを書かなければ影響しません）。


//image[ss01][Unity4.x のみ影響。Unity5.xのみ影響というように分かれる]{
PCとUityとプロジェクトの影響が分かるような図
//}


==== 何を保存するか

EditorPrefs に保存すべきものは@<b>{ウインドウの位置・サイズ}、@<b>{Unityエディターの環境設定（Preferencesにあるような設定）}の値です。


==== EditorPrefsが保存されている場所

//table[tbl2][]{
プラットフォーム    場所
-----------------
@<strong>{Windows}（Unity4.x）    HKEY_CURRENT_USER\Software\Unity Technologies\UnityEditor 4.x
@<strong>{Windows}（Unity5.x）    HKEY_CURRENT_USER\Software\Unity Technologies\UnityEditor 5.x
@<strong>{Mac OS X}（Unity4.x）    ~/Library/Preferences/com.unity3d.UnityEditor4.x.plist
@<strong>{Mac OS X}（Unity5.x）    ~/Library/Preferences/com.unity3d.UnityEditor5.x.plist
//}

EditorPrefs は一箇所に保存されます。ただしUnityのメージャーバージョンごとにファイルは分かれます。

==== 使い方

=== EditorUserSettings.Set/GetConfigValue

@<b>{プロジェクト内で共有できるデータの保存方法。}



==== 影響範囲 と 保存場所

この API で保存したデータは プロジェクト内のみに影響します。

Library フォルダーを他人と共有しない限り@<fn>{1}影響範囲は個人のみとなります。

これはデータの保存先が @<code>{Library/EditorUserSettings.asset} であるためです。


==== 何を保存するか

開発時に個人情報ですが使用しなければならないメールアドレスやパスワードを保存します。状況によっては該当プロジェクトのみにツールの設定を保存したいときにも使用します。

EditorUserSettings.asset はバイナリ形式で保存されており簡単には中身を見れないようになっています。とはいっても Unity が提供している @<b>{binary2text} を使用することでバイナリをテキスト形式に変換し、見ることが出来てしまうので注意しましょう。

==== 使い方

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
/Applications/Unity/Unity.app/Contents/Tools/binary2text /path/to/unityproject/Library/EditorUserSettings.asset
//}

値は暗号化されていますが値が保存されていることがわかりました。

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

=== ScriptableObject

@<b>{プロジェクト内で共有できるデータの保存方法。}


==== 影響範囲

ScriptableObject が Unity プロジェクト内でデータを保存するための主役となる形式です。Unity プロジェクト内に@<b>{アセット}として保存することでいつでもデータを保存でき、いつでもスクリプトからデータを読み込むことが出来ます。


==== 何を保存するか

エディタ拡張で作成したアセットのデータや設定ファイル、ビルド後にゲームデータとして使うデータベースとしての役割を持たせることが出来ます。

==== ScriptableObjectを保存する場所

Projectの Assets フォルダ以下であればどこでも保存することが出来ます。エディタ拡張専用の ScriptableObject であれば Editor フォルダ以下に作成するのが好ましいです。

==== 使い方

ScriptableObject は情報量が多いので本章では全てを紹介せず @<chapref>{scriptableobject} で紹介します。


//footnote[1][推奨している他人との共有・バージョン管理では Library フォルダーは共有しないことになっています。]