= CustomEditor

カスタムエディターは、インスペクターやシーンビューに表示されているGUIをカスタマイズするためのものです。本章ではカスタムエディターの基本的な使い方だけでなく、インスペクターの仕組みについても紹介していきます。



== インスペクターの Debug モード

#@# CustomEditor は、@<b>{Editorオブジェクトを独自でカスタマイズしたもの}になります。
例えば、Cube を作成し、インスペクターを見ると BoxCollider や MeshRenderer などのコンポーネントがアタッチされていることがわかります。

//image[ss07][Cubeを作成し、Cubeを選択している状態]{

//}

その時に、インスペクターのタブ部分、または「 ≡ 」をクリックすると@<img>{ss04}のようにコンテキストメニューが表示され「Normal」と「Debug」項目があります。

//image[ss04][通常は Normal にチェックが付いている]{

//}

Debugを選択するとインスペクターに変化が現れます。@<img>{ss03}のように普段見ているインスペクターとは少し違う見た目になりました。

//image[ss03][普段見ない Instance ID や File ID のプロパティが見える]{

//}

Debug モードでは、GUI 要素がカスタマイズされる前の@<b>{素}の状態に戻ります。@<fn>{1}このように Unity は、インスペクターに表示したい GUI 要素を取捨選択し、カスタマイズして表示します。

では、カスタマイズをするために「インスペクターに表示したい GUI 要素を取捨選択」する部分は一体どこなのかを知らなくてはいけません。

== オブジェクトと Editor クラス

