= 標準で使えるエディタ拡張機能

//lead{
Unityが既にエディタ拡張の機能を使い、ユーザーの手で自由に拡張できるものをいくつか用意しています。この章では用意されているものを使ってみて「エディタ拡張ではこういうことが出来る」ということを知りましょう。
//}

== インスペクターの見た目を変える

=== Range Attribute

@<b>{int、float、long、double}など数値をスライダーで変更できるようにするための機能です。

開発時には「なんとなくな値」で「それとない動き」を作ることはよくあります。それを普段のインスペクター表示では不向きなところが出てきます。それを解決策の1つとして@<b>{スライダー}で操作できるようにして操作性を上げる方法があります。

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


=== Multiline / TextArea Attribute

デフォルトでは一行のTextFieldですが、複数行のTextAreaに変更することが出来ます。MultilineとTextAreaはほぼ機能は同じですが、Multilineは「幅に合わせて自動改行されない」「スクロールバーが表示されない」といった制限があります。特に理由のない限りTextAreaを使うことをオススメします。

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

== インスペクターの見た目を整える

プロパティに直接作用しませんが見た目を見やすくしたり装飾を行うことが可能です。

=== Header Attribute

プロパティをある程度にまとめてヘッダーを付けることにより、わかりやすくすることが可能です。


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

=== Space Attribute

縦に余白を設けることが出来ます。プロパティとの間に余白を設けることで見やすくしたいときに便利です。

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

=== Tooltip Attribute

プロパティに対する説明をインスペクター上で確認したいときに使用します。

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

=== HideInInspector Attribute


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

@<b>{最低1つは指定されたコンポーネントがアタッチされてなければいけない}という制限を行う属性です。

他のコンポーネントが必須となるスクリプトを作成することはよくあります。
例えばAnimatorに関するスクリプトを作成した時は必ずAnimatorコンポーネントが必要となります。
必須なコンポーネントをアタッチするのを忘れてしまうと、当然ながらエラーが発生します。このエラーを事前に防ぐのに@<b>{RequireComponent}を使用します。



RequireComponentが付いたスクリプトをアタッチすると自動的にRequireComponentで指定しているコンポーネントもアタッチされます。既にアタッチされている場合は何も行いません。そして指定されたコンポーネントを削除しようとすると「削除できません」という内容のダイアログが表示されます。

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

1つだけアタッチされてればいいのに気付かずに同じコンポーネントを2つ以上アタッチされている場合があります。2つ以上アタッチされている状態でゲームを再生すると上手く動作せず「コードは正しいのに何故か動作しない」という原因不明の状態に陥りやすくなります。同じコンポーネントが2つアタッチされていたというのは意外と気付きにくいものです。この問題を回避するために@<b>{DisallowMultipleComponent}を使用します。

//image[ss08][同じスクリプトをアタッチしようとするとダイアログが表示される]{

//}

継承クラスでもDisallowMultipleComponentは効果を発揮します。

//emlist{
using UnityEngine;
using System.Collections;

public class NewBehaviourScript : Base
{
}


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

ここで気を付けて欲しいのが、@<b>{変数名の変更とRenamedSerializedData属性の追加、oldNameの指定は同時に行わなければいけない点}です。スクリプトのコンパイルによって必要ないデータは破棄されてしまい、変数名を変更した（コンパイルされる）にもかかわらずRenamedSerializedDataの指定を忘れていて慌てて追加（2回目のコンパイル。ここでデータは破棄される。）しても既にデータは破棄されています。

=== AddComponentMenu

@<i>{Component}メニューにメニュー項目を追加します。


スクリプトは全て@<b>{Component/Scripts}メニューの中にまとめられます。
汎用的なスクリプトコンポーネント郡を作成した時、1つのカテゴリにまとめたい時があります。
例えばUIに関するスクリプトを作成した時などです。

//image[ss12][TweenColor/Position/Rotation/Scale.csを作成した状態]{

//}

この時に@<b>{AddComponentMenu}を使用すると別メニューとして作成することが出来ます。AddComponentMenuを使用したスクリプトは他の場所にメニューが作成されるので@<b>{Component/Scripts}メニューの中から削除されます。

//emlist{
using UnityEngine;

[AddComponentMenu("MyUI/Tween Color")]
public class TweenColor : MonoBehaviour
{
}
//}

//image[ss13][My UI下に移動し、カテゴライズ出来た]{

//}