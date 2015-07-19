using UnityEngine;
using UnityEditor;

public class Example : EditorWindow
{
	TimeControl timeControl;

	[MenuItem ("Window/Example")]
	static void Open ()
	{
		GetWindow<Example> ();
	}

	void OnEnable ()
	{
		timeControl = new TimeControl ();
		timeControl.SetMinMaxTime (0, 10);
	}

	void OnGUI ()
	{
		var buttonText = timeControl.isPlaying ? "Pause" : "Play";

		if (GUILayout.Button (buttonText)) {

			if (timeControl.isPlaying)
				timeControl.Pause ();
			else
				timeControl.Play ();

		}

		timeControl.currentTime = EditorGUILayout
			.Slider (timeControl.currentTime, 0, 10);

		if (timeControl.isPlaying)
			Repaint ();
	}
}
