= 標準で使えるエディター拡張機能


//lead{
Unityエディターは、既に内部でエディター拡張の機能を使い、さまざまな拡張を行っています。ユーザーは属性（Attribute）を付けるだけで、それらの拡張にアクセスすることができ、自由にカスタマイズを行えます。この章では既に用意されているものを使ってみて「エディター拡張ではこういうことができる」ということを知りましょう。
//}

== インスペクターの見た目を変える

=== Range

@<b>{int、float、long、double}など数値をスライダーで変更できるようにするための機能です。

開発時には「なんとなくな値」で「それとない動き」を作ることはよくあります。それを普段のインスペクター表示で行うには、不向きなところが出てきます。それを解決策の1つとして@<b>{スライダー}で操作できるようにして操作性を上げる方法があります。

//image[ss01][左がデフォルト、右がRangeで変更したもの]{

//}

//emlist{
using UnityEngine;
using System.Collections;

public class NewBehaviourScript : MonoBehaviour
{
    [Range(1, 10)]
    public int num1;

    [Range(1, 10)]
    public float num2;

    [Range(1, 10)]
    public long num3;

    [Range(1, 10)]
    public double num4;
}
//}


=== Multiline / TextArea

デフォルトでは一行のTextFieldですが、複数行のTextAreaに変更できます。MultilineとTextAreaは、ほぼ機能は同じですが、Multilineは「幅に合わせて自動改行されない」「スクロールバーが表示されない」といった制限があります。特に理由のない限りTextAreaを使うことをオススメします。

//image[ss02][上がMultiline。下がTextArea。TextAreaはスクロールバーが表示される]{

//}

//emlist{
using UnityEngine;
using System.Collections;

public class NewBehaviourScript : MonoBehaviour
{
    [Multiline(5)]
    public string multiline;

    [TextArea(3, 5)]
    public string textArea;
}

//}

== インスペクターで扱う機能を追加する

=== ContextMenuItem

インスペクターに表示されている変数にコンテキストメニューを追加します。少し手順が必要な要素を自動化したり、他の変数も一緒に値を変更しなければいけない時に使用してみるといいかもしれません。また、コンポーネント単位で値をリセットできる「Reset」機能はありますが、各変数に対してのリセット機能がないので ContextMenuItem で実装してみるのもいいかもしれません。

//image[ss15][プロパティー名を右クリックするとコンテキストメニューが表示できる]{

//}

//emlist{
using UnityEngine;

public class NewBehaviourScript : MonoBehaviour
{
    [ContextMenuItem ("Random", "RandomNumber")]
    [ContextMenuItem ("Reset", "ResetNumber")]
    public int number;

    void RandomNumber ()
    {
        number = Random.Range (0, 100);
    }

    void ResetNumber ()
    {
        number = 0;
    }
}
//}

=== ColorUsage

色の変更にはカラーピッカーを使用します。ColorUsageは、カラーピッカーでアルファの使用を有効/無効にできたり、HDR用のからピッカーに変更できます。

//image[ss14][左からデフォルト、アルファなし、HDRカラーピッカー]{

//}

//emlist{
using UnityEngine;

public class NewBehaviourScript : MonoBehaviour
{
    public Color color1;

    [ColorUsage (false)]
    public Color color2;

    [ColorUsage (true, true, 0, 8, 0.125f, 3)]
    public Color color3;
}
//}

== インスペクターの見た目を整える

プロパティーに直接作用しませんが、見た目を見やすくしたり装飾を行うことが可能です。

=== Header

プロパティーをある程度にまとめてヘッダーを付けることにより、わかりやすくすることが可能です。


//image[ss03][ヘッダーを付けることによって区切りがつき、見やすくなる]{

//}

//emlist{
using UnityEngine;
using System;

public class NewBehaviourScript : MonoBehaviour
{
    [Header("Player Settings")]
    public Player player;

    [Serializable]
    public class Player
    {
        public string name;

        [Range(1,100)]
        public int hp;
    }

    [Header("Game Settings")]
    public Color background;
}
//}

=== Space

縦に余白を設けることができます。プロパティーとの間に余白を設けることで、見やすくしたいときに便利です。

//image[ss04][]{

//}

//emlist{
using UnityEngine;

