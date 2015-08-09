using UnityEngine;
using System.Collections;
using UnityEditor;
using System.Reflection;

public class NewBehaviourScript : EditorWindow
{

	Object[] objs;

	[InitializeOnLoadMethod]
	static void OnEnable ()
	{
		var flags = BindingFlags.Static | BindingFlags.NonPublic;
		MethodInfo info = typeof(EditorGUIUtility).GetMethod ("GetEditorAssetBundle", flags);
		AssetBundle bundle = info.Invoke (null, new object[0]) as AssetBundle;
		foreach (var name in bundle.GetAllAssetNames()) {
			Debug.Log (name);
		}
	}


	[InitializeOnLoadMethod]
	static void Test ()
	{
//		Debug.Log (EditorGUIUtility.Load ("avatar/materials/root-red.mat"));


	}
}