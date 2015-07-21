using UnityEngine;
using UnityEditor;

public class ExampleAsset : ScriptableObject
{
	[SerializeField]
	string str;


	[SerializeField, Range (0, 10)]
	int number;

	[MenuItem ("Example/Create ExampleAsset Instance")]
	static void CreateExampleAssetInstance ()
	{
		var exampleAsset = CreateInstance<ExampleAsset> ();

		AssetDatabase.CreateAsset (exampleAsset, "Assets/Editor/ExampleAsset.asset");
		AssetDatabase.Refresh ();
	}

	[MenuItem ("Example/Load ExampleAsset")]
	static void LoadExampleAsset ()
	{
		var exampleAsset = AssetDatabase.LoadAssetAtPath<ExampleAsset> ("Assets/Editor/ExampleAsset.asset");
	} 
}
