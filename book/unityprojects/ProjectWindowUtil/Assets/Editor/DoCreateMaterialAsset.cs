using UnityEngine;
using UnityEditor;
using UnityEditor.ProjectWindowCallback;

public class DoCreateMaterialAsset : EndNameEditAction
{
	public override void Action (int instanceId, string pathName, string resourceFile)
	{
		var mat = (Material)EditorUtility.InstanceIDToObject (instanceId);

		mat.color = Color.red;

		AssetDatabase.CreateAsset (mat, pathName);
		AssetDatabase.ImportAsset (pathName);
		ProjectWindowUtil.ShowCreatedAsset (mat);
	}
}
