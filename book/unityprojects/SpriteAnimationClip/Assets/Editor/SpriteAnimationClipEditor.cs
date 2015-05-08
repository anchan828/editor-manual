﻿using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEditor;


//#@@range_begin(first)
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
    //#@@range_end(first)

    Dictionary<Object, SpriteAnimationSettings> dic = new Dictionary<Object, SpriteAnimationSettings>();

    private TimeControl timeControl = new TimeControl();
    void OnEnable()
    {

        foreach (AnimationClip clip in targets)
        {
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

    public override void OnInteractivePreviewGUI(Rect r, GUIStyle background)
    {
        var settings = dic[target];


        if (settings.sprites.Length != 0)
        {
            var currentSpriteNum = Mathf.Clamp(Mathf.FloorToInt(timeControl.GetCurrentTime(settings.stopTime) * settings.frameRate), 0, settings.sprites.Length - 1);
            var sprite = settings.sprites[currentSpriteNum];
            var texture = AssetPreview.GetAssetPreview(sprite);

            if (texture != null)
                EditorGUI.DrawTextureTransparent(r, texture, ScaleMode.ScaleToFit);
        }
        else
            baseEditor.OnInteractivePreviewGUI(r, background);

    }

    //#@@range_end(draw_preview)
    //#@@range_begin(getSprites)
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
    //#@@range_end(getSprites)

    public override void OnPreviewSettings()
    {
        var playButtonContent = EditorGUIUtility.IconContent("PlayButton");
        var previewButtonSettingsStyle = new GUIStyle("preButton");

        EditorGUI.BeginChangeCheck();
        var isPlaying = GUILayout.Toggle(timeControl.isPlaying, playButtonContent, previewButtonSettingsStyle);
        if (EditorGUI.EndChangeCheck())
        {
            if (isPlaying) timeControl.Play(); else timeControl.Stop();
        }
        GUIStyle preSlider = "preSlider";
        GUIStyle preSliderThumb = "preSliderThumb";
        GUIStyle preLabel = "preLabel";
        GUIContent speedScale = EditorGUIUtility.IconContent("SpeedScale");

        GUILayout.Box(speedScale, preLabel);
        timeControl.speed = GUILayout.HorizontalSlider(timeControl.speed, 0, 10, preSlider, preSliderThumb);
        GUILayout.Label(timeControl.speed.ToString("0.00"), preLabel, GUILayout.Width(40));
        foreach (var activeEditor in ActiveEditorTracker.sharedTracker.activeEditors)
        {
            activeEditor.Repaint();
        }
    }

    struct SpriteAnimationSettings
    {
        public Sprite[] sprites { get; set; }
        public float stopTime { get; set; }
        public float frameRate { get; set; }
    }
}


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
            var deltaTime = (float)(timeSinceStartup - lastFrameEditorTime);
            lastFrameEditorTime = timeSinceStartup;
            currentTime += deltaTime * speed;
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

    public void Stop()
    {
        isPlaying = false;
    }
}

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
            EditorGUI.DrawTextureTransparent(r, AssetPreview.GetAssetPreview(sprites[0]));
        else
            baseEditor.OnInteractivePreviewGUI(r, background);
    }
    //#@@range_end(draw_sample_preview)

    //#@@range_begin(sample_OnPreviewSettings)
    public override void OnPreviewSettings()
    {
        var playButtonContent = EditorGUIUtility.IconContent("PlayButton");
        var previewButtonSettingsStyle = new GUIStyle("preButton");

        if (GUILayout.Button(playButtonContent, previewButtonSettingsStyle))
        {
        }
    }
    //#@@range_end(sample_OnPreviewSettings)

    protected override Editor GetBaseEditor()
    {
        throw new System.NotImplementedException();
    }
}