using UnityEngine;
using System.Collections;
using UnityEditor;

public class VisualNodeBasedEditorWindow : EditorWindow
{

    [MenuItem("Window/VisualNodeBasedEditor")]
    static void Open()
    {
        GetWindow<VisualNodeBasedEditorWindow>();
    }

    private GridSystem gridSystem;
    void OnEnable()
    {
        gridSystem = new GridSystem(this);
    }

    void OnGUI()
    {
        gridSystem.DrawGrid();
    }
}
