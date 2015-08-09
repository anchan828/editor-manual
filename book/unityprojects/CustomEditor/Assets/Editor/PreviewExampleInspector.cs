using UnityEngine;
using UnityEditor;

[CustomEditor (typeof(PreviewExample))]
public class PreviewExampleInspector : Editor
{
	PreviewRenderUtility previewRenderUtility;
	GameObject previewObject;

	void OnEnable ()
	{
		previewRenderUtility = new PreviewRenderUtility (true);
		previewRenderUtility.m_CameraFieldOfView = 30f;

		previewRenderUtility.m_Camera.farClipPlane = 1000;
		previewRenderUtility.m_Camera.nearClipPlane = 0.3f; 

		var component = (Component)target;
		previewObject = component.gameObject;
	}

	void OnDisable ()
	{
		previewRenderUtility.Cleanup ();
		previewRenderUtility = null;
		previewObject = null;
	}

	public override bool HasPreviewGUI ()
	{
		return true;
	}

	public override void OnPreviewGUI (Rect r, GUIStyle background)
	{
		previewRenderUtility.BeginPreview (r, background);

		var previewCamera = previewRenderUtility.m_Camera;

		previewCamera.transform.position =
			previewObject.transform.position + new Vector3 (0, 2.5f, -5);

		previewCamera.transform.LookAt (previewObject.transform);

		previewCamera.Render ();

		previewRenderUtility.EndAndDrawPreview (r);

	}
}