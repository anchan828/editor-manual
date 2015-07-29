using UnityEngine;
using UnityEditor;

[CustomEditor (typeof(Character))]
public class CharacterInspector : Editor
{
	SerializedProperty exampleProperty;
	PreviewRenderUtility previewRenderUtility;
	GameObject previewObject;
	Vector3 centerPosition;

	void OnEnable ()
	{
		var previewLayer = LayerMask.NameToLayer ("Preview");
		previewRenderUtility = new PreviewRenderUtility (true);
		previewRenderUtility.m_CameraFieldOfView = 30f;
		previewRenderUtility.m_Camera.cullingMask = 1 << previewLayer;

		var character = (Character)target;
		previewObject = Instantiate (character.gameObject);
		previewObject.hideFlags = HideFlags.HideAndDontSave;

		previewObject.layer = previewLayer;
		foreach (Transform transform in previewObject.transform) {
			transform.gameObject.layer = previewLayer;
		}


		centerPosition = GetCenterPosition ();


		previewObject.SetActive (false);

	}

	Vector3 GetCenterPosition(){
		var currentMode = Tools.pivotMode;
		Tools.pivotMode = PivotMode.Center;
		var centerPosition = Tools.handlePosition;
		Tools.pivotMode = currentMode;
		return centerPosition;
	}

	void OnDisable ()
	{
		DestroyImmediate (previewObject);
		previewRenderUtility.Cleanup ();
		previewRenderUtility = null;
		UnityEditorInternal.InternalEditorUtility.RepaintAllViews ();
	}

	public override bool HasPreviewGUI ()
	{
		return true;
	}

	public override void OnInteractivePreviewGUI (Rect r, GUIStyle background)
	{
		previewRenderUtility.BeginPreview (r, background);

		var drag = Vector2.zero;

		if (Event.current.type == EventType.MouseDrag) {
			drag = Event.current.delta;
		}

		previewRenderUtility.m_Camera.transform.position = centerPosition + Vector3.forward * -5;
	
		previewObject.transform.RotateAround (centerPosition, Vector3.up, -drag.x);
		previewObject.transform.RotateAround (centerPosition, Vector3.right, -drag.y);

		previewObject.SetActive (true);
		previewRenderUtility.m_Camera.Render ();
		previewObject.SetActive (false);

		previewRenderUtility.EndAndDrawPreview (r);

		if (drag != Vector2.zero)
			Repaint ();
	}
}
