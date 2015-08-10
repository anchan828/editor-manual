= パーティクルを制御する

== パーティクルシステムの制御

普段、パーティクルシステムを扱うときは GameObject を作成し、@<b>{選択しながら}パーティクルの再生やパラメーターの調整を行います。

//image[ss01][シーン、ヒエラルキー、インスペクターを使ってパーティクルを作成]{

//}

ですが、@<b>{複数同時編集に対応していない}ため面倒なことが多々あります。

//image[ss02][インスペクターに「Multi-object editing not supported」と出ており複数同時編集ができない]{

//}

エフェクトはさまざまですが、@<b>{Duration（継続時間）} や @<b>{Start Lifetime（存続時間）} は統一したり、複数のエフェクトに対して @<b>{Color Over Lifetime} を使って最後は透明にして消えるようにしたりします。

既に複数のエフェクトを作成済みで、それらのパラメータを全て変更していかなくてはならない場合、１つ１つ編集していかなければいけません。とても手間な作業となってしまいます。

== 複数同時編集を行うには

通常でできないとなれば、エディター拡張を使って拡張できる方法を模索することになります。

=== パーティクルを制御する Editor API は（まだ）存在しない

残念ながらエディター拡張用の API は用意されていません。ランタイム用の API を駆使して編集していくことになります。

=== API で（まだ）全てのプロパティーを触れるわけではない

ランタイム用の API は、ランタイムで編集可能なプロパティーのみ編集できます。なので ParticleSystem.duration は getter のみで操作はできませんし、圧倒的に触りたいプロパティーが足りません。

@<b>{Unityが公開しているロードマップによれば Unity 5.3 に API が公開される模様です。} 

=== 困ったときの SerializedObject

本書では何度も出てきていますが UnityEngine.Object であるものは全て SerializedObject としてシリアライズされています。今回はシリアライズされたものを@<b>{直接}編集することでパーティクルシステムの複数同時編集を行います。

== ParticleSystem を SerializedObject にする


SerializedObjectに変換する方法は簡単です。まずは確認のために MenuItem から SerializedObject を作成してみましょう。

下記のコードを試してみてコンソールウィンドウに SerializedObject のログが表示されるか確認してください。

//emlist{
using UnityEngine;
using UnityEditor;
using System.Linq;
public class NewBehaviourScript
{
  [MenuItem ("Assets/Get SerializedObject")]
  static void GetSerializedObject ()
  {
    var particleSystems = 
        Selection.gameObjects.Select (o => o.GetComponent<ParticleSystem> ());

    foreach (var particleSystem in particleSystems) {

      var so = new SerializedObject (particleSystem);
      Debug.Log (so);

    }
  }

  [MenuItem ("Assets/Get SerializedObject", true)]
  static bool GetSerializedObjectValidate ()
  {
    return Selection.gameObjects.Any (o => o.GetComponent<ParticleSystem> ());
  }
}
//}

== ParticleSystem のプロパティー名を知る

SerializedObject を取得できたので次は SerializedProperty を取得します。SerializedProperty を取得するにはプロパティー名を知らなけでばいけません。

=== Prefabを作成して、テキストエディターで見る

プロパティーを知るうえで最も簡単な方法は、テキストベースで保存されたアセットをテキストエディターで見ることです。ParticleSystem のプロパティーを知りたい場合は、ParticleSystem をアタッチしたゲームオブジェクトをプレハブにしましょう。

ここで、@<b>{必ずしもインスペクターで表示されているプロパティー名と同じわけではない}ということに注意してください。以下はプレハブの YAML 形式のデータです。そこに、@<code>{lengthInSec} がありますが、このプロパティーは @<code>{duration} のことを指しています。

//emlist{
--- !u!198 &19810290
ParticleSystem:
  m_ObjectHideFlags: 1
  m_PrefabParentObject: {fileID: 0}
  m_PrefabInternal: {fileID: 100100000}
  m_GameObject: {fileID: 109222}
  lengthInSec: 5
  startDelay: 0
  speed: 1
  randomSeed: 0
  looping: 1
  prewarm: 0
  playOnAwake: 1
  moveWithTransform: 1
  ...
//}

