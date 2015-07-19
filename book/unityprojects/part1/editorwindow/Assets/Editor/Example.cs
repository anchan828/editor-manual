using UnityEditor;
using UnityEngine;
public class Example : EditorWindow
{
	[SerializeField]
	string text;

	[SerializeField]
	bool boolean;

	[MenuItem ("Assets/Save EditorWindow")]
	static void SaveEditorWindow ()
	{
		AssetDatabase.CreateAsset (CreateInstance<Example> (), "Assets/Example.asset");
		AssetDatabase.Refresh ();
	}
}