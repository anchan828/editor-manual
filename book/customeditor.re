= CustomEditor

カスタムエディターは、インスペクターやシーンビューに表示されているGUIをカスタマイズするためのものです。本章ではカスタムエディターの基本的な使い方だけでなく、インスペクターの仕組みについても紹介していきます。

== インスペクターの Debug モード

例えば、Cube を作成し、インスペクターを見ると BoxCollider や MeshRenderer などのコンポーネントがアタッチされていることがわかります。

//image[ss07][Cubeを作成し、Cubeを選択している状態][scale=0.5]{

//}

その時に、インスペクターのタブ部分、または「 ≡ 」をクリックすると@<img>{ss04}のようにコンテキストメニューが表示され「Normal」と「Debug」項目があります。

//image[ss04][通常は Normal にチェックが付いている]{

//}

Debugを選択するとインスペクターに変化が現れます。@<img>{ss03}のように普段見ているインスペクターとは少し違う見た目になりました。

//image[ss03][普段見ない Instance ID や File ID のプロパティーが見える]{

//}

Debug モードでは、GUI 要素がカスタマイズされる前の@<b>{素}の状態に戻ります。@<fn>{1}このように Unity は、インスペクターに表示したい GUI 要素を取捨選択し、カスタマイズして表示します。

では、カスタマイズをするために「インスペクターに表示したい GUI 要素を取捨選択」する部分は一体どこなのかを知らなくてはいけません。

== オブジェクトと Editor クラス

@<href>{http://docs.unity3d.com/ScriptReference/Editor.html, Editor}クラスは、オブジェクトの情報をインスペクターやシーンビューに表示するための機能です。各オブジェクトに対応した Editor オブジェクトが生成され、Editor オブジェクトを介して必要な情報を GUI で表示できるようになります。


//image[ss06][ボックスコライダーを Editor オブジェクトを介して GUI を表示するイメージ]{

//}

その時に、全ての要素を見せるのは必要なかったり、追加の GUI 要素を表示したいということもあるので @<b>{CustomEditor（カスタムエディター）}の機能を使い Editor オブジェクトをカスタマイズすることができます。

=== 普段見ているインスペクターは既にカスタムエディターが使われている

普段インスペクターで触れているコンポーネントは、既にカスタムエディターによってカスタマイズされています。

//image[ss05][語尾がInspectorとEditorで表記にゆれがあるが違いはない]{

//}

これからカスタムエディターを作成するときには「何ができるか」の参考になります。


== カスタムエディターを使う

例えば、ゲーム中の実際の攻撃力は、キャラクターのちからや武器の強さで計算されて決まるとします。そのときにプログラム上で「攻撃力」というプロパティーを持ち、 getter で計算を行ってみましょう。プログラム上これでいいのですが、攻撃力の値を Unity エディターのインスペクターで確認したい場合は少し困ったことになります。Unityのインスペクターは、シリアライズ可能なフィールドを表示します。setter があってもシリアライズ対象ではないプロパティーは表示されません。

//image[ss01][下記コンポーネントをインスペクターで見た図]{

//}


//quote{
ソースコードは計算式がちょっとだけ分かりやすいように日本語変数にしてみました。
//}

//emlist{
using UnityEngine;

public class Character : MonoBehaviour
{
  [Range (0, 255)]
  public int 基本攻撃力;
  [Range (0, 99)]
  public int 剣の強さ;
  [Range (0, 99)]
  public int ちから;

  // プレイヤーの能力と、剣の強さから攻撃力を求めるプロパティー
  public int 攻撃力 {
      get {
        return 基本攻撃力 + 
               Mathf.FloorToInt (基本攻撃力 * (剣の強さ + ちから - 8) / 16);
      }
  }
}
//}




今回は、@<img>{ss02}のようにプロパティーである攻撃力をインスペクター上に表示して、確認しながらパラメーターを調整していきます。

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

インスペクターの GUI は OnInspectorGUI をオーバーライドすることでカスタマイズすることができます。

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

これで @<img>{ss02} と同じ表示を行うことができました。

=== シーンビューの GUI のカスタマイズ

シーンビューの GUI は OnSceneGUI を使うことでカスタマイズすることができます。これについては @<chapref>{handles} にて詳しく説明しているので、そちらをご覧ください。

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

Unity はデータの持ち方として SerializedObject で、すべてのデータを管理しています。SerializedObject 経由でデータにアクセスすることでデータの操作に柔軟に対応することができます。SerializedObject の詳しい説明は @<chapref>{serializedobject} をご覧ください。

Editor オブジェクトが生成されると同時に、コンポーネントがシリアライズされ、serializedObject 変数に格納されます。serializedObject 変数から各プロパティーにアクセスすることができます。

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
        //hpProperty.intValue = 
        //    EditorGUILayout.IntSlider ("Hp", hpProperty.intValue, 0, 100);

        serializedObject.ApplyModifiedProperties ();
    }
}
//}