public class NewBehaviourScript : MonoBehaviour
{
    [Space(16)]
    public string str1;

    [Space(48)]
    public string str2;
}
//}

=== Tooltip

プロパティーに対する説明をインスペクター上で確認したいときに使用します。

//image[ss05][「Tooltip」にマウスカーソルを乗せるとツールチップが表示される]{

//}

//emlist{
using UnityEngine;

public class NewBehaviourScript : MonoBehaviour
{
    [Tooltip("これはツールチップです")]
    public long tooltip;
}

//}

=== HideInInspector


//image[ss06][本来public変数はインスペクターに表示されるがstr2が表示されていない]{

//}

//emlist{
using UnityEngine;

public class NewBehaviourScript : MonoBehaviour
{
    public string str1;

    [HideInInspector]
    public string str2;
}
//}

== インスペクターを少し便利にする

=== RequireComponent

「@<b>{最低1つは指定されたコンポーネントがアタッチされてなければいけない}」という制限を行う属性です。

他のコンポーネントが、必須となるスクリプトを作成することはよくあります。
例えば、Animatorに関するスクリプトを作成した時は、必ずAnimatorコンポーネントが必要となります。
必須なコンポーネントをアタッチするのを忘れてしまうと、当然ながらエラーが発生します。このエラーを事前に防ぐために@<b>{RequireComponent}を使用します。

RequireComponentが付いたスクリプトをアタッチすると、自動的に@<code>{RequireComponent}で指定しているコンポーネントもアタッチされます。既にアタッチされている場合は何も行いません。そして指定されたコンポーネントを削除しようとすると「削除できません」という内容のダイアログが表示されます。

//image[ss07][Animatorコンポーネントを削除しようとするとダイアログが表示される]{

//}

//emlist{
using UnityEngine;

[RequireComponent(typeof(Animator))]
public class NewBehaviourScript : MonoBehaviour
{
    Animator animator;

    void Awake ()
    {
        animator = GetComponent<Animator> ();
    }
}
//}

=== DisallowMultipleComponent

@<b>{1つのゲームオブジェクトに同じコンポーネントを複数アタッチすることを禁止}する属性です。

1つだけアタッチされてればいいのに気付かずに、同じコンポーネントを2つ以上アタッチしている場合があります。2つ以上アタッチされている状態でゲームを再生すると上手く動作せず「コードは正しいのに何故か動作しない」という原因不明の状態に陥りやすくなります。同じコンポーネントが2つアタッチされていた、というのは意外と気付きにくいものです。この問題を回避するために@<b>{DisallowMultipleComponent}を使用します。

//image[ss08][同じスクリプトをアタッチしようとするとダイアログが表示される]{

//}

継承クラスでもDisallowMultipleComponentは効果を発揮します。

//emlist{
using UnityEngine;

public class NewBehaviourScript : Base
{
}
//}

//emlist{
using UnityEngine;

[DisallowMultipleComponent]
public class Base : MonoBehaviour
{
}
//}

=== RenamedSerializedData

@<b>{変数名を変更した時に新しい変数名へデータの移行を行うための属性です。}

インスペクターに表示されるような、シリアライズされているデータは変数名をパスとして保存されています。ここで変数名の変更を行うとシリアライズされていたデータが引き継げずに初期化されてしまいます。これはパスから値を参照できなくなったためです。変数名が変更されたという検知をするのは不可能でこれはどうしようもない問題です。この問題を回避するためにRenamedSerializedDataを使用します。


//emlist{
using UnityEngine;

public class NewBehaviourScript : MonoBehaviour
{
    [SerializeField]
    string hoge;
}
//}

インスペクター上で値を入力します。

//indepimage[ss09]

hogeからfugaへと変数名を変更します。

//emlist{
sing UnityEngine;

public class NewBehaviourScript : MonoBehaviour
{
    [SerializeField]
    string fuga;
}
//}

するとインスペクターで入力した文字列は引き継がれずに何も表示されません。

//indepimage[ss10]

そこでRenamedSerializedDataを使用することで引き継がれるようになります。

//emlist{
using UnityEngine;

public class NewBehaviourScript : MonoBehaviour
{
    [SerializeField]
    [RenamedSerializedData("hoge")]
    string fuga;
}
//}

