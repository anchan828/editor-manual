using UnityEngine;
using System.Collections;
using UnityEditor;
using System;

public class PresetMenuItems
{
	[MenuItem ("Assets/Create/Presets/ColorLibrary")]
	static void CreateColorLibrary ()
	{
		ColorPresetLibrary.Create ();
	}
}
