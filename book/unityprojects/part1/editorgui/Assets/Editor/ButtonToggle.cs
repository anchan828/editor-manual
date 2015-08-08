using UnityEngine;
using System.Collections;
using UnityEditor;

public class ButtonToggle : EditorWindow
{
	[MenuItem ("Window/ButtonToggle")]
	static void Open ()
	{
		GetWindow<ButtonToggle> ();
	}

	int selected;

	void OnGUI ()
	{
		selected = GUILayout.SelectionGrid (selected, 
			new string[]{ "1", "2", "3" }, 1, "PreferencesKeysElement");
	}
}


/*
//#@@range_begin(ButtonToggle_Pattern1)
bool on;

void OnGUI ()
{
	on = GUILayout.Toggle (on, on ? "on" : "off", "button");
}
//#@@range_end(ButtonToggle_Pattern1)
*/

/*
//#@@range_begin(ButtonToggle_Pattern2)
bool one, two, three;

void OnGUI ()
{
	using (new EditorGUILayout.HorizontalScope ()) {
		one = GUILayout.Toggle (one, "1", EditorStyles.miniButtonLeft);
		two = GUILayout.Toggle (two, "2", EditorStyles.miniButtonMid);
		three = GUILayout.Toggle (three, "3", EditorStyles.miniButtonRight);
	}
}
//#@@range_end(ButtonToggle_Pattern2)
*/

/*
//#@@range_begin(ButtonToggle_Pattern3)
int selected;

void OnGUI ()
{
	selected = GUILayout.Toolbar (selected, new string[]{ "1", "2", "3" });
}
//#@@range_end(ButtonToggle_Pattern3)
*/

/*
//#@@range_begin(ButtonToggle_Pattern4)
int selected;

void OnGUI ()
{
	selected = GUILayout.Toolbar (selected, 
	    new string[]{ "1", "2", "3" }, EditorStyles.toolbarButton);
}
//#@@range_end(ButtonToggle_Pattern4)
*/

/*
//#@@range_begin(ButtonToggle_Pattern5)
int selected;

void OnGUI ()
{
	selected = GUILayout.SelectionGrid (selected, 
	    new string[]{ "1", "2", "3" },1, "PreferencesKeysElement");
}
//#@@range_end(ButtonToggle_Pattern5)
*/