using UnityEngine;
using System.Collections;
using UnityEditor;

public class NewBehaviourScript
{

    [DrawGizmo(GizmoType.NonSelected | GizmoType.Active)]
    static void DrawGizmoForMyScript(MyScript myScript, GizmoType gizmoType)
    {
        var collider = myScript.GetComponent<BoxCollider>();
        DrawGizmoCube(Color.blue, collider.transform, Vector3.forward, collider.size);
        DrawGizmoCube(Color.red, collider.transform, Vector3.right, collider.size);
        DrawGizmoSphere(Color.green, collider.transform, collider.size);
    }
    /*
     * #@@range_begin(DrawGizmoForBoxCollider)
    [DrawGizmo(GizmoType.NonSelected | GizmoType.Active)]
    static void DrawGizmoForBoxCollider(BoxCollider collider, GizmoType gizmoType)
    {
        DrawGizmoCube(Color.blue, collider.transform, Vector3.forward, collider.size);
        DrawGizmoCube(Color.red, collider.transform, Vector3.right, collider.size);
        DrawGizmoSphere(Color.green, collider.transform, collider.size);
    }
     * #@@range_end(DrawGizmoForBoxCollider)
    */

    static void DrawGizmoCube(Color color, Transform transform, Vector3 directioin, Vector3 colliderSize)
    {
        Gizmos.color = color;
        SetMatrix(transform, colliderSize);

        var size = new Vector3(colliderSize.x * Mathf.Max(0.1f, directioin.x * 0.5f), colliderSize.y * 0.05f, colliderSize.z * Mathf.Max(0.1f, directioin.z * 0.5f));
        Gizmos.DrawCube(directioin * 0.25f, size);
    }

    static void DrawGizmoSphere(Color color, Transform transform, Vector3 colliderSize)
    {
        Gizmos.color = color;
        SetMatrix(transform, colliderSize);

        Gizmos.DrawSphere(Vector3.zero, colliderSize.x * 0.3f);
    }

    static void SetMatrix(Transform transform, Vector3 colliderSize)
    {
        var pos = transform.localPosition + transform.up * colliderSize.y * 0.5f * transform.lossyScale.y;
        Gizmos.matrix = Matrix4x4.TRS(pos, transform.localRotation, transform.lossyScale);
    }
}
