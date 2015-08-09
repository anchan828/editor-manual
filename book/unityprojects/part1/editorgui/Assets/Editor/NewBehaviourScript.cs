using UnityEngine;
using UnityEditor;
using UnityEditor.AnimatedValues;
using UnityEngine.Events;

public class NewBehaviourScript : EditorWindow
{
	[MenuItem ("Window/Example")]
	static void Open ()
	{
		GetWindow<NewBehaviourScript> ();
	}


	float angle = 0;

	void OnGUI ()
	{
		EditorGUILayout.Space ();

		angle = EditorGUILayout.Knob (Vector2.one * 64, angle, 0, 360, "度", Color.gray, Color.red, true);
	}
}
