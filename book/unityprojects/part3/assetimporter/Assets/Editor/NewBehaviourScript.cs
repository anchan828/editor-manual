using UnityEngine;
using UnityEditor;

public class NewBehaviourScript : AssetPostprocessor
{
    void OnPreprocessTexture ()
    {
        Debug.Log ("OnPreprocessTexture");
    }

    void OnPostprocessTexture (Texture2D texture)
    {
        Debug.Log ("OnPostprocessTexture");
    }
    
    
    void OnPreprocessAudio ()
    {
        Debug.Log ("OnPreprocessAudio");
    }


    void OnPostprocessAudio (AudioClip clip)
    {
        Debug.Log ("OnPostprocessAudio");
    }
    
    void OnPreprocessModel ()
    {
        Debug.Log ("OnPreprocessModel");
    }

    void OnPostprocessModel (GameObject root)
    {
        Debug.Log ("OnPostprocessModel");
    }

    void OnPostprocessGameObjectWithUserProperties (GameObject root, string[] propNames, object[] values)
    {
        Debug.Log ("OnPostprocessGameObjectWithUserProperties");
    }

    static void OnGeneratedCSProjectFiles ()
    {
        Debug.Log ("OnGeneratedCSProjectFiles");
    }

    static void OnPostprocessAllAssets (string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromPath)
    {
        Debug.Log ("OnPostprocessAllAssets");
    }

    Material OnAssignMaterialModel (Material material, Renderer renderer)
    {
        Debug.Log ("OnAssignMaterialModel");
        return material;
    }
}