//indepimage[ss11]

ここで気を付けて欲しいのが、@<b>{変数名の変更とRenamedSerializedData属性の追加、oldNameの指定は同時に行わなければいけない点}です。スクリプトのコンパイルによって必要ないデータは破棄されてしまい、変数名を変更（コンパイル1回目）したにもかかわらず、RenamedSerializedDataの指定を忘れていて、慌てて追加（コンパイル2回目）しても既にデータは破棄されています。2回目のコンパイルで、データが破棄されるためです。

=== AddComponentMenu

@<i>{Component}メニューにメニュー項目を追加します。


スクリプトは全て@<b>{Component/Scripts}メニューの中にまとめられます。
汎用的なスクリプトコンポーネント郡を作成した時、1つのカテゴリにまとめたい時があります。
例えばUIに関するスクリプトを作成した時などです。

//image[ss12][TweenColor/Position/Rotation/Scale.csを作成した状態]{

//}

この時に@<b>{AddComponentMenu}を使用すると別メニューとして作成できます。AddComponentMenuを使用したスクリプトは他の場所にメニューが作成されるので@<b>{Component/Scripts}メニューの中から削除されます。

//emlist{
using UnityEngine;

[AddComponentMenu("MyUI/Tween Color")]
public class TweenColor : MonoBehaviour
{
}
//}

//image[ss13][My UI下に移動し、カテゴライズできた]{

//}

== ゲームの開発を楽にする

=== ExecuteInEditMode

ゲーム再生中でなくても MonoBehaviour を継承したコンポーネントの 主要な関数が呼び出されるようになります。呼び出されるタイミングはゲームオブジェクトが更新された時です。シーアセットをダブルクリックして、シーンをロードした時には Awake と Start 関数が、インスペクターなどでコンポーネントの変数などを変更したら Update 関数が呼び出されます。また、OnGUI で実装した GUI がエディターのGUI描画サイクルに則って常に表示されるようになります。


//emlist{
using UnityEngine;

[ExecuteInEditMode]
public class NewBehaviourScript : MonoBehaviour
{
    [Range(0,10)]
    public int number;

    void Awake ()
    {
        Debug.Log ("Awake");
    }

    void Start ()
    {
        Debug.Log ("Start");
    }

    void Update ()
    {
        Debug.Log ("Update");
    }
}
//}


=== ContextMenu

コンポーネントのコンテキストメニューからメソッドを実行します。@<code>{ContextMenuItem}と名前と機能が似ていますが、追加するコンテキストメニューの場所が違います。

//image[ss16][コンポーネントの歯車をクリックまたはコンポーネントのタイトルを右クリック]{

//}

//emlist{
using UnityEngine;

public class NewBehaviourScript : MonoBehaviour
{
    [Range (0, 10)]
    public int number;

    [ContextMenu ("RandomNumber")]
    void RandomNumber ()
    {
        number = Random.Range (0, 100);
    }

    [ContextMenu ("ResetNumber")]
    void ResetNumber ()
    {
        number = 0;
    }
}
//}

=== SelectionBase

シーンビューでオブジェクトを選択した時に、「選択されるゲームオブジェクトを指定する」「ゲームオブジェクトの@<b>{選択順}を決める」時に使用します。

普段、シーンビューでオブジェクトを選択した時はルートのゲームオブジェクトが選択されます。

//image[ss17][Cubeをマウスでクリックした直後の状態]{

//}

以下のスクリプトを記述し、子要素のゲームオブジェクトにアタッチします。

//emlist{
using UnityEngine;

[SelectionBase]
public class NewBehaviourScript : MonoBehaviour
{
}
//}

そしてルートのゲームオブジェクトをクリックすると、子要素のゲームオブジェクトが選択されるようになります。


//image[ss18][Cubeをマウスでクリックした直後の状態]{

//}

そして@<b>{もう一度}ゲームオブジェクトをクリックするとルートのゲームオブジェクトが選択されます。
このように、SelectionBase 属性の付いたゲームオブジェクトで@<b>{最下層の子要素から順に}選択できます。順に選択していき、SelectionBase 属性の付いたゲームオブジェクトが存在しない場合はルートを選択するという仕様です。

//image[ss19][クリックするごとに下から順に選択されていく]{

//}
