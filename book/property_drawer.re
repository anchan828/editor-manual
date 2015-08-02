= PropertyDrawer

例えば、キャラクターのためのスクリプトがあり、ヒットポイントの hp 変数があるとします。

//emlist{
using UnityEngine;

public class Example : MonoBehaviour
{
    [SerializeField]
    int hp;
}
//}

上記のコードは @<code>{SerializeField} 属性がついており、インスペクターに表示される実装となっています。

//image[ss01][いつもよく見るインスペクター]{

//}

この hp 変数に以下の制限が付いている場合、どうすればいいでしょうか。

 * 自キャラのHPがマイナスになってはいけない@<fn>{1}
 * HPの上限は決まっている
 * この値は未調整で、値を変更しながらちょうどいい値を見つけなければいけない

これらの制限（仕様）は開発時の時点で考慮しなければいけなく、特にインスペクターで値を編集するときは、これらの制限を付けるのは@<b>{標準機能であれば}難しいです。


== PropertyDrawer とは

標準機能で難しい機能・操作を実現させるのがエディター拡張です。今回のようにインスペクターに表示されるコンポーネントの GUI を変更するには @<b>{CustomEditor} が適しています。ですが、これはコンポーネント全体のカスタマイズになります。今回はコンポーネントの一部である hp 変数（プロパティー）のみをカスタマイズしたいので @<b>{CustomEditor} ではなく @<b>{PropertyDrawer} を使用します。

まず Unity は、シリアライズされたデータを Unity が自動判断して適切な GUI を使ってインスペクター@<fn>{2}に表示します。

@<b>{PropertyDrawer} はその自動判断して適切な GUI を使う判断処理をフックして、自前の GUI を使用する技術です。

//indepimage[ss02]


例えば以下の様なシリアライズ可能なクラスがあるとします。

//emlist{
[Serializable]
public class Character
{
	[SerializeField]
	string name;

	[SerializeField]
	int hp;
}
//}

これをインスペクターに表示しようとすると コンパクトではありますがとても見難い表示となります。

//image[ss13][いつもの見た目]{

//}

これを PropertyDrawer によって GUI の描画をカスタマイズし、1行で表示することが出来ます。

//image[ss14][1行に name と hp のプロパティが表示されている]{

//}

このようにインスペクターの操作で不便だと思った部分をカスタマイズしていくことが可能です。

== PropertyAttribute

PropertyAttribute は単なる Attribute を継承したクラスです。CustomEditor が コンポーネントの Editor オブジェクトを拡張するように、PropertyDrawer は、PropertyAttribute を拡張します（正確には PropertyAttribute のついたシリアライズ可能なフィールドです）。


//emlist{
using UnityEngine;

public class ExampleAttribute : PropertyAttribute
{
}
//}

== CustomPropertyDrawer と PropertyDrawer


CustomEditor と同じような実装方法で PropertyDrawer も拡張を行います。


PropertyDrawer を継承した派生クラスを作成し、拡張したいクラスを @<code>{CustomPropertyDrawer} の引数に与えます。

シリアライズ可能なクラスであれば以下のクラスを作成します。

//emlist{
[CustomPropertyDrawer (typeof(Character))]
public class CharacterDrawer : PropertyDrawer
{
}
//}

PropertyAttribute の派生クラスの場合も同様です。

//emlist{
using UnityEngine;
using UnityEditor;

[CustomPropertyDrawer (typeof(ExampleAttribute))]
public class ExampleDrawer : PropertyDrawer
{
}
//}

あとは作成した PropertyAttribute をフィールドに追加するだけです。

//emlist{
using UnityEngine;

public class Hoge : MonoBehaviour
{
    [SerializeField, Example]
    int hp;
}
//}

== RangeAttribute を試す

すでに標準でいくつかの PropertyDrawer が実装されています。

その中の１つである RangeAttribute を使ってみましょう。


//emlist{
using UnityEngine;

public class Example : MonoBehaviour
{
    [SerializeField, Range (0, 10)]
    int hp;
}
//}

属性として @<code>{Range (0, 10)} を追加するだけで 0 から 10 までスライドできるSliderを作成することが出来ました。

//image[ss03][]{

//}


== RangeAttribute を自作する

試しに、標準実装されている RangeAttribute と同じものを作成してみます。

=== Range2Attribute の作成

最小値(min)と最大値(max)を保持するようにして、Attribute の使用制限を設定します。AttributeUsage についてはこちらのリンクを参照してください。