@<href>{http://docs.unity3d.com/ScriptReference/Editor.html, Editor}クラスは、オブジェクトの情報をインスペクターやシーンビューに表示するための機能です。各オブジェクトに対応した Editor オブジェクトが生成され、Editor オブジェクトを介して必要な情報を GUI で表示することが出来るようになります。


//image[ss06][ボックスコライダーを Editor オブジェクトを介して GUI を表示するイメージ]{

//}

その時に、全ての要素を見せるのは必要なかったり、追加の GUI 要素を表示したいということもあるので @<b>{CustomEditor（カスタムエディター）}の機能を使い Editor オブジェクトをカスタマイズすることが出来ます。


== カスタムエディターを使う

例えば、ゲーム中の実際の攻撃力は、キャラクターのちからや武器の強さで計算されて決まるとします。そのときにプログラム上で「攻撃力」というプロパティーを持ち、 getter で計算を行ってみましょう。プログラム上これでいいのですが、攻撃力の値を Unity エディターのインスペクターで確認したい場合は少し困ったことになります。Unityのインスペクターは、シリアライズ可能なフィールドを表示します。setter があってもシリアライズ対象ではないプロパティーは表示されません。

//image[ss01][下記コンポーネントをインスペクターで見た図]{

//}


//quote{
ソースコードは計算式がちょっとだけ分かりやすいように日本語変数にしてみました。
//}

//emlist[][cs]{
using UnityEngine;

public class Character : MonoBehaviour
{
	[Range (0, 255)]
	public int 基本攻撃力;
	[Range (0, 99)]
	public int 剣の強さ;
	[Range (0, 99)]
	public int ちから;

	// プレイヤーの能力と、剣の強さから攻撃力を求めるプロパティ
	public int 攻撃力 {
		get { 
			return 基本攻撃力 + Mathf.FloorToInt (基本攻撃力 * (剣の強さ + ちから - 8) / 16);
		}
	}
}
//}




今回は、@<img>{ss02}のようにプロパティである攻撃力をインスペクター上に表示して、確認しながらパラメーターを調整していきます。

//image[ss02][]{

//}

=== Editor クラスの派生クラスを作成

Editor クラスの派生クラスを作成した後、Character コンポーネントに対しての Editor クラスとして CustomEditor 属性を付加します。これでカスタムエディターでカスタマイズする準備が整いました。

//emlist{
using UnityEngine;
using UnityEditor;

[CustomEditor (typeof(Character))]
public class CharacterInspector : Editor
{
}
//}

=== インスペクターの GUI のカスタマイズ

インスペクターの GUI は OnInspectorGUI をオーバーライドすることでカスタマイズすることが出来ます。

//emlist{
using UnityEngine;
using UnityEditor;

[CustomEditor (typeof(Character))]
public class CharacterInspector : Editor
{
	Character character = null;

	void OnEnable ()
	{
	    // Character コンポーネントを取得
		character = (Character) target;
	}

	public override void OnInspectorGUI ()
	{
		base.OnInspectorGUI ();

		// 攻撃力の数値をラベルとして表示する
		EditorGUILayout.LabelField ("攻撃力", character.攻撃力.ToString ());
	}
}
//}

これで @<img>{ss02} と同じ表示を行うことが出来ました。

=== シーンビューの GUI のカスタマイズ

シーンビューの GUI は OnSceneGUI を使うことでカスタマイズすることが出来ます。これについては @<chapref>{handles} にて詳しく説明しているので、そちらをご覧ください。

== カスタムエディターでデータのやり取り

カスタムエディターからコンポーネントの値にアクセスする方法は 2 種類あります。「Unityのシリアライズ機構を通してアクセスする方法」と「コンポーネントに直接アクセスする方法」です。

//image[ss08][コンポーネントの値にアクセスするときのイメージ]{

//}

2種類の方法を説明していきます。その時には、以下のコンポーネントがあり、このカスタムエディターを作成するものとして話を進めていきます。

//emlist{
using UnityEngine;

public class Character : MonoBehaviour
{
	public int hp;
}
//}

=== Unityのシリアライズ機構を通してアクセスする方法

Unity はデータの持ち方として SerializedObject で、すべてのデータを管理しています。SerializedObject 経由でデータにアクセスすることでデータの操作に柔軟に対応することが出来ます。SerializedObject の詳しい説明は @<chapref>{serializedobject} をご覧ください。

Editor オブジェクトが生成されると同時に、コンポーネントがシリアライズされ、serializedObject 変数に格納されます。serializedObject 変数から各プロパティーにアクセスすることが出来ます。

下記のコードのように、「@<b>{SerializedPropertyにアクセスする前}は必ず SerializedObject を最新に更新」します。これは同じコンポーネントの SerializedObject が他の場所で更新された場合に、その変更点を適用するためです。

「@<b>{SerializedPropertyにアクセスした後}は必ずプロパティーの変更点を SerializedObject に適用」します。これによりデータの保存処理が実行されます。

//emlist{
using UnityEngine;
using UnityEditor;

[CustomEditor (typeof(Character))]
public class CharacterInspector : Editor
{
	SerializedProperty hpProperty;

	void OnEnable ()
	{
		hpProperty = serializedObject.FindProperty ("hp");
	}

	public override void OnInspectorGUI ()
	{
		serializedObject.Update ();

		EditorGUILayout.IntSlider (hpProperty, 0, 100);

		// どちらも同じ処理
		//hpProperty.intValue = EditorGUILayout.IntSlider ("Hp", hpProperty.intValue, 0, 100);

		serializedObject.ApplyModifiedProperties ();
	}
}
//}

=== コンポーネントに直接アクセスする方法

この方法は本章の前半で行っています。コンポーネントに直接アクセスすることで、GUI のカスタマイズを簡単に行うことが可能です。

対象のコンポーネントは Editor オブジェクトの target 変数でアクセスすることが出来ます。 UnityEngine.Object 型なのでキャストする必要があります。

//emlist{
using UnityEngine;
using UnityEditor;

[CustomEditor (typeof(Character))]
public class CharacterInspector : Editor
{
	Character character;

	void OnEnable ()
	{
		character = (Character) target;
	}

	public override void OnInspectorGUI ()
	{
		character.hp = EditorGUILayout.IntSlider ("Hp", character.hp, 0, 100);
	}
}
//}

==== Undo を実装すること

コンポーネントに直接アクセスする方法はとても楽な方法です。文字列でプロパティーにアクセスする SerializedObject と比べると、typo などケアレスミスも少なくなります。ですがこの方法で、エディター拡張を「きちんと」実装するには Undo 処理を実装しなければいけません。Undo の詳しい説明は @<chapref>{undo} をご覧ください。


//emlist{
Character character;

void OnEnable ()
{
	character = (Character) target;
}

public override void OnInspectorGUI ()
{
	EditorGUI.BeginChangeCheck ();

	var hp = EditorGUILayout.IntSlider ("Hp", character.hp, 0, 100);
	
	if (EditorGUI.EndChangeCheck ()) {

		// 変更前に Undo に登録
		Undo.RecordObject (character, "Change hp");

		character.hp = hp;

		// プロパティーが変更されたことを通知
		EditorUtility.SetDirty (character);
	}
}
//}

== 複数コンポーネントの同時編集

Unity ではゲームオブジェクトを複数選択し@<b>{同時に}同じプロパティーの値を編集することが出来ます。ただし、同時編集が出来るのは同時編集を許可されたコンポーネントのみです。

//image[ss09][同時編集が許可されていないコンポーネント]{

//}

カスタムエディターを実装していない通常のコンポーネントでは@<b>{デフォルトで}同時編集が可能ですが、カスタムエディターを実装したコンポーネントでは同時編集できるようになっていません。

=== CanEditMultipleObjects

同時編集を可能にするには CanEditMultipleObjects 属性を Editor の派生クラスに付加する必要があります。

//emlist{
using UnityEngine;
using UnityEditor;

[CanEditMultipleObjects]
[CustomEditor (typeof(Character))]
public class CharacterInspector : Editor
{
}
//}

CanEditMultipleObjects 属性をつけることで同時編集が可能になる準備が出来ました。

==== Unityのシリアライズ機構を通しての同時編集

SerializedObject を通して編集をしている場合は CanEditMultipleObjects 属性を付加するだけで、SerializedObject 側で同時編集に対応します。

//emlist{
using UnityEngine;
using UnityEditor;

[CanEditMultipleObjects]
[CustomEditor (typeof(Character))]
public class CharacterInspector : Editor
{
	SerializedProperty hpProperty;

	void OnEnable ()
	{
		hpProperty = serializedObject.FindProperty ("hp");
	}

	public override void OnInspectorGUI ()
	{
		serializedObject.Update ();

		EditorGUILayout.IntSlider (hpProperty, 0, 100);

		serializedObject.ApplyModifiedProperties ();
	}
}
//}


==== コンポーネントに直接アクセスしての同時編集

同時編集を可能にするには複数のコンポーネントにアクセスしなければいけません。複数選択した場合、 target 変数ではなく @<code>{targets} 変数を使用します。targets に現在選択中で対象のコンポーネント全てが格納されています。

複数選択した時にインスペクターに表示されるものは、最初に選択した（ゲームオブジェクトにアタッチされている）コンポーネントです。これは target に格納されており、また targets の1番目の要素でもあります。

選択したコンポーネントの各プロパティーが全て同じ値ということもあれば、異なる場合もあります。
同じ値でない場合は Unity は 「@<b>{-} 」を表示して、異なる値が代入されていると表現することが出来ます。	

//image[ss10][複数選択時、左が同じ値の場合。右が異なる値の場合。]{

//}

「@<b>{-} 」を表示する仕組みは、コンポーネントに直接アクセスしている場合は自動で適用されません。自分で実装する必要があります。@<code>{EditorGUI.showMixedValue} の static 変数があり、GUI のコードの前に true を設定することで「異なる値である」と表現することが可能です。


以下のコードが上記で説明したことを全て含めたコードとなります。

//emlist{
using UnityEngine;
using UnityEditor;
using System.Linq;

[CanEditMultipleObjects]
[CustomEditor (typeof(Character))]
public class CharacterInspector : Editor
{
	Character[] characters;

	void OnEnable ()
	{
		characters = targets.Cast<Character> ().ToArray ();
	}

	public override void OnInspectorGUI ()
	{
		EditorGUI.BeginChangeCheck ();

		// 異なる値が 2 以上であれば true
		EditorGUI.showMixedValue = characters.Select (x => x.hp).Distinct ().Count () > 1;

		var hp = EditorGUILayout.IntSlider ("Hp", characters [0].hp, 0, 100);

		EditorGUI.showMixedValue = false;

		if (EditorGUI.EndChangeCheck ()) {

			// 全てのコンポーネントを Undo に登録
			Undo.RecordObjects (characters, "Change hp");

			// 全てのコンポーネントに値を代入して更新
			foreach (var character in characters) {
				character.hp = hp;
				EditorUtility.SetDirty (character);
			}
		}
	}
}
//}

== カスタムエディター内で PropertyDrawer を使用する

カスタムエディター内でも PropertyDrawer を使用することが出来ます。使い方は @<code>{EditorGUILayout.PropertyField} に対象の SerializedProperty を渡すだけです。PropertyDrawer の詳しい説明は @<chapref>{property_drawer} をご覧ください。

@<img>{ss11} のような PropertyDrawer を作成し、カスタムエディター内で表示してみます。

//image[ss11][MinMaxSlider]{

//}

まずは PropertyDrawer 対象となる Example クラスを作成し、Character クラスに変数として記述します。

//emlist{
[System.Serializable]
public class Example
{
	public int minHp;
	public int maxHp;
}
//}

//emlist{
using UnityEngine;

public class Character : MonoBehaviour
{
	public Example example;
}
//}

次に、Example クラスの PropertyDrawer を作成します。MinMaxSlider の実装と、それぞれの値をラベルとして表示しています。

//emlist{
[CustomPropertyDrawer (typeof(Example))]
public class ExampleDrawer : PropertyDrawer
{
	public override void OnGUI (Rect position, SerializedProperty property, GUIContent label)
	{
		using (new EditorGUI.PropertyScope (position, label, property)) {

			var minHpProperty = property.FindPropertyRelative ("minHp");
			var maxHpProperty = property.FindPropertyRelative ("maxHp");


			var minMaxSliderRect = new Rect (position) { 
				height = position.height * 0.5f 
			};

			var labelRect = new Rect (minMaxSliderRect) { 
				x = minMaxSliderRect.x + EditorGUIUtility.labelWidth,
				y = minMaxSliderRect.y + minMaxSliderRect.height 
			};

			float minHp = minHpProperty.intValue;
			float maxHp = maxHpProperty.intValue;

			EditorGUI.BeginChangeCheck ();

			EditorGUI.MinMaxSlider (label, minMaxSliderRect, ref minHp, ref maxHp, 0, 100);

			EditorGUI.LabelField (labelRect, minHp.ToString (), maxHp.ToString ());

			if (EditorGUI.EndChangeCheck ()) {
				minHpProperty.intValue = Mathf.FloorToInt (minHp);
				maxHpProperty.intValue = Mathf.FloorToInt (maxHp);
			}
		}
	}

	public override float GetPropertyHeight (SerializedProperty property, GUIContent label)
	{
		return base.GetPropertyHeight (property, label) * 2;
	}
}
//}

あとはカスタムエディター内で使用するだけです。

//emlist{
using UnityEngine;
using UnityEditor;

[CanEditMultipleObjects]
[CustomEditor (typeof(Character))]
public class CharacterInspector : Editor
{
	SerializedProperty exampleProperty;

	void OnEnable ()
	{
		exampleProperty = serializedObject.FindProperty ("example");
	}

	public override void OnInspectorGUI ()
	{
		serializedObject.Update ();

		EditorGUILayout.PropertyField (exampleProperty);

		serializedObject.ApplyModifiedProperties ();
	}
}
//}

このように、細かく部品とすることができるものは PropertyDrawer として実装すると、複雑なスパゲティーコード@<fn>{2}にならずに済むかもしれません。これは冗談ではなく、GUI の描画のためのコードは冗長になりがちなのでオススメです。


=== インスペクターのプレビュー画面

//emlist{
using UnityEngine;
using UnityEditor;

[CustomEditor (typeof(Character))]
public class CharacterInspector : Editor
{
	SerializedProperty exampleProperty;
	PreviewRenderUtility previewRenderUtility;
	GameObject previewObject;

	void OnEnable ()
	{
		previewRenderUtility = new PreviewRenderUtility (true);
		previewRenderUtility.m_CameraFieldOfView = 30f;

		var character = (Character)target;
		previewObject = character.gameObject;
	}

	void OnDisable ()
	{
		previewRenderUtility.Cleanup ();
		previewRenderUtility = null;
		previewObject = null;
	}

	public override bool HasPreviewGUI ()
	{
		return true;
	}

	public override void OnInteractivePreviewGUI (Rect r, GUIStyle background)
	{
		previewRenderUtility.BeginPreview (r, background);
		previewRenderUtility.m_Camera.transform.position = previewObject.transform.position + Vector3.forward * -5;
		previewRenderUtility.m_Camera.transform.LookAt (previewObject.transform);
		previewRenderUtility.m_Camera.Render ();
		Repaint ();
		previewRenderUtility.EndAndDrawPreview (r);
	}

}

//}


//emlist{
using UnityEngine;
using UnityEditor;

[CustomEditor (typeof(Character))]
public class CharacterInspector : Editor
{
	SerializedProperty exampleProperty;
	PreviewRenderUtility previewRenderUtility;
	GameObject previewObject;
	Vector3 centerPosition;

	void OnEnable ()
	{
		var previewLayer = LayerMask.NameToLayer ("Preview");
		previewRenderUtility = new PreviewRenderUtility (true);
		previewRenderUtility.m_CameraFieldOfView = 30f;
		previewRenderUtility.m_Camera.cullingMask = 1 << previewLayer;

		var character = (Character)target;
		previewObject = Instantiate (character.gameObject);
		previewObject.hideFlags = HideFlags.HideAndDontSave;

		previewObject.layer = previewLayer;
		foreach (Transform transform in previewObject.transform) {
			transform.gameObject.layer = previewLayer;
		}


		centerPosition = GetCenterPosition ();


		previewObject.SetActive (false);

	}

	Vector3 GetCenterPosition(){
		var currentMode = Tools.pivotMode;
		Tools.pivotMode = PivotMode.Center;
		var centerPosition = Tools.handlePosition;
		Tools.pivotMode = currentMode;
		return centerPosition;
	}

	void OnDisable ()
	{
		DestroyImmediate (previewObject);
		previewRenderUtility.Cleanup ();
		previewRenderUtility = null;
		UnityEditorInternal.InternalEditorUtility.RepaintAllViews ();
	}

	public override bool HasPreviewGUI ()
	{
		return true;
	}

	public override void OnInteractivePreviewGUI (Rect r, GUIStyle background)
	{
		previewRenderUtility.BeginPreview (r, background);

		var drag = Vector2.zero;

		if (Event.current.type == EventType.MouseDrag) {
			drag = Event.current.delta;
		}

		previewRenderUtility.m_Camera.transform.position = centerPosition + Vector3.forward * -5;
	
		previewObject.transform.RotateAround (centerPosition, Vector3.up, -drag.x);
		previewObject.transform.RotateAround (centerPosition, Vector3.right, -drag.y);

		previewObject.SetActive (true);
		previewRenderUtility.m_Camera.Render ();
		previewObject.SetActive (false);

		previewRenderUtility.EndAndDrawPreview (r);

		if (drag != Vector2.zero)
			Repaint ();
	}
}
//}

//footnote[1][Unity内部で非表示設定となっている場合は除きます。また Debug モードは独自機能として編集はできませんが private フィールドも表示します。]
//footnote[2][プログラムのソースコードがそれを制作したプログラマ以外にとって解読困難である事を表す俗語]