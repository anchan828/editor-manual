= AssetImporter

//indepimage[frontispiece]


//lead{
アセットのインポート設定をエディタ拡張で制御することはとても簡単にできます。これからはテクスチャのスプライト設定やオーディオのストリーミング指定への変更を「インポート時に自動で」行うようにしましょう。
//}

//pagebreak

== AssetPostprocessor

例えばテクスチャをUnity上でスプライトとして使いたいとします。Unity上でスプライトとして使用するには

 1. テクスチャをインポートする
 2. Texture TypeをSpriteへと変更する
 3. Packing Tagを付ける
 4. Pixele To Unitを変更する
 5. Sprite ModeをMultipleにする
 6. Sprite Editorでスライスする

といったような手順の処理を行わなければいけません。

これが数枚のテクスチャであればマウスでポチポチポチと苦労せずテクスチャをスプライトに変更できます。ですがこれが何十枚ものテクスチャを同時にインポートした時はどうでしょうか。私ならやってられません。また、ほとんどは設定パラメータの指定があり、それらを統一させるのも人の手では一苦労です。

この章では先ほど書いた1-6までのうち、1-5までを自動化してみましょう。6については他の章で扱います。

== AssetModificationProcessor




//emlist[][[Sharp\]C]{
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

//}