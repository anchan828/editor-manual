using UnityEngine;
using System.IO;
using UnityEditor;
using System.Collections.Generic;
using UnityEditorInternal;
using System.Linq;

[CustomEditor (typeof(DefaultAsset))]
public class SceneInspector : Editor
{
	
	bool isSceneInspector = false;
	GameObject scenePrefab = null;
	Dictionary<Editor,bool> activeEditors = new Dictionary<Editor, bool> ();

	void OnEnable ()
	{
		isSceneInspector = Path.GetExtension (AssetDatabase.GetAssetPath (target)) == ".unity";

		if (isSceneInspector) {
			RefreshActiveEditor ();
			Undo.undoRedoPerformed += () => {
				RefreshActiveEditor ();
				Repaint ();
			};
		}



	}

	void OnDisable ()
	{
		scenePrefab = null;
		isSceneInspector = false;
		activeEditors = null;
		ActiveEditorTracker.sharedTracker.ClearDirty ();
		var t = Types.GetType ("UnityEditor.GenericInspector", "UnityEditor.dll");
		foreach (var item in Resources.FindObjectsOfTypeAll<Editor> ()) {
			if (item.GetType () == t) {
				Object.DestroyImmediate (item);
			}
		}
		EditorUtility.UnloadUnusedAssetsImmediate ();
	}



	private void RefreshActiveEditor ()
	{
		var scenePrefabPath = AssetDatabase.GetAssetPath (Selection.activeObject);
		scenePrefab = ScenePrefabUtility.GetScenePrefab (scenePrefabPath);
		if (scenePrefab == null)
			scenePrefab = ScenePrefabUtility.CreateScenePrefab (scenePrefabPath);
		
		activeEditors.Clear ();

		foreach (var component in scenePrefab.GetComponents<Component> ()) {
			if (component is Transform || component is RectTransform)
				continue;
			activeEditors.Add (Editor.CreateEditor (component), true);
		}
	}

	public override void OnInspectorGUI ()
	{
		if (isSceneInspector == false)
			return;
		GUI.enabled = true;

		var editors = new List<Editor> (activeEditors.Keys);
		foreach (var editor in editors) {

			var rect = GUILayoutUtility.GetRect (GUIContent.none, GUIStyle.none, GUILayout.Height (20));
			rect.x = 0;
			rect.y -= 5;
			rect.width += 20;
			activeEditors [editor] = EditorGUI.InspectorTitlebar (rect, activeEditors [editor], new Object[]{ editor.target });
			GUILayout.Space (-5f);


			if (activeEditors [editor] && editor.target != null)
				editor.OnInspectorGUI ();
			
			EditorGUILayout.Space ();
			var lineRect = GUILayoutUtility.GetRect (GUIContent.none, GUIStyle.none, GUILayout.Height (2));
			lineRect.y -= 3;
			Handles.color = Color.black;
			Handles.DrawLine (new Vector2 (0, lineRect.y), new Vector2 (rect.width, lineRect.y));
		}

		if (editors.All (e => e.target != null) == false) {
			RefreshActiveEditor ();
			Repaint ();
		}
		Rect remainingRect = GUILayoutUtility.GetRect (GUIContent.none, GUIStyle.none, GUILayout.ExpandHeight (true),GUILayout.MinHeight(300));

		switch (Event.current.type) {
		case EventType.DragUpdated:
		case EventType.DragPerform:

			if (remainingRect.Contains (Event.current.mousePosition) == false)
				break;


			DragAndDrop.visualMode = DragAndDropVisualMode.Copy;

			if (Event.current.type == EventType.DragPerform) {
				DragAndDrop.AcceptDrag ();

				var components = DragAndDrop.objectReferences
					.Where (x => x.GetType () == typeof(MonoScript))
					.OfType<MonoScript> ()
					.Select (m => m.GetClass ());

				foreach (var component in components) {
					Undo.AddComponent (scenePrefab, component);
				}
				RefreshActiveEditor ();
			}
			break;
		}


		GUI.Label (remainingRect, "");
	}

}
