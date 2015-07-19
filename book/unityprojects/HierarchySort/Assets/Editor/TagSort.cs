using UnityEngine;
using UnityEditor;

public class TagSort : BaseHierarchySort
{
	private SerializedObject hierarcyWindowObject;

	public override int Compare (GameObject lhs, GameObject rhs)
	{
		if (lhs == rhs)
			return 0;
		if (lhs == null)
			return -1;
		if (rhs == null)
			return 1;
		return EditorUtility.NaturalCompare (lhs.tag, rhs.tag);
	}
	private GUIContent m_content = EditorGUIUtility.IconContent ("TagSort");

	public override GUIContent content {
		get {

			if (hierarcyWindowObject == null) {
				var hierarcyType = Types.GetType ("UnityEditor.SceneHierarchyWindow", "UnityEditor.dll");
				var hierarcyWindows = Resources.FindObjectsOfTypeAll (hierarcyType);
				if (hierarcyWindows.Length != 0) {
					hierarcyWindowObject = new SerializedObject (hierarcyWindows[0]);
					EditorApplication.hierarchyWindowItemOnGUI += HierarchyWindowItemOnGUI;
				}
			}

			return m_content;
		}
	}

	private void HierarchyWindowItemOnGUI (int instanceID, Rect selectionRect)
	{
		hierarcyWindowObject.Update ();

		var currentSortName =
			hierarcyWindowObject.FindProperty ("m_CurrentSortMethod").stringValue;

		if (currentSortName != "TagSort") {
			EditorApplication.hierarchyWindowItemOnGUI -= HierarchyWindowItemOnGUI;
			hierarcyWindowObject = null;
			return;
		}

		var go = EditorUtility.InstanceIDToObject (instanceID) as GameObject;

		selectionRect.x += selectionRect.width - 64;
		selectionRect.width = 64;

		EditorGUI.LabelField (selectionRect, go.tag, Style.miniBox);
	}

	private class Style
	{
		public static GUIStyle miniBox;

		static Style ()
		{
			miniBox = new GUIStyle ("box");
			miniBox.fontSize = 8;
		}
	}
}