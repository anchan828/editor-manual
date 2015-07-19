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

標準機能で難しい機能・操作を実現させるのがエディタ拡張です。今回のようにインスペクターに表示されるコンポーネントの GUI を変更するには @<b>{CustomEditor} が適しています。ですが、これはコンポーネント全体のカスタマイズになります。今回はコンポーネントの一部である hp 変数（プロパティー）のみをカスタマイズしたいので @<b>{CustomEditor} ではなく @<b>{PropertyDrawer} を使用します。

まず Unity は、シリアライズされたデータを Unity が自動判断して適切な GUI を使ってインスペクター@<fn>{2}に表示します。

@<b>{PropertyDrawer} はその自動判断して適切な GUI を使う判断処理をフックして、自前の GUI を使用する技術です。

//indepimage[ss02]




== PropertyAttribute

PropertyAttribute は単なる Attribute を継承したクラスです。CustomEditor が コンポーネントの Editor オブジェクトを拡張するように、PropertyDrawer は を説明する前に、PropertyAttribute を拡張します（正確には PropertyAttribute のついたシリアライズ可能なフィールドです）。


//emlist{
using UnityEngine;

public class ExampleAttribute : PropertyAttribute
{
}
//}

== CustomPropertyDrawer と PropertyDrawer


CustomEditor と同じような実装方法で PropertyDrawer も拡張を行います。


PropertyDrawer を継承した派生クラスを作成し、拡張したいクラスを @<code>{CustomPropertyDrawer} の引数に与えます。

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


//footnote[1][ushort や uint 使えばいいじゃんというツッコミはなしで...!!]
//footnote[2][実際は OnGUI を使う場所だとどこでも Unity が自動判断して適切な GUI を表示します。]