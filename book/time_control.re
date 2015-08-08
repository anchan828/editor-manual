= 時間を制御する TimeControl

エディター拡張をしていると、Animation ウインドウや AnimationClip のプレビューで使われているような、タイムコントロールを扱いたい時が出てきます。

//image[ss01][再生ボタンを押すと時間が流れる比較的単純なもの][scale=0.6]{

//}


//image[ss02][複雑な要素が絡んだ本格的なもの]{

//}

本章では、これらの時間を管理するための基板となる TimeControl を解説します。


== 時間の素 EditorApplication.timeSinceStartup

経過時間を把握するために @<b>{EditorApplication.timeSinceStartup} を使用します。これは Unity エディターが起動されてからの経過時間を取得することが可能です。@<fn>{1}

以下のコードが、時間を計測し続ける仕組みになります。経過時間の更新は エディターの更新時に呼び出される @<b>{EditorApplication.update} 経由で行います。

//emlist{
public class TimeControl
{
    private double m_lastFrameEditorTime = 0;
    public float currentTime;
    
    public TimeControl ()
    {
        EditorApplication.update += TimeUpdate;
    }
    
    public void TimeUpdate ()
    {
        var timeSinceStartup = EditorApplication.timeSinceStartup;
        var deltaTime = timeSinceStartup - m_lastFrameEditorTime;
        m_lastFrameEditorTime = timeSinceStartup;
        currentTime += (float)deltaTime;
    }
}
//}

これだけでは、時間が流れていくだけなので「再生」や「一時停止」「停止」を実装していきます。

== 再生

//emlist{
public bool isPlaying { get; private set; }

public void TimeUpdate ()
{
    if (isPlaying) {
        // 略。currentTime 更新
    }
}

public void Play ()
{
    isPlaying = true;
    m_lastFrameEditorTime = EditorApplication.timeSinceStartup;
}
//}

再生は簡単です。トリガーとなる Play メソッドと再生中かを判断する isPLaying プロパティを作成するだけで好きな時に再生を行うことが出来ます。

1つだけ注意する点として、@<b>{m_lastFrameEditorTime} の更新も同時に行わなければいけません。これを行わなければ、@<b>{再生するまでの経過時間}も更新時に含まれてしまい、時間が飛んだような挙動になってしまいます。

== 一時停止

//emlist{
public void Pause ()
{
    isPlaying = false;
}
//}

isPlaying を false にするだけです。


== 停止

//emlist{
public void Stop ()
{
    isPlaying = false;
    currentTime = 0;
}
//}

isPlaying を false にして、currentTime を 0 にリセットします。

== 時間の下限/上限

Play メソッドを実行し、ずっと時間を計測するのもいいですが「@<b>{ある一定の経過時間でループする}」処理を加えると AnimationClip で扱うような挙動が出来るようになります。

//emlist{
public float minTime { get; private set; }

public float maxTime { get; private set; }

public float currentTime {
    get { 
        m_currentTime = Mathf.Repeat (m_currentTime, maxTime);
        m_currentTime = Mathf.Clamp (m_currentTime, minTime, maxTime);
        return m_currentTime;
    }
    set { 
        m_currentTime = value;
    }
}

public void SetMinMaxTime (float minTime, float maxTime)
{
    this.minTime = minTime;
    this.maxTime = maxTime;
}
//}


まず、@<code>{minTime} と @<code>{maxTime} プロパティーを作成します。
次に、@<code>{currentTime} を取得する時に @<code>{Repeat} と @<code>{Clamp} で時間を加工します。Repeat は時間ループのため@<fn>{2}、Clamp は @<code>{minTime} による時間の開始位置を指定するために使用します。

== 再生スピード

//emlist{
public float speed { get; set; }

public void TimeUpdate ()
{
    if (isPlaying) {
        var timeSinceStartup = EditorApplication.timeSinceStartup;
        var deltaTime = timeSinceStartup - m_lastFrameEditorTime;
        m_lastFrameEditorTime = timeSinceStartup;
        
        currentTime += (float)deltaTime * speed;
    }
}
//}

最後に再生スピードを変更できるようにします。@<code>{speed} プロパティを作成して時間更新時に乗算します。

== 動くか確認

簡単なコードを書いて動作を確認してみましょう。サイドバーのつまみを移動させても、その位置から再生されることを確認してください。

//emlist{
using UnityEngine;
using UnityEditor;

public class Example : EditorWindow
{
    TimeControl timeControl;

    [MenuItem ("Window/Example")]
    static void Open ()
    {
        GetWindow<Example> ();
    }

    void OnEnable ()
    {
        timeControl = new TimeControl ();
        timeControl.SetMinMaxTime (0, 10);
    }

    void OnGUI ()
    {
        var buttonText = timeControl.isPlaying ? "Pause" : "Play";
    
        if (GUILayout.Button (buttonText)) {
    
            if (timeControl.isPlaying)
                timeControl.Pause ();
            else
                timeControl.Play ();
        }
    
        timeControl.currentTime = EditorGUILayout
            .Slider (timeControl.currentTime, 0, 10);
    
        // GUI 更新
        if (timeControl.isPlaying)
            Repaint ();
    }
}
//}


本章で作成した TimeControl は、 @<chapref>{shuriken_controller} や @<chapref>{spriteanimationpreview2} で使用されています。使用例として参考にしてください。

//footnote[1][DateTime.Now.Ticks を使っても問題ありません。紹介のために timeSinceStartup を扱ってみました。]
//footnote[2][機能的にループ強制としました。ループをオプションとしたいときには loop 変数などを作成して制御するようにしてください。]