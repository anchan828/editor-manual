using UnityEngine;
using UnityEditor;
using System.IO;

public class NewBehaviourScript
{
	[MenuItem ("Assets/Create ExampleAssets")]
	static void CreateExampleAssets ()
	{
		var resourceFile = Path.Combine (EditorApplication.applicationContentsPath, "Resources/ScriptTemplates/81-C# Script-NewBehaviourScript.cs.txt");

		// cs は "cs Script Icon"
		// js は "js Script Icon"
		Texture2D csIcon = EditorGUIUtility.IconContent ("cs Script Icon").image as Texture2D;
		var endNameEditAction = ScriptableObject.CreateInstance<DoCreateScriptAsset> ();

		ProjectWindowUtil.StartNameEditingIfProjectWindowExists (0, endNameEditAction, "NewBehaviourScript.cs", csIcon, resourceFile);
	}
}