=== コンポーネントに直接アクセスする方法

この方法は本章の前半で行っています。コンポーネントに直接アクセスすることで、GUI のカスタマイズを簡単に行うことが可能です。

対象のコンポーネントは Editor オブジェクトの target 変数でアクセスすることができます。 UnityEngine.Object 型なのでキャストする必要があります。

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

Unity ではゲームオブジェクトを複数選択し@<b>{同時に}同じプロパティーの値を編集することができます。ただし、同時編集ができるのは同時編集を許可されたコンポーネントのみです。

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

CanEditMultipleObjects 属性をつけることで同時編集が可能になる準備ができました。

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
同じ値でない場合は Unity は 「@<b>{-} 」を表示して、異なる値が代入されていると表現することができます。

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
        EditorGUI.showMixedValue = 
            characters.Select (x => x.hp).Distinct ().Count () > 1;

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

カスタムエディター内でも PropertyDrawer を使用することができます。使い方は @<code>{EditorGUILayout.PropertyField} に対象の SerializedProperty を渡すだけです。PropertyDrawer の詳しい説明は @<chapref>{property_drawer} をご覧ください。

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
    public override void OnGUI (Rect position, 
                           SerializedProperty property, GUIContent label)
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

            EditorGUI.MinMaxSlider (label, 
                        minMaxSliderRect, ref minHp, ref maxHp, 0, 100);

            EditorGUI.LabelField (labelRect, minHp.ToString (), maxHp.ToString ());

            if (EditorGUI.EndChangeCheck ()) {
                minHpProperty.intValue = Mathf.FloorToInt (minHp);
                maxHpProperty.intValue = Mathf.FloorToInt (maxHp);
            }
        }
    }

    public override float GetPropertyHeight (SerializedProperty property, 
                                                                GUIContent label)
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

このように、細かく部品にできるものは PropertyDrawer として実装すると、複雑なスパゲティーコード@<fn>{2}にならずに済むかもしれません。これは冗談ではなく、GUI の描画のためのコードは冗長になりがちなのでオススメです。


== プレビュー

インスペクターではメッシュやテクスチャ、スプライトなどプレビュー可能なアセットがある場合にプレビュー画面で確認することができます。

//image[ss12][Cubeのプレハブを選択した状態のインスペクターにあるプレビューウィンドウ]{

//}

=== プレビュー画面の表示

プレビューの表示はデフォルトでは無効となっており、「プレビューできる状態である」とインスペクターに判断させるには@<code>{HasPreviewGUI} メソッドをオーバーライドし、戻り値として true を返す必要があります。


//emlist{
public override bool HasPreviewGUI ()
{
    //プレビュー表示できるものがあれば true を返す
    return true;
}
//}

これにより@<img>{ss13}のように、普段は空のゲームオブジェクトではプレビューの表示はできませんが、できるようになりました。

