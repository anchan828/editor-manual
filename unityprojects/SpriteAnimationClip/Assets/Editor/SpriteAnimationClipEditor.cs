using System.Linq;
using UnityEngine;
using UnityEditor;

#region first
[CustomEditor(typeof(AnimationClip)), CanEditMultipleObjects]
public class SpriteAnimationClipEditor : OverrideEditor
{
    protected override Editor GetBaseEditor()
    {
        Editor editor = null;
        var baseType = Types.GetType("UnityEditor.AnimationClipEditor", "UnityEditor.dll");
        CreateCachedEditor(targets, baseType, ref editor);
        return editor;
    }
#endregion first
    private Sprite[] sprites = new Sprite[0];

    void OnEnable()
    {
        sprites = GetSprites(target as AnimationClip);
    }

    #region draw_preview
    public override bool HasPreviewGUI()
    {
        return true;
    }

    public override void OnInteractivePreviewGUI(Rect r, GUIStyle background)
    {
        if (sprites.Length != 0)
            EditorGUI.DrawTextureTransparent(r, AssetPreview.GetAssetPreview(sprites[0]));
        else
            baseEditor.OnInteractivePreviewGUI(r, background);
    }

    #endregion draw_preview

    #region getSprites
    private Sprite[] GetSprites(AnimationClip animationClip)
    {
        var sprites = new Sprite[0];

        if (animationClip != null)
        {
            var editorCurveBinding = EditorCurveBinding.PPtrCurve("", typeof(SpriteRenderer), "m_Sprite");

            var objectReferenceKeyframes = AnimationUtility.GetObjectReferenceCurve(animationClip, editorCurveBinding);

            var _sprites = objectReferenceKeyframes
                .Select(objectReferenceKeyframe => objectReferenceKeyframe.value)
                .OfType<Sprite>();

            foreach (var sprite in _sprites)
            {
                AssetPreview.GetAssetPreview(sprite);
            }
            sprites = _sprites.ToArray();
        }

        return sprites;
    }
    #endregion getSprites

    private bool isPlaying;
    public override void OnPreviewSettings()
    {
        if (GUILayout.Button("Play"))
        {
            isPlaying = true;
        }

        Debug.Log(isPlaying);
    }
}

class Sample : OverrideEditor
{
    private Sprite[] sprites = new Sprite[0];

    #region draw_sample_preview
    public override bool HasPreviewGUI()
    {
        return true;
    }

    public override void OnInteractivePreviewGUI(Rect r, GUIStyle background)
    {
        //スプライトがなければ通常（3D）のプレビュー画面にする
        if (sprites.Length != 0)
            EditorGUI.DrawTextureTransparent(r, AssetPreview.GetAssetPreview(sprites[0]));
        else
            baseEditor.OnInteractivePreviewGUI(r, background);
    }
    #endregion draw_sample_preview
    protected override Editor GetBaseEditor()
    {
        throw new System.NotImplementedException();
    }
}