using UnityEditor;
using UnityEngine;

public class Example : EditorWindow
{
	[MenuItem("CONTEXT/Transform/Example1")]
	static void Example1 (MenuCommand menuCommand)
	{
		// 実行したTransformの情報が取得できる
		Debug.Log (menuCommand.context);
	}
}