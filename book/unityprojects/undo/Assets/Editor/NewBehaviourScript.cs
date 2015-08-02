using UnityEngine;
using UnityEditor;

public class NewBehaviourScript : EditorWindow
{
	int groupID = 0;

	[MenuItem ("Window/Example")]
	static void Open ()
	{
		GetWindow<NewBehaviourScript> ();
	}

	void OnEnable ()
	{
		groupID = Undo.GetCurrentGroup ();
	}

	void OnGUI ()
	{
		if (GUILayout.Button ("Cube 作成")) {
			var cube = GameObject.CreatePrimitive (PrimitiveType.Cube);
			Undo.RegisterCreatedObjectUndo (cube, "Create Cube");
		}

		if (GUILayout.Button ("Plane 作成")) {
			var plane = GameObject.CreatePrimitive (PrimitiveType.Plane);
			Undo.RegisterCreatedObjectUndo (plane, "Create Plane");
		}

		if (GUILayout.Button ("Cylinder 作成")) {
			var cylinder = GameObject.CreatePrimitive (PrimitiveType.Cylinder);
			Undo.RegisterCreatedObjectUndo (cylinder, "Create Cylinder");
		}
	}

	void OnDisable ()
	{
		Undo.CollapseUndoOperations (groupID);
	}
}
