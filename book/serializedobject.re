
= SerializedObjectについて

Unityのオブジェクトは全てSerializedObjectに変換され保持されています。


== SerializedObjectとは


Unityのデータをファイル（以後アセット）として保存できるように変換したものです。
SerializedObjectからバイナリ形式、またはYAML形式のテキストファイルが生成されます。



みなさんは普段、アセット（AnimationClipなど）を扱っているように見えますが、Unity上では全て@<tt>{SerializedObject}を扱っていることになります。


== UnityEngine.Object


「Unityのオブジェクトは全て」と言いましたが、詳しく言うとUnityEngine.ObjectがSerializedObjectに変換され保持されます。



これから説明することはUnityEngine.Objectに言えることなのですが、イメージがしやすいように皆さんが普段使うものを例として説明していきます。


=== MonoBehaviour


MonoBehaviourを継承したクラス、つまりスクリプトのコンポーネントもSerializedObjectに変換されます。



普段、「@<strong>{インスペクターに変数の値を表示するにはpublicにする}」ということを目にしませんか？
また、「インスペクターに値を表示するにはpublicにしなければいけないので厄介だ」ということを目にする方も多いのではないでしょうか。

#@# lang: cs
//emlist{
public int hp; //インスペクター上でHPの設定を行う
//}


そもそも「インスペクターに変数の値を表示する」ということは、インスペクター上で設定した値をどこかで保持しておかなくてはいけません。



もちろん保持する役目はSerializedObjectですが、SerializedObjectが「どの値を保持すればいいか」という「@<strong>{印}」が必要になります。
その印の1つが「public」をつけることになります。



印はもう１つ提供されています。
変数に@<strong>{[SerializedField]}属性を付けることでSerializedObjectに保持されるようになります。


#@# lang: cs
//emlist{
[SerializedField]
private int hp; //インスペクター上でHPの設定を行う
//}


なのでシリアライズするデータを決める、つまりインスペクターに表示するために必ずしもpublicにする必要はありません。

=== ScriptableObject


UnityEngine.Objectがシリアライズされアセットとして保存されますが、ユーザーが独自の（UnityEngine.Objectを継承した）オブジェクトを作成し、アセットとして保存するために@<strong>{独自アセットを作成できる}仕組みが提供されています。



#@# 詳しい説明は長くなってしまうので@<chapref>{keigo-content23}をご覧ください。


=== Editor (クラス)


EditorクラスはScriptableObjectを継承したクラスになります。



SerializedObjectは、Editorクラスを継承したCustomEditorでも扱われています。



MonodevelopのアセンブリブラウザでEditorクラスを見ると、target(s)に対するserializedObjectを生成していることがわかります。



//indepimage[ss01][]




また、内部的な処理としてシリアライズされたものをキャッシュする仕組みがあるので、1度SerializedObjectを生成するとキャッシュされ、new SerializedObjectでインスタンスを生成した時、自動的にキャッシュからロードされます。


=== EditorWindow


EditorWindowクラスもScriptableObjectを継承したクラスになります。



EditorとEditorWindowはともにScriptableObjectを継承しているため、アセットとして保存することが可能です。


== SerializedObjectの使い方

=== SerializedObjectからパラメーターを取得する


シリアライズされたデータはSerializedPeopertyとして取得することが可能です。
イテレーターとして取得することも可能ですし、パスを指定して特定のものを取得することもできます。



例えば「変数Vector3型position」の値を取得したい時、


#@# lang: cs
//emlist{
serializedObject.FindProperty ("position").vector3Value;
//}


「変数Hoge型のhoge内にある変数string型fuga」の値を取得したい時、


#@# lang: cs
//emlist{
serializedObject.FindProperty ("hoge.fuga").stringValue;
//}


「変数stringの配列で2番目」の値を取得したい時


#@# lang: cs
//emlist{
serializedObject.FindProperty ("names").GetArrayElementAtIndex(1);
//}

=== 最新のデータを取得、更新


SerializedObjectは内部でキャッシュされており、（ユーザー側で）インスタンス化したものはそのキャッシュから引っ張ってきたものとなります。



例えば、オブジェクトに対するSerializedObjectが２つ存在した場合、古いままの情報で更新してしまわないように、その2つは常に最新の情報を保っておくべきです。


それを解決するためにUnityには2つのAPIが提供されています。


==== Update


内部キャッシュから最新のデータを取得します。
何かしらのオブジェクトのViewer的なものを作成するのであればUpdateで常に最新の情報を表示させてください。


#@# lang: cs
//emlist{
serializedObject.Update ();
Debug.Log ("現在のHP: " + serializedObject.FindProperty ("hp").intValue);
//}

==== ApplyModifiedProperties


内部キャッシュへ変更点を適用します。
インスペクターなどでユーザーに操作をさせる場合、Updateで常に最新の情報にし、それを元に変更を適用させることをお勧めします。
（参考にする値が古い値だったということは、あってはならないですよね）


#@# lang: cs
//emlist{
// 最新の情報を取得する。Updateを呼び出さないとhpの値はいつまでたっても同じまま
serializedObject.Update ();

EditorGUILayout.LabelField ("現在のHP: " + serializedObject.FindProperty ("hp").intValue);

// 最新の情報を元に値の変更をさせる
if(GUILayout.Button("値を変更する")) {
    serializedObject.FindProperty ("hp").intValue = 100;
}

// 変更された値を適用する
serializedObject.ApplyModifiedProperties();
//}

== そもそもシリアライズの仕組みってどうなっているの


それについてはUnityのブログで解説が行われています。



Unityのシリアライゼーション - @<href>{http://japan.unity3d.com/blog/?p=1630,http://japan.unity3d.com/blog/?p=1630}


こちらで、参照ループに陥った時に、ネストは7回までというような、知っておくべき仕様も書かれています。

== 最後に

SerializedObjectはUnityではなくてはならない存在です。複雑なEditor拡張を行う場合にはSerializedObjectの知識は必ず必要になります。