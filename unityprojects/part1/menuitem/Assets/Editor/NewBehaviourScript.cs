using UnityEditor;
using UnityEngine;
using System.IO;

public class NewBehaviourScript
{
		[MenuItem("Assets/Create/TemplateFolder/Editor", false, 1)]
		static void CreateEditorFolder ()
		{
				CreateFolder ("Editor");
		}

		[MenuItem("Assets/Create/TemplateFolder/Resources", false, 2)]
		static void CreateResourcesFolder ()
		{
				CreateFolder ("Resources");
		}

		[MenuItem("Assets/Create/TemplateFolder/Scripts", false, 3)]
		static void CreateScriptsFolder ()
		{
				CreateFolder ("Scripts");
		}

		[MenuItem("Assets/Create/TemplateFolder/Prefabs", false, 4)]
		static void CreatePrefabsFolder ()
		{
				CreateFolder ("Prefabs");
		}
    
		[MenuItem("Assets/Create/TemplateFolder/Shaders", false, 5)]
		static void CreateShadersFolder ()
		{
				CreateFolder ("Shaders");
		}

		static void CreateFolder (string name)
		{
				string path = "Assets";

				if (Selection.activeObject != null) {
						var assetPath = AssetDatabase.GetAssetPath (Selection.activeObject);
						path = Directory.Exists (assetPath) ? assetPath : Path.GetDirectoryName (assetPath);
				}
				AssetDatabase.CreateFolder (path, name);
		}

}