@<href>{https://msdn.microsoft.com/ja-jp/library/system.attributeusageattribute(v=vs.110).aspx,MSDN - AttributeUsageAttribute クラス}

//emlist{
using UnityEngine;

[System.AttributeUsage (System.AttributeTargets.Field, Inherited = true, AllowMultiple = false)]
public class Range2Attribute : PropertyAttribute
{
    public readonly int min;
    public readonly int max;

    public Range2Attribute (int min, int max)
    {
        this.min = min;
        this.max = max;
    }
}
//}

=== Range2Drawer の作成

属性のついたフィールドは SerializedProperty 経由で扱います。 propertyType が int であれば IntSlider を使用し、int 以外であれば標準の GUI を使用します。

//emlist{
using UnityEditor;
using UnityEngine;

[CustomPropertyDrawer (typeof(Range2Attribute))]
internal sealed class RangeDrawer : PropertyDrawer
{
    public override void OnGUI (Rect position, SerializedProperty property, GUIContent label)
    {
        Range2Attribute range2 = (Range2Attribute)attribute;

        if (property.propertyType == SerializedPropertyType.Integer)
            EditorGUI.IntSlider (position, property, range2.min, range2.max, label);
        else
            EditorGUI.PropertyField (position, property, label);
    }
}
//}

== Range2Attribute を使用する

以上で実装が終わったので Range2Attribute を使用してみます。 int 以外では扱えないことを確認するために string に対しても属性を付加しています。

//indepimage[ss04]

//emlist{
using UnityEngine;

public class Example : MonoBehaviour
{
    [SerializeField, Range2 (0, 10)]
    int hp;

    [SerializeField, Range2 (0, 10)]
    string str;
}
//}

== 様々な PropertyDrawer

Unity に標準実装されているものは@<chapref>{part2-standardextension}で紹介しています。ここからは私が今までに作成した PropertyDrawer を紹介していきます。

=== Angle

//image[ss06][真ん中の数字は InputField なので値を入力して変更できる]{

//}


API として @<img>{ss06} のようにノブ（取って）を表示する @<code>{EditorGUILayout.Knob} があります。ですが、 PropertyDrawer では EditorGUILayout の使用は禁止されているので使うことが出来ません。
内部的には EditorGUI.Knob が実装されており、リフレクションを使って呼び出すことによって使用が可能になります。

//emlist{
private readonly MethodInfo knobMethodInfo = typeof(EditorGUI).GetMethod("Knob",
       BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Static);

private float Knob(Rect position, Vector2 knobSize, float currentValue, float start, float end, string unit, Color backgroundColor, Color activeColor, bool showValue)
{
    var invoke = knobMethodInfo.Invoke(null, new object[] { position, knobSize, currentValue, start, end, unit, backgroundColor, activeColor, showValue, GUIUtility.GetControlID("Knob".GetHashCode(), FocusType.Native, position) });
    return (float)(invoke ?? 0);
}
//}

==== GetPropertyHeight

デフォルトの GUI の高さ (@<code>{EditorGUIUtility.singleLineHeight}) から変更したいときは GetPropertyHeight をオーバーライドします。

//emlist{
[CustomPropertyDrawer(typeof(AngleAttribute))]
public class AngleDrawer : PropertyDrawer
{
    private AngleAttribute angleAttribute { get { return (AngleAttribute)attribute; } }

    public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
    {
        // 略
    }

    // 戻り値として返した値が GUI の高さとして使用されるようになる
    public override float GetPropertyHeight(SerializedProperty property, GUIContent label)
    {
        var height = base.GetPropertyHeight(property, label);
        return property.propertyType != SerializedPropertyType.Float ? height : angleAttribute.knobSize + 4;
    }
}
//}


=== AnimatorParameter

Animatorウインドウにあるパラメータ名をタイプセーフにフィールドにアタッチすることができます。
取得できるパラメーターは同じゲームオブジェクトにアタッチされている Animator Controller のパラメーターになります。

//indepimage[ss07]

//emlist{
using UnityEngine;

[RequireComponent(typeof(Animator))]
public class AnimatorParameterExample : MonoBehaviour
{
    // すべてのタイプのパラメーターを取得
    [AnimatorParameter]
    public string param;
    
    // Float のみ
    [AnimatorParameter(AnimatorParameterAttribute.ParameterType.Float)]
    public string floatParam;
    
    // Int のみ
    [AnimatorParameter(AnimatorParameterAttribute.ParameterType.Int)]
    public string intParam;
    
    // Bool のみ
    [AnimatorParameter(AnimatorParameterAttribute.ParameterType.Bool)]
    public string boolParam;
    
    // Trigger のみ
    [AnimatorParameter(AnimatorParameterAttribute.ParameterType.Trigger)]
    public string triggerParam;
}
//}

==== 同オブジェクトのコンポーネントを取得

今回は同じゲームオブジェクトにアタッチされている Animator コンポーネントから AnimatorController を取得しています。

コンポーネントは SerializedProperty -> SerializedObject -> Component の順で取得することが可能です。

//emlist{
AnimatorController GetAnimatorController(SerializedProperty property)
{
    var component = property.serializedObject.targetObject as Component;

    if (component == null)
    {
        Debug.LogException(new InvalidCastException("Couldn't cast targetObject"));
    }

    var anim = component.GetComponent<Animator>();

    if (anim == null)
    {
        Debug.LogException(new MissingComponentException("Missing Aniamtor Component"));
        return null;
    }

    return anim.runtimeAnimatorController as AnimatorController;
}
//}

=== DisableAttribute

プロパティーをインスペクター上で編集不可にします。インスペクターに表示したいが、値を変更させたくない時に使います。

//indepimage[ss05]

//emlist{
using UnityEngine;

public class DisableExample : MonoBehaviour
{
    [Disable]
    public string hoge = "hoge";

    [Disable]
    public int fuga = 1;

    [Disable]
    public AudioType audioType = AudioType.ACC;
}
//}

実装の方法は簡単です。

BeginDisabledGroup と EndDisabledGroup、または DisabledGroupScope を使って PropertyField を囲むだけで実装できます。

インスペクターで編集できなくなったといっても、インスペクターを Debug モードにすると編集することが出来ますし、スクリプトから値の編集ができるので注意してください。

//emlist{
public override void OnGUI(Rect position, SerializedProperty property, GUIContent label)
{
    EditorGUI.BeginDisabledGroup(true);
    EditorGUI.PropertyField(position, property, label);
    EditorGUI.EndDisabledGroup();
}
//}

=== EnumLabel

Enumの表示名を変更します。

//indepimage[ss08]

//emlist{
using UnityEngine;

public class EnumLabelExample : MonoBehaviour
{
    public enum Example
    {
        [EnumLabel("テスト")]
        HIGH,
        [EnumLabel("その２")]
        LOW
    }

    [EnumLabel("例")]
    public Example test = Example.HIGH;
}
//}

GUI を表示する時に、 EnumLabel に渡した文字列を使って Popup を表示しています。上記の例のように test 変数にも属性を付けないと適用されません。これは PropertyAttribute がフィールドについていないとイベントが発火しないためです。


=== Popup

Attributeに渡したパラメーターを使って Popup を表示します。値を Popup で選択できるようになります。

//indepimage[ss09]

//emlist{
using UnityEngine;
using System.Collections;

public class PopupExample : MonoBehaviour
{
    [Popup("Hoge","Fuga","Foo","Bar")]
    public string popup;

    [Popup(1,2,3,4,5,6)]
    public int popup2;

    [Popup(1.5f,2.3f,3.4f,4.5f,5.6f,6.7f)]
    public float popup3;
}
//}

値の持ち方を object にしているので複数のタイプをサポートしています。

//emlist{
public class PopupAttribute : PropertyAttribute
{
    public object[] list;
    
    // 引数は object 配列
    public PopupAttribute (params object[] list)
    {
        this.list = list;
    }
}
//}

ですがこうすると Popup から選択した値を代入する時に少し苦労します。

SerializedProperty に値を代入するには @<code>{property.stringValue}、@<code>{property.intValue}	、@<code>{property.floatValue} というように各々の変数へと代入しなければいけません。



=== PreviewTexture

テクスチャのプレビューを表示します。

//indepimage[ss10]    

//emlist{
using UnityEngine;

public class PreviewTextureAttributeExample : MonoBehaviour
{
    //60秒キャッシュする
    [PreviewTexture(60)]
    public string textureURL = "https://www.hogehoge.com/image.png";

    [PreviewTexture]
    public Texture hoge;
}
//}

==== テクスチャの描画には GUIStyle を使う

テクスチャの描画には @<code>{EditorGUI.DrawPreviewTexture} を使用しますが、PropertyDrawer では @<b>{描画タイミングの関係でテクスチャが表示/非表示を繰り返してチカチカしてしまう}問題が発生します。
この問題があるため代案として GUIStyle を使ってスタイルの背景としてテクスチャを描画します。

//emlist{
void DrawTexture(Rect position, Texture2D texture)
{
    float width = Mathf.Clamp(texture.width, position.width * 0.7f, position.width * 0.7f);
    var rect = new Rect(position.width * 0.15f, position.y + 16, width, texture.height * (width / texture.width));

    if (style == null)
    {
        style = new GUIStyle();
        style.imagePosition = ImagePosition.ImageOnly;
    }

    style.normal.background = texture;
    GUI.Label(rect, "", style);
}
//}

=== SceneName

有効なシーン名をPopupで選択することができます。Popupで表示されるものは Build Settings の Scene In Build に含まれているシーン名です。

//indepimage[ss11]

//emlist{
using UnityEngine;

public class SceneNameExample : MonoBehaviour
{
    [SceneName]
    public string sceneName;

    // 無効状態のシーンも表示する
    [SceneName(false)]
    public string sceneName2;
}
//}

==== EditorBuildSettings.scenes でシーンの一覧を取得

シーンは @<code>{EditorBuildSettings.scenes} 変数で管理されています。ただし Build Settings の Scene In Build にシーンを登録してないと一覧に含まれないので注意してください。

//footnote[1][ushort や uint 使えばいいじゃんというツッコミはなしで...!!]
//footnote[2][実際は OnGUI を使う場所だとどこでも Unity が自動判断して適切な GUI を表示します。]