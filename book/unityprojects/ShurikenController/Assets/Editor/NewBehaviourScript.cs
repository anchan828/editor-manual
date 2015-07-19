using UnityEngine;
using UnityEditor;
using System.Linq;
public class NewBehaviourScript
{
	[MenuItem ("Assets/Get SerializedObject")]
	static void GetSerializedObject ()
	{
		var particleSystems = Selection.gameObjects.Select (o => o.GetComponent<ParticleSystem> ());

		foreach (var particleSystem in particleSystems) {
			
			var so = new SerializedObject (particleSystem);

			so.FindProperty ("lengthInSec").floatValue = 10;

			so.ApplyModifiedProperties ();
		}
	}

	[MenuItem ("Assets/Get SerializedObject", true)]
	static bool GetSerializedObjectValidate ()
	{
		return Selection.gameObjects.Any (o => o.GetComponent<ParticleSystem> ());
	}
}
