using UnityEngine;
using UnityEditor;

public class EditorScript
{
	//GizmoType.InSelectionHierarchy

	[DrawGizmo (GizmoType.NonSelected | GizmoType.Active)]
	static void DrawExampleGizmos (Example example, GizmoType gizmoType)
	{
		var transform = example.transform;
		Gizmos.color = new Color32 (145, 244, 139, 210);

		if ((gizmoType & GizmoType.Active) == GizmoType.Active)
			Gizmos.color = Color.red;

		Gizmos.DrawWireCube (transform.position, transform.lossyScale);
	}
}
