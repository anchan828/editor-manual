using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEditor;
using Object = UnityEngine.Object;


//#@@range_begin(first)
[CustomEditor(typeof(AnimationClip)), CanEditMultipleObjects]
public class SpriteAnimationClipEditor : OverrideEditor
{
    protected override Editor GetBaseEditor()
    {
        Editor editor = null;
        var baseType = 
            Types.GetType("UnityEditor.AnimationClipEditor", "UnityEditor.dll");
        CreateCachedEditor(targets, baseType, ref editor);
        return editor;
    }
    //#@@range_end(first)

    Dictionary<Object, SpriteAnimationSettings> dic = new Dictionary<Object, SpriteAnimationSettings>();

    private TimeControl timeControl = new TimeControl();
    void OnEnable()
    {

        foreach (AnimationClip clip in targets)
        {
            var sprites = GetSprites(clip);

            if (sprites.Length == 0) continue;

            dic.Add(clip, new SpriteAnimationSettings
            {
                sprites = GetSprites(clip),
                stopTime = GetStopTime(clip),
                frameRate = clip.frameRate
            });

        }
    }

    private float GetStopTime(AnimationClip animationClip)
    {
        var animationClipSettings = AnimationUtility.GetAnimationClipSettings(animationClip);
        return animationClipSettings.stopTime;
    }



    //#@@range_begin(draw_preview)
    public override bool HasPreviewGUI()
    {
        return true;
    }

    //#@@range_begin(OnInteractivePreviewGUI)
    public override void OnInteractivePreviewGUI(Rect r, GUIStyle background)
    {
        SpriteAnimationSettings settings;

        if (dic.TryGetValue(target, out settings))
        {
            //#@@range_begin(currentSpriteNum)
            var currentSpriteNum = 
                Mathf.FloorToInt(timeControl.GetCurrentTime(settings.stopTime) 
                                                                  * settings.frameRate);
            //#@@range_end(currentSpriteNum)
            var sprite = settings.sprites[currentSpriteNum];
            var texture = AssetPreview.GetAssetPreview(sprite);

            if (texture != null)
                EditorGUI.DrawTextureTransparent(r, texture, ScaleMode.ScaleToFit);
        }
        else
            baseEditor.OnInteractivePreviewGUI(r, background);
    }
    //#@@range_end(OnInteractivePreviewGUI)
    //#@@range_end(draw_preview)
    //#@@range_begin(getSprites)
    private Sprite[] GetSprites(AnimationClip animationClip)
    {
        var sprites = new Sprite[0];

        if (animationClip != null)
        {
            var editorCurveBinding = 
                EditorCurveBinding.PPtrCurve("", typeof(SpriteRenderer), "m_Sprite");

            var objectReferenceKeyframes = 
                AnimationUtility.GetObjectReferenceCurve(animationClip, 
                                                         editorCurveBinding);

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
    //#@@range_end(getSprites)

   
    public override void OnPreviewSettings()
    {
        SpriteAnimationSettings settings;

        if (dic.TryGetValue(target, out settings))
        {
           DrawPlayButton();
           

            DrawSpeedSlider();


            if (timeControl.isPlaying)
            {
                foreach (var activeEditor in ActiveEditorTracker.sharedTracker.activeEditors)
                {
                    activeEditor.Repaint();
                }
            }
        }
        else
            baseEditor.OnPreviewSettings();
    }

    //#@@range_begin(DrawPlayButton)
    private void DrawPlayButton()
    {
        var playButtonContent = EditorGUIUtility.IconContent("PlayButton");
        var pauseButtonContent = EditorGUIUtility.IconContent("PauseButton");
        var previewButtonSettingsStyle = new GUIStyle("preButton");
        var buttonContent = timeControl.isPlaying ? pauseButtonContent : playButtonContent;

        EditorGUI.BeginChangeCheck();

        var isPlaying = 
            GUILayout.Toggle(timeControl.isPlaying, 
                                buttonContent, previewButtonSettingsStyle);

        if (EditorGUI.EndChangeCheck())
        {
            if (isPlaying) timeControl.Play();
            else timeControl.Pause();
        }
    }
    //#@@range_end(DrawPlayButton)
    //#@@range_begin(DrawSpeedSlider)
    private void DrawSpeedSlider()
    {
        var preSlider = new GUIStyle("preSlider");
        var preSliderThumb = new GUIStyle("preSliderThumb");
        var preLabel = new GUIStyle("preLabel");
        var speedScale = EditorGUIUtility.IconContent("SpeedScale");

        GUILayout.Box(speedScale, preLabel);
        timeControl.speed = 
            GUILayout.HorizontalSlider(timeControl.speed, 0, 10, preSlider, preSliderThumb);
        GUILayout.Label(timeControl.speed.ToString("0.00"), preLabel, GUILayout.Width(40));
    }
    //#@@range_end(DrawSpeedSlider)
    struct SpriteAnimationSettings
    {
        public Sprite[] sprites { get; set; }
        public float stopTime { get; set; }
        public float frameRate { get; set; }
    }
}

//#@@range_begin(TimeControl)
public class TimeControl
{
    public bool isPlaying { get; private set; }
    private float currentTime { get; set; }
    private double lastFrameEditorTime { get; set; }
    public float speed { get; set; }

    public TimeControl()
    {
        speed = 1;
        EditorApplication.update += Update;
    }

    public void Update()
    {
        if (isPlaying)
        {
            var timeSinceStartup = EditorApplication.timeSinceStartup;
            var deltaTime = timeSinceStartup - lastFrameEditorTime;
            lastFrameEditorTime = timeSinceStartup;
            currentTime += (float)deltaTime * speed;
        }
    }

    public float GetCurrentTime(float stopTime)
    {
        return Mathf.Repeat(currentTime, stopTime);
    }

    public void Play()
    {
        isPlaying = true;
        lastFrameEditorTime = EditorApplication.timeSinceStartup;
    }

    public void Pause()
    {
        isPlaying = false;
    }
}
//#@@range_end(TimeControl)
class Sample : OverrideEditor
{
    private Sprite[] sprites = new Sprite[0];

    //#@@range_begin(draw_sample_preview)
    public override bool HasPreviewGUI()
    {
        return true;
    }

    public override void OnInteractivePreviewGUI(Rect r, GUIStyle background)
    {
        //スプライトがなければ通常（3D）のプレビュー画面にする
        if (sprites.Length != 0)
        {
            var texture = AssetPreview.GetAssetPreview(sprites[0]);
            EditorGUI.DrawTextureTransparent(r, texture, ScaleMode.ScaleToFit);
        }
        else
            baseEditor.OnInteractivePreviewGUI(r, background);
    }
    //#@@range_end(draw_sample_preview)

    //#@@range_begin(sample_OnPreviewSettings)
    private bool isPlaying = false;
    public override void OnPreviewSettings()
    {
        var playButtonContent = EditorGUIUtility.IconContent("PlayButton");
        var pauseButtonContent = EditorGUIUtility.IconContent("PauseButton");
        var previewButtonSettingsStyle = new GUIStyle("preButton");
        var buttonContent = isPlaying ? pauseButtonContent : playButtonContent;
        isPlaying = GUILayout.Toggle(isPlaying, buttonContent, previewButtonSettingsStyle);
    }
    //#@@range_end(sample_OnPreviewSettings)

    protected override Editor GetBaseEditor()
    {
        throw new System.NotImplementedException();
    }
}