テキストエディターで直接、値を書き換えてもいいのですが Unity の管理外での変更となるため @<b>{Unity を起動したままテキストエディターで編集を行うとデータが破損してしまう}可能性があります。どうしてもテキストエディターで編集したい場合は @<b>{Unity を終了}させてから行うようにしてください。

=== SaveToSerializedFileAndForget を使う

Prefab やアセットにできない（アセットにする手法が提供されていない）ものである場合は、@<code>{UnityEditorInternal} にある @<code>{InternalEditorUtility.SaveToSerializedFileAndForget} を使用します。SaveToSerializedFileAndForget は、UnityEngine.Object をシリアライズを行いアセットとして保存するための API です。

//emlist{
InternalEditorUtility.SaveToSerializedFileAndForget (
  new Object[]{ particleSystem }, 
  "particleSystem.txt", 
  true);
//}

上記のようにAPIを呼び出すことでプロジェクトフォルダー以下に @<code>{particleSystem.txt} が生成され YAML 形式のデータを見ることができます。

=== SerializedObject.GetIterator を使う

Iterator により、全ての SerializedProperty を取得します。Uity上のみで完結させたいのであればこの方法を使いますが、プロパティー名のみを把握したい時にとっては手間となることが多いので、この方法はおすすめしません。

//emlist{
var so = new SerializedObject (particleSystem);

var prop = so.GetIterator ();

while (prop.NextVisible (true)) {
  Debug.Log (prop.propertyPath);
}
//}

=== 編集した値を保存

必要な情報は全てそろったので、あとは編集して値を適用させるだけです。GUIを実装すると説明が長くなってしまうため、今回は duration に 10 をハードコートして代入させてみます。値を代入したら最後に必ず @<code>{ApplyModifiedProperties} を呼び出して更新を行います。

//emlist{
var so = new SerializedObject (particleSystem);

so.FindProperty ("lengthInSec").floatValue = 10;

so.ApplyModifiedProperties ();
//}

== タイムライン

//image[ss04][]{

//}


通常は、ParticleSystem コンポーネントをアタッチしたゲームオブジェクトを選択することで、シーンビューでパーティクルアニメーションの@<b>{シミュレーション}を行うことができます。

//image[ss05][]{

//}

ですがこの機能は、@<b>{複数のパーティクルアニメーション}に対応していません。@<fn>{1}



なのでアニメーションを確認するには1つ1つ確認するか、複数同時に確認したいときはゲームを再生させて確認する必要があります。

これでは、作業効率的にもよくないので、タイムラインを作成してみましょう。

== タイムラインを作成してみた

//image[ss06][]{

//}

本章で説明するために作成したものなので簡素ではありますが、やりたいこと（複数のパーティクルを同時に再生）はできています。

画像だけではわかりにくいと思うので動画を用意しました。

