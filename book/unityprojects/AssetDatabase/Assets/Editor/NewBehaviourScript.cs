using UnityEngine;
using System.Collections;
using UnityEditor;
using System.IO;

public class NewBehaviourScript
{
	[MenuItem ("Assets/Create AnimationClip")]
	static void Example ()
	{
		var clip = new AnimationClip ();
		AssetDatabase.CreateAsset (clip, "Assets/New AnimationClip.anim");
	}

	[MenuItem ("Assets/Change FrameRate")]
	static void ChangeFrameRate ()
	{
		var clip = AssetDatabase.LoadAssetAtPath<AnimationClip> ("Assets/New AnimationClip.anim");
		clip.frameRate++;
	}


	[MenuItem ("Assets/Create TextAsset")]
	static void CreateTextAsset ()
	{
		var path = "Assets/New TextAsset.txt";
		var text = "text";
		File.WriteAllText (path, text);
		AssetDatabase.ImportAsset (path);
	}
		
}