//image[ss13][左が無効状態（false）、右が有効状態（true）]{

//}

=== プレビューの表示


==== 事前準備

事前に以下のスクリプトファイルを作成しておきます。

//emlist{
using UnityEngine;

public class PreviewExample : MonoBehaviour {

}
//}

//emlist{
using UnityEngine;
using UnityEditor;

[CustomEditor (typeof(PreviewExample))]
public class PreviewExampleInspector : Editor
{
    public override bool HasPreviewGUI ()
    {
        return true;
    }
}
//}

そして新規作成した Cube に PreviewExample をアタッチします。

//indepimage[ss14]


=== プレビューの最低限の実装

プレビューのウィンドウが表示する最低限の実装のために3つのメソッドを知っておかなくてはいけません。

: GetPreviewTitle
  プレビュー名です。同じゲームオブジェクトにアタッチされているコンポーネントが異なるプレビューを持っている場合の識別子にもなります。

//image[ss15][プレビューが複数ある場合はプレビュー名の部分がドロップダウンになる]{

//}

//emlist{
public override GUIContent GetPreviewTitle ()
{
    return new GUIContent ("プレビュー名");
}
//}

: OnPreviewSettings
  右上のヘッダーにGUIを追加するために使用します。プレビュー環境を変更したりするボタンや情報を記載します。ここに適した GUIStyle がドキュメント化されておらず見つけにくいですが「ラベルは@<code>{preLabel}」「ボタンは@<code>{preButton}」「ドロップダウンは@<code>{preDropDown}」「スライダーは@<code>{preSlider}」となります。
  また、ここでは (Editor)GUILayout を使うのを推奨しており、@<code>{EditorGUILayout.BeginHorizontal} によって水平に GUI が並べられるように設定されています。


//image[ss16][一番右端から並べられていく]{

//}

//emlist{
public override void OnPreviewSettings ()
{
    GUIStyle preLabel = new GUIStyle ("preLabel");
    GUIStyle preButton = new GUIStyle ("preButton");

    GUILayout.Label ("ラベル", preLabel);
    GUILayout.Button ("ボタン", preButton);
}
//}

: OnPreviewGUI

プレビューを表示（つまりテクスチャやレンダリング結果を表示するためのGUIを表示）する場所です。メソッドの引数として描画すべき領域の Rect を取得できるのでプレビューに合わせ Rect をカスタマイズすることができます。

//image[ss17][プレビュー領域全体に Box が描画されている]{

//}

//emlist{
public override void OnPreviewGUI (Rect r, GUIStyle background)
{
    GUI.Box (r, "Preview");
}
//}

== プレビューでカメラを使う

モデルデータやアニメーション編集時にプレビュー画面で、マウスドラッグによって対象オブジェクトを回転させ隅々まで見渡せる機能があります。

//image[ss18][AnimationClip のプレビュー。マウスでグリグリ動かすことができる]{

//}

これらの仕組みは特別なことをやっているわけではありません。@<img>{ss18}までリッチに、とはいきませんがそこまで作るための手順を紹介していきます。


=== PreviewRenderUtility

プレビューのユーティリティクラスとして @<code>{PreviewRenderUtility} があります。このクラスには@<b>{プレビュー専用のカメラ}が用意されており、簡単に@<code>{シーン内の景色を映し出すことができます。}


例として「対象のゲームオブジェクトをカメラで LookAt し続けるプレビュー」を作成してみましょう。

//image[ss19][完成図、プレビュー画面で特定の位置からゲームオブジェクトを見ることができる]{

//}

まずは OnEnable メソッドの中で PreviewRenderUtility のインスタンスを生成し、 LookAt する対象のゲームオブジェクトをコンポーネント経由で取得します。

//emlist{
using UnityEngine;
using UnityEditor;

