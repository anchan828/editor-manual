using UnityEngine;
using UnityEditor;
using System.Reflection;

[CustomEditor (typeof(PreviewExample))]
public class PreviewExampleInspector : Editor
{
	PreviewRenderUtility previewRenderUtility;
	GameObject previewObject;
	Vector3 centerPosition;

	void OnEnable ()
	{
		var flags = BindingFlags.Static | BindingFlags.NonPublic;
		var propInfo = typeof(Camera).GetProperty ("PreviewCullingLayer", flags);
		int previewLayer = (int)propInfo.GetValue (null, new object[0]);

		previewRenderUtility = new PreviewRenderUtility (true);
		previewRenderUtility.m_CameraFieldOfView = 30f;
		previewRenderUtility.m_Camera.cullingMask = 1 << previewLayer;

		var component = (Component)target;
		previewObject = Instantiate (component.gameObject);
		previewObject.hideFlags = HideFlags.HideAndDontSave;

		previewObject.layer = previewLayer;
		foreach (Transform transform in previewObject.transform) {
			transform.gameObject.layer = previewLayer;
		}


		var isPrefab = PrefabUtility.GetPrefabType (target) == PrefabType.Prefab;

		centerPosition = isPrefab ? Vector3.zero : GetCenterPosition ();

		if (isPrefab)
			previewObject.transform.position = Vector3.zero;

		previewObject.SetActive (false);

		RotatePreviewObject (new Vector2 (-120, 20));
	}

	Vector3 GetCenterPosition ()
	{
		var currentMode = Tools.pivotMode;
		Tools.pivotMode = PivotMode.Center;
		var centerPosition = Tools.handlePosition;
		Tools.pivotMode = currentMode;
		return centerPosition;
	}

	public override GUIContent GetPreviewTitle ()
	{
		return new GUIContent (target.name+ " Preview");
	}

	void OnDisable ()
	{
		DestroyImmediate (previewObject);
		previewRenderUtility.Cleanup ();
		previewRenderUtility = null;
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

		RotatePreviewObject (drag);

		previewObject.SetActive (true);
		previewRenderUtility.m_Camera.Render ();
		previewObject.SetActive (false);

		previewRenderUtility.EndAndDrawPreview (r);

		if (drag != Vector2.zero)
			Repaint ();
	}

	private void RotatePreviewObject (Vector2 drag)
	{
		previewObject.transform.RotateAround (centerPosition, Vector3.up, -drag.x);
		previewObject.transform.RotateAround (centerPosition, Vector3.right, -drag.y);
	}
}