using System.Linq;
using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(AnimationClip))]
public class SpriteAnimationClipEditor : OverrideEditor
{
    protected override Editor GetBaseEditor()
    {
        Editor editor = null;
        var baseType = Types.GetType("UnityEditor.AnimationClipEditor", "UnityEditor.dll");
        CreateCachedEditor(targets, baseType, ref editor);
        return editor;
    }

    #region getSprites
    private Sprite[] GetSprites(AnimationClip animationClip)
    {
        var sprites = new Sprite[0];

        if (animationClip != null)
        {
            var editorCurveBinding = EditorCurveBinding.PPtrCurve("", typeof(SpriteRenderer), "m_Sprite");

            var objectReferenceKeyframes = AnimationUtility.GetObjectReferenceCurve(animationClip, editorCurveBinding);

            sprites = objectReferenceKeyframes
               .Select(objectReferenceKeyframe => objectReferenceKeyframe.value)
               .OfType<Sprite>()
               .ToArray();
        }

        return sprites;
    }
    #endregion getSprites
}