@<href>{https://www.youtube.com/watch?v=hZvEDxXgArg,Youtube - パーティクルシステムの自作タイムライン}

今回作成したタイムラインで扱った手法や API などを紹介していきます。


== シーン内の ParticleSystem を全て取得

シーン内にある ParticleSystem は @<code>{FindObjectsOfType} で取得します。


== ParticleSystem.Simulate でパーティクルの再生

パーティクルの再生は Simulate 関数で行うことが可能です。
Simulate 関数には第2引数に @<code>{withChildren} があり、子要素のパーティクルも同時に再生してくれます。なので先ほど取得した全てのパーティクルから子要素のパーティクルを省きます。


== 親の ParticleSystem かどうかを判断する

判断の方法は簡単です。

//emlist{
bool IsRoot (ParticleSystem ps)
{
  var parent = ps.transform.parent;
  // 親のいない ParticleSystem であればルート
  if (parent == null)
    return true;

  // 親がいても ParticleSystem コンポーネントがなければルート
  return parent.GetComponent<ParticleSystem> () == false;
}
//}

== 要の TimeControl

時間制御の基板となるものは@<chapref>{time_control}で紹介しています。続きを読む前に@<chap>{time_control}を一読すると理解が深まるかもしれません。

=== GUIを作る

タイムラインは @<code>{GUILayout.HorizontalSlider} をカスタマイズしたものを扱います。EditorGUILayout.Slider は右側に FloatField が付属しており必要ないので扱いません。

通常の HorizontalSlider は、エディター上では @<img>{ss07} のように、つまみの部分が小さなボタンに見えます。これでは小さすぎて扱いづらいかもしれません。そこで HorizontalSlider のスタイルをカスタマイズして見た目を大きくします。@<fn>{2}


//indepimage[ss07]

==== Box in Box

//indepimage[ss08]


背景をボックス、さらにつまみ部分もボックスにします。その時にスライダーの高さを変更、スタイルをボックスにしたことで幅いっぱいに広がる設定が失われたので @<code>{GUILayout.ExpandWidth} を設定します。

//emlist{
GUILayout.HorizontalSlider (value, leftValue, rightValue, 
    "box", "box", GUILayout.Height (40), GUILayout.ExpandWidth (true));
//}

これで大きなスライダーで、つまみ部分がつかみやすくなりました。

==== 目盛を付ける

簡素なものですが目盛を付けておおよそどのあたりにあるかを確認できるようにします。@<fn>{3}

//emlist{
var timeLength = timeControl.maxTime - timeControl.minTime; // 時間の長さ
var gridline = timeLength * 2; // 0.5目盛ごと
var sliderRect = new Rect(lastRect); // タイムラインSliderのRect

for (int i = 1; i < gridline; i++) {

    var x = (sliderRect.width / gridline) * i;
    x += 4f - 1.5f * (i - 1);

    Handles.DrawLine (
    　　　　new Vector2 (sliderRect.x + x, sliderRect.y),
    　　　　new Vector2 (sliderRect.x + x, sliderRect.y + sliderRect.height));
    Handles.Label (
    　　　　new Vector2 (sliderRect.x + x - 10, sliderRect.y - 10),
    　　　　(timeLength / gridline * i).ToString ("0.0"));
}
//}

目盛（ライン）は @<code>{Handles} クラスを使用して描画します。また、@<code>{Handles.Label} というラベルを描画する API があり、これを使うと Vector2 の座標のみでラベルを描画します。このように、@<code>{Handles} は普段 Sceneビューで描画するために用意されたものですが、EditorWindowやインスペクターのGUI描画としても扱うことが可能です。

//image[ss09][下側にあるものは別途実装したもの。真ん中が FloatField なので値を入力できる。]{

//}

=== 矢印キーで時間移動

最後に矢印キーで時間を調整できるようにしてみます。今回は 0.01 秒刻みにします。

//emlist{
if (Event.current.type == EventType.KeyDown) {
    
    // 再生中であれば一時停止させる
    timeControl.Pause ();
    
    if (Event.current.keyCode == KeyCode.RightArrow)
      timeControl.currentTime += 0.01f;
    
    if (Event.current.keyCode == KeyCode.LeftArrow)
      timeControl.currentTime -= 0.01f;
      
    GUI.changed = true;
    Event.current.Use ();
    Repaint ();
}
//}

時間が変化したことによって GUI が変化する（しなければならない）ので、@<code>{GUI.changed} を true にして通知します。そうすることで、@<code>{EditorGUI.BeginChangeCheck} で監視しているイベントを発火できます。

そして@<b>{必ず} @<code>{Event.current.Use ();} を呼び出してください。矢印キーで時間移動をさせ、GUIを更新したのでイベントを1つ消費したことになります。既に消費したイベントでその後のイベント処理を行おうとすると何かしらの不具合が生じてきます。なので「既にイベントを消費した（Used）。なので今後のイベント処理は無視するようにする」ようにしなければいけません。


== 再生リスト

あまり説明することでもありませんが、再生リストには各パーティクルのトグル（見た目はボタン）があり、オン/オフをすることで再生するか否かを選択できます。

//emlist{
playlist [key] = GUILayout.Toggle (
                     playlist [key], key.name, 
                     EditorStyles.miniButton, 
                     GUILayout.MaxWidth (position.width / 3));
//}


//footnote[1][子要素の SubEmitter である ParticleSystem は再生される]
//footnote[2][本当はもっとリッチに作りたいのですが説明するために簡素なものとしています。]
//footnote[3][パッと見てわかれば良い設計になっているので、メモリの位置が正確ではないです。ごめんなさい！]