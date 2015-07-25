using UnityEngine;
using UnityEditor;
using UnityEditorInternal;

[CustomEditor (typeof(Example2))]
public class ExampleInspector2 : Editor
{
	ReorderableList reorderableList;
	Example2 component;

	void OnEnable ()
	{
		Tools.current = Tool.None;
		component = target as Example2;
		reorderableList = new ReorderableList (component.vertexes, typeof(Vector3));

		reorderableList.drawElementCallback = (rect, index, isActive, isFocused) => {
			component.vertexes [index] = EditorGUI.Vector3Field (rect, GUIContent.none, component.vertexes [index]);
		};

		reorderableList.onAddCallback = (list) => {
			ArrayUtility.Add (ref component.vertexes, Vector3.zero);
			ActiveEditorTracker.sharedTracker.ForceRebuild ();
		};
		reorderableList.onRemoveCallback = (list) => {
			ArrayUtility.Remove (ref component.vertexes, component.vertexes [list.index]);
			ActiveEditorTracker.sharedTracker.ForceRebuild ();
		};
		reorderableList.onChangedCallback = (list) => SceneView.RepaintAll ();
	}

	public override void OnInspectorGUI ()
	{
		reorderableList.DoLayoutList ();
	}

	void OnSceneGUI ()
	{
		var vertexes = component.vertexes;


		for (int i = 0; i < vertexes.Length; i++) {

			vertexes [i] = Handles.PositionHandle (vertexes [i], Quaternion.identity);

		}

		Handles.DrawAAPolyLine (vertexes);
	}
}
