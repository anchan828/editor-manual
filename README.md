Editor拡張入門
=============

## 目的

* Editor拡張に対する敷居を下げるための入門書
* 金儲け

## 書けそうな内容

* MenuItem
	* ショートカットキーでメニュー実行
	* ValidateFunctionを使う
	* priority
	* 'Create/Folder/Editor'とかどう？
* EditorWindow
	* PreferenceItem
		* Editorの設定を行うときはPreferenceWindowにメニューを追加しよう
	* PopupWindow
	* IHasCustomMenu
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
* ScriptTemplates
	* _using UnityEditor;_を追加したスクリプトファイル
	* _GetWindow_を追加したEditorWindowファイル	



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

* リフレクション満載の章

## 部立て

### エディタ拡張とは

#### エディタ拡張とは

#### エディタ拡張でもたらす恩恵

### 第1部 エディタ拡張を知る

#### AssetStoreにあるエディタ拡張のアセットを使ってみる
#### Web上にあるエディタ拡張のアセットを使ってみる


### 第2部 はじめてのエディタ拡張

#### Editorフォルダ
#### 既に用意されているエディタ拡張

### 第3部 エディタ拡張を使う

#### MenuItem
#### EditorWindow
#### EditorGUI
#### Callbacks
#### EditorApplication


### 第4部 仕組みを理解してエディタ拡張を使う

#### PropertyDrawer
#### ScriptableObject
#### CustomEditor
#### CustomPreview
#### Undo
#### DragAndDrop


### 第5部 更なる深みへ

#### ControlID
#### GUIStyleDraw

### 第6部  エディタ拡張の闇へようこそ

#### Refrection
#### 逆コンパイラ
#### 「何かリフレクション使うもの」