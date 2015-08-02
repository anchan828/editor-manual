using UnityEngine;
using UnityEditor;

public class Example
{
	[MenuItem ("Example/Create Cube")]
	static void CreateCube ()
	{
		var cube = GameObject.CreatePrimitive (PrimitiveType.Cube);
		Undo.RegisterCreatedObjectUndo (cube, "Create Cube");
	}

	[MenuItem ("Example/Random Rotate")]
	static void RandomRotate ()
	{
		var transform = Selection.activeTransform;

		if (transform) {
			Undo.willFlushUndoRecord += () => Debug.Log ("flush");

			Undo.postprocessModifications += (modifications) => {
				Debug.Log ("modifications");
				return modifications;
			};

			Undo.RecordObject (transform, "Rotate " + transform.name);
			Debug.Log ("recorded");

			transform.rotation = Random.rotation;
			Debug.Log ("changed");
		}
	}

	[MenuItem ("Example/Change PlayerInfo")]
	static void ChangePlayerInfo ()
	{
		var player = Selection.activeGameObject.GetComponent<Player> ();

		if (player) {
			Undo.RecordObject (player, "Change PlayerInfo");
			player.info = new PlayerInfo {
				name = "New PlayerName",
				hp = Random.Range (0, 10)
			};
		}
	}

	[MenuItem ("Example/Group Name")]
	static void GroupName ()
	{
		
	}

}