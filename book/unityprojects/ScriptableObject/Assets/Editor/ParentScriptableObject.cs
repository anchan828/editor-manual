using UnityEngine;
using UnityEditor;

public class ParentScriptableObject : ScriptableObject
{
	const string PATH = "Assets/Editor/New ParentScriptableObject.asset";
//	[SerializeField]
//	ChildScriptableObject child1;
//
//
//
//	[SerializeField]
//	ChildScriptableObject child2;
//
//
//	[MenuItem ("Assets/Creat ScriptableObject in ScriptableObject")]
//	static void Create ()
//	{
//		var parent = ScriptableObject.CreateInstance<ParentScriptableObject> ();
//
//		parent.child1 = CreateScriptableObjectWithHideFlags<ChildScriptableObject> ("child1", HideFlags.HideInHierarchy);
//		parent.child2 = CreateScriptableObjectWithHideFlags<ChildScriptableObject> ("child2", HideFlags.HideInHierarchy);
//
//
//		AssetDatabase.CreateAsset (parent, PATH);
//
//		AssetDatabase.AddObjectToAsset (parent.child1, PATH);
//		AssetDatabase.AddObjectToAsset (parent.child2, PATH);
//		AssetDatabase.ImportAsset (PATH);
//	}

	[SerializeField]
	ChildScriptableObject child;

	[MenuItem ("Assets/Create ScriptableObject")]
	static void CreateScriptableObject ()
	{
		var parent = ScriptableObject.CreateInstance<ParentScriptableObject> ();
		parent.child = ScriptableObject.CreateInstance<ChildScriptableObject> ();

		parent.child.hideFlags = HideFlags.HideInHierarchy;

		AssetDatabase.AddObjectToAsset (parent.child, PATH);

		AssetDatabase.CreateAsset (parent, PATH);
		AssetDatabase.ImportAsset (PATH);
	}


//	static T CreateScriptableObjectWithHideFlags<T> (string name, HideFlags flags) where T : ScriptableObject
//	{
//		var scriptableObject = ScriptableObject.CreateInstance<T> ();
//		scriptableObject.name = name;
//		scriptableObject.hideFlags = flags;
//		return scriptableObject;
//	}
//
	[MenuItem ("Assets/Remove ChildScriptableObject")]
	static void Remove ()
	{
		var parent = AssetDatabase.LoadAssetAtPath<ParentScriptableObject> (PATH);	

		Object.DestroyImmediate (parent.child, true);

		parent.child = null;

		AssetDatabase.ImportAsset (PATH);
	}
}
