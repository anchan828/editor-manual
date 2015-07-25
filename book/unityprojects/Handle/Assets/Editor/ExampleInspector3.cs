using UnityEngine;
using UnityEditor;

[CustomEditor (typeof(Example3))]
public class ExampleInspector3 : Editor
{
	int windowID = 1234;
	Rect windowRect;

	void OnSceneGUI ()
	{
		Handles.BeginGUI ();

		windowRect = GUILayout.Window (windowID, windowRect, (id) => {

			EditorGUILayout.LabelField ("Label");

			EditorGUILayout.ToggleLeft ("Toggle", false);

			GUILayout.Button ("Button");

			GUI.DragWindow();
		
		}, "Window", GUILayout.Width (100));

		Handles.EndGUI ();
	}
}



public class HandleGUIScope : GUI.Scope
{
	public HandleGUIScope ()
	{
		Handles.BeginGUI ();
	}

	protected override void CloseScope ()
	{
		Handles.EndGUI ();
	}
}