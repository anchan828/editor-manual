using UnityEngine;
using UnityEditor;
using UnityEditor.AnimatedValues;
using UnityEngine.Events;

public class NewBehaviourScript : EditorWindow
{
    [MenuItem("Window/Example")]
    static void Open ()
    {
        GetWindow<NewBehaviourScript> ();
    }

    void OnGUI ()
    {
        EditorGUILayout.ObjectField (null, typeof(Object), false);
       
        EditorGUILayout.Space ();
       
        EditorGUILayout.ObjectField (null, typeof(Texture), false, GUILayout.Width (128), GUILayout.Height (128));
    }
}
