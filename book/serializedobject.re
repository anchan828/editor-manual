= SerializedObjectについて


Unity上では、ファイル（Unity上ではアセット）を少し特殊な形式へと変換して使用します。本章では、Unityでオブジェクトを扱う時に基盤となる SerializedObject について解説していきます。また、「シリアライズについて@<b>{すべてを網羅した}情報」は、Unity公式のマニュアルにて説明されています。本章では入門として知っておいて欲しい情報を抜粋して解説します。@<href>{http://docs.unity3d.com/Manual/script-Serialization.html}

== SerializedObjectとは

SerializedObject は、シリアライズされたデータを Unity が扱いやすいように加工したものです。これにより、さまざまなデータにアクセスが可能になったり、Undo 処理やゲームオブジェクトからプレハブが容易に作成できるようになっています。

SerializedObject は、Unity上で扱う全てのオブジェクトに関係しています。普段扱っているアセット（マテリアルやテクスチャ、アニメーションクリップ等）も SerializedObject がなくては作成できません。


=== UnityEngine.Object と SerializedObject の関係


Unityエディター上では、全てのオブジェクト（UnityEngine.Object）は SerializedObject に変換されて扱われています。インスペクターでコンポーネントの値を編集している時も、Component のインスタンスを編集しているわけではなく、SerializedObject のインスタンスを編集していることになります。

//image[ss03][Unityエディター上では必ず SerializedObject 経由で値を編集することになる。ただし CustomEditor を実装している場合はその限りではない。]{

//}

Unityエディター上、つまりエディター拡張では@<b>{できるだけ}すべてのオブジェクト操作を SerializedObject で行っていく必要があります。この理由として SerializedObject では、シリアライズされたデータを扱うだけではなく @<b>{Undo} や @<b>{Selection} のハンドリングも行っています。

: Undo のハンドリング
  SerializedObject で値を編集する時、Undo処理は意識せずとも登録されます。UnityEngine.Object のインスタンスを直接編集した場合は、Undo 処理を独自で実装しなければいけません。Undo について詳しくは@<chapref>{undo}をご覧ください。

: Selection のハンドリング
 プロジェクトウィンドウでアセットを選択した時、即座にデシリアライズして UnityEngine.Object のインスタンスを取得し、インスペクターに値を表示します。このハンドリングで主に役立つのが、複数のオブジェクトを選択した時に、シリアライズされたプロパティーの同時編集を可能にする仕組みです。

このように Unity でオブジェクトを扱う上で便利な機能を包括しています。もし、SerializedObject 経由でオブジェクトを扱わない場合、Undo や Selection のハンドリングを自分で実装しなければいけません。この2つのハンドリングについては@<chapref>{customeditor}にて説明しています。また、この章の後半でも軽く説明します。




=== アセットと SerializedObject の関係

UnityEngine.Object をアセットとして保存する場合、@<b>{バイナリ形式}、または @<b>{YAML 形式のテキストデータ}として保存されます。これらのシリアル化を担うのが SerializedObject です。

単純なイメージとしては@<img>{ss02} となります。UnityEngine.Object をアセットとして保存するには SerializedObject に一度変換します。次に、変換された SerializedObject はアセットと @<b>{.meta} ファイルの作成を試みます。

//image[ss02][データの流れ]{

//}

==== アセットと .meta ファイル

SerializedObject ではアセットと @<b>{.meta} ファイルの2つを作成します。アセットは実際のオブジェクトがシリアル化されたものです。@<b>{.meta} ファイルはインポーターの設定などを保存します。

試しに下記のコードを試してみましょう。

//emlist{
[InitializeOnLoadMethod]
static void CheckPropertyPaths ()
{
	var so = new SerializedObject (Texture2D.whiteTexture);

	var pop = so.GetIterator ();

	while (pop.NextVisible (true))
		Debug.Log (pop.propertyPath);

}
//}

ログに表示されるものは下記になります。

//emlist{
m_ImageContentsHash.bytes[0]
m_ImageContentsHash.bytes[1]
.
.
.
m_IsReadable
m_TextureSettings
m_ColorSpace
//}

このように Texture2D オブジェクトは SerializedObject に変換した時にはインポーターの設定も持っていることがわかります。Texture2D をアセットとして保存する場合は、これらの設定をディスク上にあるテクスチャ（jpg や png）に書き込むわけにもいきませんので、.meta ファイルに書き出す仕様となっています。


また逆に、アセットをインポートするときは、アセットと .meta ファイル（.metaファイルがなければデフォルト設定で自動生成）から SerializedObject が生成され、UnityEngine.Object へと変換されます。

=== シリアライズ対象のクラス変数


UnityEngine.Object の派生クラス（ユーザーがよく触るのは MonoBehaviour や ScriptableObject、Editor、EditorWindow など）にて、シリアライズ対象のフィールドであると判断させるには条件があります。


 * public 変数であること、または SerializeField 属性の付いたフィールドであること
 * シリアライズ可能な Unity がサポートしているタイプであること （sbyte、short、int、long、byte、ushort、uint、ulong、float、double、bool、char、string、UnityEngine.Object、Serializable 属性を付加したクラスと構造体、等）


さらに細かく上げると以下の2つも挙げられます。

 * 変数が、static、const、readonly ではないこと
 * abstract クラスではないこと


よく入門書などで、「@<strong>{インスペクターに変数の値を表示するにはpublicにする}」ということを目にしたかもしれません。これはプログラマー以外でも理解しやすいように言っているだけで、public 変数にするのはシリアライズ対象の条件の1つでしかありません。エディター拡張を行うユーザーは private フィールドに SerializeField 属性を付けることをお勧めします。


//emlist{
[SerializeField]
private string m_str;

public string str {
	get {
		return m_str;
	}
	set {
		m_str = value;
	}
}
//}

外部から SerializeField 属性の付いたフィールドにアクセスするときは @<b>{SerializedObject} 経由でアクセスします。

== SerializedObjectの使い方

本格的な実装例は@<chapref>{customeditor}で紹介しています。ここでは、SerializedObject を上手く扱うための API を紹介していきます。


=== SerializedObjectからパラメーターを取得する


シリアライズされたデータは SerializedPeoperty として取得することが可能です。

イテレーターとして取得することが可能で、本章の前半で紹介したプロパティーの一覧をログに表示するコードは、イテレーターを使って操作可能な全プロパティーを取得しています。

//emlist{
[InitializeOnLoadMethod]
static void CheckPropertyPaths ()
{
	var so = new SerializedObject (Texture2D.whiteTexture);

	var pop = so.GetIterator ();

	while (pop.NextVisible (true))
		Debug.Log (pop.propertyPath);

}
//}

また、パスを指定して特定の SerializedPeoperty を取得することも可能です。

例えば「Vector3型 position 変数」の値を取得したい時、

//emlist{
public class Hoge : MonoBehaviour
{
    [SerializeField] Vector3 position;
}
//}


#@# lang: cs
//emlist{
var hoge = /* さまざまな方法で Hoge コンポーネントを取得 */;

var serializedObject = new SerializedObject(hoge);
serializedObject.FindProperty ("position").vector3Value;
//}


「Fuga 型の fuga 変数内にある string 型 bar 変数」の値を取得したい時、

//emlist{
[System.Serializable]
public class Fuga
{
	[SerializeField] string bar;
}
//}

//emlist{
public class Hoge : MonoBehaviour
{
    [SerializeField] Fuga fuga;
}
//}

#@# lang: cs
//emlist{
var hoge = /* さまざまな方法で Hoge コンポーネントを取得 */;

var serializedObject = new SerializedObject(hoge);
serializedObject.FindProperty ("fuga.bar").stringValue;
//}


「stringの配列で2番目」の値を取得したい時

//emlist{
public class Hoge : MonoBehaviour
{
    [SerializeField] string[] names;
}
//}

#@# lang: cs
//emlist{
var hoge = /* さまざまな方法で Hoge コンポーネントを取得 */;

var serializedObject = new SerializedObject(hoge);
serializedObject.FindProperty ("names").GetArrayElementAtIndex(1);
//}

=== 最新のデータを取得、更新


SerializedObject は内部でキャッシュされており、インスタンス化した時に、既にキャッシュされていればキャッシュから引っ張ってきます。



例えば、エディターウィンドウとインスペクター内部でそれぞれ1つのオブジェクトに対する SerializedObject を生成した場合、2つの SerializedObject を同期しなければ、どちらかが古いままの情報で更新してしまうことがあるかもしれません。

//image[ss04][同じ UnityEngine.Object を SerializedObject に変換したもの]{

//}

このように、オブジェクトに対する SerializedObject が2つ存在した場合、片方が古いままの情報で更新してしまわないように、2つの SerializedObject は常に最新の状態に保っておくべきです。


それを解決するために、2つのAPIが提供されています。


==== Update


内部キャッシュから最新のデータを取得します。
常に最新の情報を扱うために、SerializedObject にアクセスする前に呼び出してください。

//emlist{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript : Editor
{
	public override void OnInspectorGUI ()
	{
		serializedObject.Update ();

		EditorGUILayout.PropertyField (serializedObject.FindProperty ("name"));
	}
}
//}

==== ApplyModifiedProperties


内部キャッシュに変更点を適用します。
先ほどの Update で常に最新の情報にし、変更点を ApplyModifiedProperties で適用します。これで1セットと考えてください。

特に変更点を適用するための条件がない場合は、Update をメソッドの最初の行に、ApplyModifiedProperties をメソッドの最後の行に記述します。

//emlist{
using UnityEngine;
using UnityEditor;

public class NewBehaviourScript : Editor
{
	public override void OnInspectorGUI ()
	{
		serializedObject.Update ();

		EditorGUILayout.PropertyField (serializedObject.FindProperty ("name"));

		// 他、さまざまな処理

		serializedObject.ApplyModifiedProperties ();
	}
}
//}


逆に言えば、Update を行わない限りは外部からの変更でプロパティーの反映はされませんし、また ApplyModifiedProperties を行わない限りは外部に適用しません。

== 複数の UnityEngine.Object を1つの SerializedObject で扱う

SerializedObject のコンストラクタで配列を渡すだけで複数の UnityEngine.Object を扱うことができます。@<b>{ただし、引数として渡せるものは同じ型のみです。}もし異なる型のオブジェクトを引数として渡した場合、キーマップが一致せずにエラーが発生します。

//emlist{
// 複数のリジッドボディ
Rigidbody[] rigidbodies = /* さまざまな方法で Rigidbody コンポーネントを取得 */;

var serializedObject = new SerializedObject(rigidbodies);

serializedObject.FindProperty ("m_UseGravity").boolValue = true;
//}

== プロパティー名を知るには

SerializedProperty にアクセスするには、プロパティーのパスを知らなければいけません。自分で作成した MonoBehaviour コンポーネントにアクセスする場合は@<b>{プロパティーのパス}はスクリプトファイルを見ればすぐ分かります。Unity側で実装している コンポーネントや UnityEngine.Object 関連のプロパティーは、プロパティー名に @<b>{m_} が付いている場合があります。@<b>{m_} はインスペクター上では省かれてプロパティー名として表示されるため、実際のプロパティを把握するのが難しくなっています。また、インスペクターに表示されるプロパティ名と実際のプロパティ名が一致しない場合があります。

プロパティーを知る方法は大きく分けて 2 パターンあります。

=== SerializedObject.GetIterator

イテレーターを使ってプロパティー名を総なめする方法です。これは本章の最初で紹介しています。


=== アセットをテキストエディタで見る

対象がコンポーネントであれば、@<b>{Asset Serialization を Force Text} に設定した上でプレハブにし、テキストエディターでプレハブを開きます。

YAML 形式のデータを見ることができ、そこにプロパティー名が記載されています。

//emlist{
%YAML 1.1
%TAG !u! tag:unity3d.com,2011:
--- !u!1 &113998
GameObject:
  m_ObjectHideFlags: 0
  m_PrefabParentObject: {fileID: 0}
  m_PrefabInternal: {fileID: 100100000}
  serializedVersion: 4
  m_Component:
  - 4: {fileID: 442410}
  - 54: {fileID: 5488994}

... 略 ...

--- !u!54 &5488994
Rigidbody:
  m_ObjectHideFlags: 1
  m_PrefabParentObject: {fileID: 0}
  m_PrefabInternal: {fileID: 100100000}
  m_GameObject: {fileID: 113998}
  serializedVersion: 2
  m_Mass: 1
  m_Drag: 0
  m_AngularDrag: .0500000007
  m_UseGravity: 1
  m_IsKinematic: 0
  m_Interpolate: 0
  m_Constraints: 0
  m_CollisionDetection: 0
//}

また、マテリアルなどの Unity独自のアセットもテキストエディタで見ることができます。

豆知識として、UnityEditorInternal名前空間にある@<b>{InternalEditorUtility.SaveToSerializedFileAndForget}で UnityEngine.Object をアセットとして保存できます。

//emlist{
using UnityEngine;
using UnityEditorInternal;
using UnityEditor;

public class NewBehaviourScript : MonoBehaviour
{
	void Start ()
	{
		var rigidbody = GetComponent<Rigidbody> ();

		InternalEditorUtility.SaveToSerializedFileAndForget (
		    new Object[]{ rigidbody },
		    "Rigidbody.yml",
		    true);
	}
}
//}
