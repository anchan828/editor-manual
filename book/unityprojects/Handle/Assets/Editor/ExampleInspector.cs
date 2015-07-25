using UnityEngine;
using UnityEditor;

[CustomEditor (typeof(Example))]
public class ExampleInspector : Editor
{
	Vector3 snap;

	void OnEnable ()
	{
		var snapX = EditorPrefs.GetFloat ("MoveSnapX", 1f);
		var snapY = EditorPrefs.GetFloat ("MoveSnapY", 1f);
		var snapZ = EditorPrefs.GetFloat ("MoveSnapZ", 1f);
		snap = new Vector3 (snapX, snapY, snapZ);
	}

	void OnSceneGUI ()
	{
		var component = target as Example;
		Tools.current = Tool.None;

		var transform = component.transform;

		transform.position = PositionHandle (transform);
	}

	Vector3 PositionHandle (Transform transform)
	{
		var position = transform.position;

		var size = HandleUtility.GetHandleSize (position);

		// X 軸
		Handles.color = Handles.xAxisColor;
		position = Handles.Slider (position, transform.right, size, Handles.ArrowCap, snap.x); 

		// Y 軸
		Handles.color = Handles.yAxisColor;
		position = Handles.Slider (position, transform.up, size, Handles.ArrowCap, snap.y); 

		// Z 軸
		Handles.color = Handles.zAxisColor;
		position = Handles.Slider (position, transform.forward, size, Handles.ArrowCap, snap.z); 

		return position;
	}
}
