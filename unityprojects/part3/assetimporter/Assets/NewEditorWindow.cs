using UnityEngine;
using UnityEditor;

public class NewEditorWindow : EditorWindow
{
    [MenuItem("Window/NewEditorWindow")]
    static void Open ()
    {
        GetWindow<NewEditorWindow> ();
    }

    void OnGUI ()
    {


    }
}