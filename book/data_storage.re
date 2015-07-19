= データの保存

//indepimage[frontispiece]
//pagebreak


== データ保存の種類

 * EditorPrefs
 * EditorUserSettings.Set/GetConfigValue
 * ScriptableObject
 * 自前
 * JSON (Unity5.3から)

=== データを保存する

エディター拡張で何かを作成している時、値を保存して次回以降も使いまわしたいと言う時はありませんか。そのために用意された、データを保存するAPIが複数存在します。今回はそれらをすべて紹介し、どの場面で使えばいいかのアドバイスが出来ればと思います。


=== EditorPrefs

@<b>{PC内で共有できるデータの保存方法。}

==== 何を保存するか

EditorPrefs に保存すべきものは@<b>{ウインドウの位置・サイズ}、@<b>{Unityエディターの環境設定（Preferencesにあるような設定）}の値です。

==== EditorPrefsが保存されている場所

//table[tbl2][]{
プラットフォーム	場所
-----------------
@<strong>{Windows}（Unity4.x）	HKEY_CURRENT_USER\Software\Unity Technologies\UnityEditor 4.x
@<strong>{Windows}（Unity5.x）	HKEY_CURRENT_USER\Software\Unity Technologies\UnityEditor 5.x
@<strong>{Mac OS X}（Unity4.x）	~/Library/Preferences/com.unity3d.UnityEditor4.x.plist
@<strong>{Mac OS X}（Unity5.x）	~/Library/Preferences/com.unity3d.UnityEditor5.x.plist
//}

EditorPrefs は一箇所に保存されます。ただしUnityのメージャーバージョンごとにファイルは分かれます。

==== 影響範囲

保存した値はメジャーバージョンごとの全Unity & 全プロジェクトに影響します（もちろんその値を使用するコードを書かなければ影響しません）。


//image[ss01][Unity4.x のみ影響。Unity5.xのみ影響というように分かれる]{
PCとUityとプロジェクトの影響が分かるような図
//}

==== 使い方

=== EditorUserSettings.Set/GetConfigValue

@<b>{プロジェクト内で共有できるデータの保存方法。}


==== 何を保存するか

==== ConfigValueが保存されている場所

==== 影響範囲

==== 使い方

=== ScriptableObject

@<b>{プロジェクト内で共有できるデータの保存方法。}

==== 何を保存するか

==== ScriptableObjectを保存する場所

==== 影響範囲

==== 使い方

=== 自前

@<b>{プロジェクト内で共有できるデータの保存方法。}

==== 何を保存するか

==== 保存する場所

==== 影響範囲

==== 使い方

=== JSON

@<b>{プロジェクト内で共有できるデータの保存方法。}

==== 何を保存するか

==== 保存する場所

==== 影響範囲

==== 使い方