[CustomEditor (typeof(PreviewExample))]
public class PreviewExampleInspector : Editor
{
    PreviewRenderUtility previewRenderUtility;
    GameObject previewObject;

    void OnEnable ()
    {
        // true にすることでシーン内のゲームオブジェクトを描画できるようになる
        previewRenderUtility = new PreviewRenderUtility (true);

        // FieldOfView を 30 にするとちょうどいい見た目になる
        previewRenderUtility.m_CameraFieldOfView = 30f;

        // 必要に応じて nearClipPlane と farClipPlane を設定
        previewRenderUtility.m_Camera.nearClipPlane = 0.3f;
        previewRenderUtility.m_Camera.farClipPlane = 1000;

        // コンポーネント経由でゲームオブジェクトを取得
        var component = (Component)target;
        previewObject = component.gameObject;
    }
}
//}

そして描画を行う部分です。@<code>{BeginPreview} と @<code>{EndAndDrawPreview} で囲み、その中で @<code>{Camera.Render} を呼び出します。そうすることでプレビュー画面に、 「PreviewRenderUtility が持つカメラからのレンダリング結果」が表示されるようになります。

//emlist{
public override void OnPreviewGUI (Rect r, GUIStyle background)
{
    previewRenderUtility.BeginPreview (r, background);
    
    var previewCamera = previewRenderUtility.m_Camera;

    previewCamera.transform.position = 
        previewObject.transform.position + new Vector3 (0, 2.5f, -5);
    
    previewCamera.transform.LookAt (previewObject.transform);
    
    previewCamera.Render ();

    previewRenderUtility.EndAndDrawPreview (r);

    
    // 描画タイミングが少ないことによって
    // カクつきがきになる時は Repaint を呼び出す（高負荷）
    // Repaint ();
}
//}

これで、@<img>{ss19}のようなプレビュー画面ができました。

//emlist{
using UnityEngine;
using UnityEditor;

[CustomEditor (typeof(PreviewExample))]
public class PreviewExampleInspector : Editor
{
    PreviewRenderUtility previewRenderUtility;
    GameObject previewObject;

    void OnEnable ()
    {
        previewRenderUtility = new PreviewRenderUtility (true);
        previewRenderUtility.m_CameraFieldOfView = 30f;

        previewRenderUtility.m_Camera.farClipPlane = 1000;
        previewRenderUtility.m_Camera.nearClipPlane = 0.3f; 

        var component = (Component)target;
        previewObject = component.gameObject;
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

    public override void OnPreviewGUI (Rect r, GUIStyle background)
    {
        previewRenderUtility.BeginPreview (r, background);

        var previewCamera = previewRenderUtility.m_Camera;

        previewCamera.transform.position =
            previewObject.transform.position + new Vector3 (0, 2.5f, -5);
        
        previewCamera.transform.LookAt (previewObject.transform);
        
        previewCamera.Render ();

        previewRenderUtility.EndAndDrawPreview (r);

    }
}
//}

=== プレビュー用オブジェクトを作成する


次は@<img>{ss18}のような、マウスでグリグリ動かすプレビューを作成します。


=== プレビューのゲームオブジェクトの生成場所


プレビューで使用されているゲームオブジェクトはシーンの中に生成されています。

下記の手順を行うことで、シーン内にあるプレビュー用ゲームオブジェクトを、プレビュー画面で表示すること可能です。 

