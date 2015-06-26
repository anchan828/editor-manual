= 概要

//indepimage[frontispiece]
//pagebreak

== Unityエディタの完成度はいつも8割程度

私がUnityエディタについて説明するとき、いつも「Unityのエディタはいつも8割程度の完成度でリリースされています」と言っています。
これはUnityのクオリティが8割というわけではありません。そのようにしているのは「これ以上の完成度を持った機能はユーザーの手で実装して欲しい」という狙いがあるからです。
もし、FPSに特化したUnityであればRPGが作りにくくなるかもしれません。3Dゲームに特化すれば2Dゲームが作りにくくなるかもしれません。
このように、これ以上の完成度を持つとユーザーが好きなゲームを作成できなくなる可能性があります。そこでUnityは、Unityで出来る（想定している）ことの10割のうち、2割をユーザーが自由にカスタマイズ出来るようにしました。

== 2割の使い道はアセットストアにあふれている

ユーザーはこの2割を使ってカスタマイズしていくことになります。プロジェクトに合ったカスタマイズをすることで、Unityの性能を10割、もしかすると10割以上引き出すことができるかもしれません。


どのようにして2割を補っていくかですが、そのアイデアはアセットストアにあふれています。

パラメーターを編集している時のインスペクターが使いにくいですか？では、インスペクターを拡張しているアセットを見てみましょう。
ノベルゲームの作り方＆管理の方法がわかりませんか？では、フレームワークを提供しているアセットを見てみましょう。

このように、みなさんが「不便だな、もうちょっとここ良くならないかな？」と思っていることは大抵アセットストアで公開済みかもしれません。または、特殊なプロジェクト事情を解決するヒントになるかもしれません。皆さん通る道は同じということですね。

このようにして残りの2割を補うことで、ユーザー＆プロジェクト毎に最適化されたUnityエディタが完成します。

== プロジェクトのワークフロー

　エディタ拡張の恩恵が大きくなるのは、仕事やサークルでゲームを開発するときのような@<b>{多人数}でのゲーム開発です。

　通常プロジェクトを進めていくには「ワークフロー」が存在しなければいけません。ワークフローとは、物事の流れのように一連の作業の流れを定義したものです。これにより個々の作業プロセスが決まり、他人から見てもそのプロセスの流れや繋ぎがわかりやすくなってフローの受け渡しが円滑になります。特にこのワークフローは複数人で作業している時に大きく発揮します。

//quote{
問題が発生した場合、どのフロー（プロセス）の時に発生したかがわかるようになり、「問題の把握」「進捗具合が把握」しやすくなる「見える化@<fn>{mieru-ka}」が行われますが、本書ではあまり関係ないので詳しい説明は行いません。
//}
//footnote[mieru-ka][見える化: 作業におけるタスクを常に見えるようにして、問題が発生してもすぐに解決できる環境のこと。有名なのは、かんばんボード]

== ケアレスミスをできるだけ避けたい

　Unityは「インスペクター」「ヒエラルキー」「シーン」「プロジェクト」と言った、GUIベースの上で操作を行いゲームを作成します。
//image[ss01][初期状態のUnityエディタ]{
//}

　例えば、「コンポーネントをゲームオブジェクトにアタッチする」といった作業をするとしましょう。アタッチする方法は、インスペクターの@<b>{Add Component}ボタンや上部のComponentメニューから行います。もし「@<b>{コンポーネントをアタッチするのを忘れていた}」 ためにエラーが発生している場合、アタッチする方法/対象が単純な構造であればすぐに原因は特定出来るでしょうが、複雑になればなるほど原因の特定は困難になっていきます。

　Unityのインスペクターでは、自由に「ゲームオブジェクトにコンポーネントを追加」「コンポーネントの値をマウスドラッグで変更/微調整」などができます。しかもゲーム再生中にも変更でき、とても素晴らしい機能です。ですが、ワークフローの上にある決められた値を設定したりする「決まった作業」に限ってはこの自由な機能が邪魔になるかもしれません。

特に:

 * 決められた値なのに間違えた値を設定してしまう
 * 触ってはいけない値を変更してしまう
 * あやまってオブジェクトを削除してしまう

というようなケアレスミスは避けて通れません。特にGUIベースのUnityでは多く発生してしまいます。
これは筆者の考えなのですが、ケアレスミスをしてしまう大半の部分は単純な作業の部分が多く、（実現可能かはともかく）ケアレスミスをなくすための解決は容易なものと考えています。

 * 決められた値なのに間違えた値を設定してしまう → 仕様書のデータから値を抽出して入力する
 * インスペクターで触ってはいけない値を変更してしまう → その値に触れないようにする
 * 誤ってオブジェクトを削除してしまう → オブジェクトを削除できないようにする。または確認ダイアログを出すようにする

というように（極端でわかりやすい）一例を出しましたが、これらはどれもエディタ拡張で実現可能です。

== エディタ拡張の特徴

