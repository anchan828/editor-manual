Editor拡張入門
=============

## 目的

* Editor拡張に対する敷居を下げるための入門書
* 金儲け

## 書けそうな内容

* MenuItem
	* ショートカットキーでメニュー実行
	* ValidateFunctionを使う
* EditorWindow
	* PreferenceItem
		* Editorの設定を行うときはPreferenceWindowにメニューを追加しよう
	* PopupWindow
* xxDrawer
	* PropertyDrawer
		* すでに用意されているPropertyDrawer
			* MultilineAttribute
			* RangeAttribute
			* TextAreaAttribute
		* 自作する
	* DecoratorDrawer
		* すでに用意されているDecoratorDrawer
			* ContextMenuItemAttribute
			* SpaceAttribute
			* TooltipAttribute
		* 自作する
* CustomEditor
	* Editor
	* CanEditMultipleObjects
		 * 複数のオブジェクトの値を変更する
	* DisallowMultipleComponent
		* 同じコンポーネントをアタッチしないようにする
* CustomPreview
	* ScriptableObjectのインスペクターにプレビューを追加する
* SerializedObject
	* SerializedProperty
* ScriptableObject
* EditorGUI
	* Handle
	* xxScope
* AssetPostprocessor
	* AssetImporter
* BuildPipline
	* AssetBundle
	* BuildPlayer
* EditorApplication
	* playmodeStateChanged
		* [エディタ再生時にiTunesを一時停止＆エディタ再生をやめた時にiTunesを再生する](https://gist.github.com/anchan828/4af242328bbdfbfd6c41)
		* [コンパイルエラーが出たまま再生ボタンを押すと、ドラクエの呪いの効果音を再生する](https://gist.github.com/anchan828/13c901d5daa9f7502d65)
* AnimatorController
* Callbacks
	* PostProcessBuildAttribute
	* PostProcessSceneAttribute
	* OnOpenAssetAttribute
* Sprite
	* SpritePacker
* AssetDatabase
	* ProjectWindowUtil
* AnimationUtility
* HierarchySort
* Undo
* Profiler
* Command Line
* PrefabUtility
* Selection
* DragAndDrop
* ControlID
* Event
* ObjectPicker




### 気になる内容

* MaterialDrawer
* SaveAssetsProcessor
* ShaderGUI
* ConstantDataAttribute
* SelectionBaseAttribute


## 書けそうな構成

*  仕組みはわからないけどとりあえずエディタ拡張の機能を使える章
	* あくまで「使う側」としての読み物にする
* 仕組みを理解して何かを自作する 章
	* 比較的軽いもの
		* SerializedPropertyのパスやObjectPicker
* もっと仕組みを理解して何かを自作する章
	* コントロールID
	* Event
	* Serialize 

* どういう仕組みかわからなくても使える技術
	* MenuItem実行する
	* EditorWindow作成する
	* PropertyDrawerつかう
	* Callbacks
	* EditorApplication
	* EditorGUI
	* Selection
* （SerializedObjectとか）仕組みを理解しないと使えない技術
	* Undo
	* DragAndDrop
	* Event
	* ObjectPicker
	* ControlID
	* AnimationUtility