 1. Object.Instantiate でプレビュー用ゲームオブジェクトを生成する
 2. プレビュー用ゲームオブジェクトに Preview 専用のレイヤー「PreviewCullingLayer」を設定
 3. Camera.Render の直前後にプレビュー用オブジェクトをアクティブ/非アクティブにする

順を追って説明していきます。

==== 1. Object.Instantiate でプレビュー用ゲームオブジェクトを生成する

コンポーネントからゲームオブジェクトを取得し、Instantiate で複製を行います。
この時、必ず @<code>{HideFlags.HideAndDontSave} を設定してヒエラルキーにゲームオブジェクトの表示を行わず、また、シーンに保存しないようにしてください。

最後に、ゲームオブジェクトを非アクティブにしてシーン内で見えないようにします。

//emlist{
GameObject previewObject;

void OnEnable ()
{
    var component = (Component)target;
    previewObject = Instantiate (component.gameObject);
    previewObject.hideFlags = HideFlags.HideAndDontSave;
    previewObject.SetActive (false);
}
//}

==== 2. プレビュー用ゲームオブジェクトに Preview 専用のレイヤー「PreviewCullingLayer」を設定

プレビュー専用の @<code>{Camera.PreviewCullingLayer} が用意されています。ですが、Reflection でアクセスする必要があります。

//emlist{
var flags = BindingFlags.Static | BindingFlags.NonPublic;
var propInfo = typeof(Camera).GetProperty ("PreviewCullingLayer", flags);
int previewLayer = (int)propInfo.GetValue (null, new object[0]);
//}

取得した @<code>{previewLayer} をプレビュー用のカメラとゲームオブジェクトに設定します。

//emlist{
previewRenderUtility = new PreviewRenderUtility (true);

// previewLayer のみ表示する
previewRenderUtility.m_Camera.cullingMask = 1 << previewLayer;
//}

階層下すべてに @<code>{previewLayer} を設定します。

//emlist{
previewObject.layer = previewLayer;
foreach (Transform transform in previewObject.transform) {
    transform.gameObject.layer = previewLayer;
}
//}


==== 3. Camera.Render の直前後にプレビュー用オブジェクトをアクティブ/非アクティブにする


@<b>{Camera.Render を実行する直前後にゲームオブジェクトを有効/無効にします。}こうすることでプレビューのゲームオブジェクトはプレビュー時のみ描画されるようになります。

もし、ゲーム再生中にプレビューを表示する場合、@<b>{ゲームに影響のあるコンポーネント}は無効にするか破棄するようにしてください。プレビューもシーンの中のゲームオブジェクトを表示しているだけなので影響が出てきてしまいます。

//emlist{
public override void OnInteractivePreviewGUI (Rect r, GUIStyle background)
{
    previewRenderUtility.BeginPreview (r, background);

    previewObject.SetActive (true);

    previewRenderUtility.m_Camera.Render ();

    previewObject.SetActive (false);

    previewRenderUtility.EndAndDrawPreview (r);

}
//}

=== グリグリ動かす

マウスで@<b>{ドラッグ}して、プレビューのゲームオブジェクトをグリグリ動かします。

マウスドラッグ時のマウス位置の差分は @<code>{Event.current.delta} で取得可能です。この差分で得たものを @<code>{transform.RotateAround} で設定しプレビュー用のゲームオブジェクトを回転させます。また、その時にゲームオブジェクトの中心位置を把握しておかなくてはいけません。

@<code>{transform.position} で取得できるものは必ずゲームオブジェクトの中心位置とは限りません。モデルデータであれば足元が原点である可能性もあります。中心位置を割り出すにはメッシュであれば Bounds を取得し、その中心位置を求めなければいけませんが、@<code>{PivotMode.Center} に設定されていれば @<code>{Tools.handlePosition} でゲームオブジェクトの中心位置を取得することが可能です。


//image[ss20][左が@<code>{PivotMode.Pivot}、右が @<code>{PivotMode.Center}]{

//}

ただ、この方法だとプレハブをプレビューした時に破綻してしまいます（プレハブはアセットでシーン上にはなく、Tools.handlePosition で位置を取得できないため）。どうしてもプレハブをプレビューしたい場合は、中心位置を独自で実装する必要があります。もしメッシュがあるなら Bounds から求めることが可能です。

//emlist{
Vector3 centerPosition;

void OnEnable ()
{
    centerPosition = GetCenterPosition ();
}

// ゲームオブジェクトの中心位置を取得
Vector3 GetCenterPosition ()
{
    // 設定中のPivotモードを保持
    var currentMode = Tools.pivotMode;

    // PivotMode.Center に変更
    Tools.pivotMode = PivotMode.Center;

    // ゲームオブジェクトの中心位置を取得
    var centerPosition = Tools.handlePosition;

    // pivotMode を元に戻す
    Tools.pivotMode = currentMode;

    return centerPosition;
}

public override void OnInteractivePreviewGUI (Rect r, GUIStyle background)
{
    var drag = Vector2.zero;

    // ドラッグ時のマウス位置の差分を取得
    if (Event.current.type == EventType.MouseDrag) {
        drag = Event.current.delta;
    }

    RotatePreviewObject (drag);
}

// X 軸と Y 軸それぞれ回転させる
private void RotatePreviewObject (Vector2 drag)
{
    previewObject.transform.RotateAround (centerPosition, Vector3.up, -drag.x);
    previewObject.transform.RotateAround (centerPosition, Vector3.right, -drag.y);
}
//}

最終的なコードが以下になります。

//emlist{
using UnityEngine;
using UnityEditor;
using System.Reflection;

[CustomEditor (typeof(PreviewExample))]
public class PreviewExampleInspector : Editor
{
    PreviewRenderUtility previewRenderUtility;
    GameObject previewObject;
    Vector3 centerPosition;

