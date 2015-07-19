using UnityEngine;
using System.Collections;
using System;
using UnityEditor;
using UnityEditorInternal;

public class ColorPresetLibrary
{

	public static readonly Type colorLibType = Types.GetType ("UnityEditor.ColorPresetLibrary", "UnityEditor.dll");

	public static void Create (params Color[] colors)
	{
		var colorLib = ScriptableObject.CreateInstance (colorLibType);

		Add (colorLib, colors);

		ProjectWindowUtil.CreateAsset (colorLib, "New ColorLibrary.colors");
	}

	public static ScriptableObject ColorLibField (ScriptableObject colorLib)
	{
		return EditorGUILayout.ObjectField ("Color Lib", colorLib, colorLibType, false) as ScriptableObject;
	}

	public static void Add (ScriptableObject colorLib, params Color[] colors)
	{
		foreach (var color in colors) {
			colorLibType.GetMethod ("Add").Invoke (colorLib, new object[]{ color, color.ToHexStringRGB () });
		}
	}

	public static bool IsColorPresetLibrary (ScriptableObject colorLib)
	{
		return colorLib.GetType ().FullName == "UnityEditor.ColorPresetLibrary";
	}

	public static void Save (ScriptableObject colorLib)
	{
		EditorUtility.SetDirty(colorLib);
		AssetDatabase.SaveAssets ();
		AssetDatabase.Refresh ();
		ActiveEditorTracker.sharedTracker.ForceRebuild ();
	}
}