    void OnEnable ()
    {
        var flags = BindingFlags.Static | BindingFlags.NonPublic;
        var propInfo = typeof(Camera).GetProperty ("PreviewCullingLayer", flags);
        int previewLayer = (int)propInfo.GetValue (null, new object[0]);

        previewRenderUtility = new PreviewRenderUtility (true);
        previewRenderUtility.m_CameraFieldOfView = 30f;
        previewRenderUtility.m_Camera.cullingMask = 1 << previewLayer;

        var component = (Component)target;
        previewObject = Instantiate (component.gameObject);
        previewObject.hideFlags = HideFlags.HideAndDontSave;

        previewObject.layer = previewLayer;
        foreach (Transform transform in previewObject.transform) {
            transform.gameObject.layer = previewLayer;
        }


        var isPrefab = PrefabUtility.GetPrefabType (target) == PrefabType.Prefab;

        centerPosition = isPrefab ? Vector3.zero : GetCenterPosition ();

        if (isPrefab)
            previewObject.transform.position = Vector3.zero;

        previewObject.SetActive (false);

        RotatePreviewObject (new Vector2 (-120, 20));
    }

    Vector3 GetCenterPosition ()
    {
        var currentMode = Tools.pivotMode;
        Tools.pivotMode = PivotMode.Center;
        var centerPosition = Tools.handlePosition;
        Tools.pivotMode = currentMode;
        return centerPosition;
    }

    public override GUIContent GetPreviewTitle ()
    {
        return new GUIContent (target.name+ " Preview");
    }

    void OnDisable ()
    {
        DestroyImmediate (previewObject);
        previewRenderUtility.Cleanup ();
        previewRenderUtility = null;
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

        RotatePreviewObject (drag);

        previewObject.SetActive (true);
        previewRenderUtility.m_Camera.Render ();
        previewObject.SetActive (false);

        previewRenderUtility.EndAndDrawPreview (r);

        if (drag != Vector2.zero)
            Repaint ();
    }

    private void RotatePreviewObject (Vector2 drag)
    {
        previewObject.transform.RotateAround (centerPosition, Vector3.up, -drag.x);
        previewObject.transform.RotateAround (centerPosition, Vector3.right, -drag.y);
    }
}
//}

//footnote[1][Unity内部で非表示設定となっている場合は除きます。また Debug モードは独自機能として編集はできませんが private フィールドも表示します。]
//footnote[2][プログラムのソースコードがそれを制作したプログラマ以外にとって解読困難である事を表す俗語]
