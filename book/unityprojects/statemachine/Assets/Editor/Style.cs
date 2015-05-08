using UnityEngine;
using UnityEditor;

public class Style
{

    static GUIStyle[] m_backgrounds = new GUIStyle[2];
    public static GUIStyle backgorund
    {
        get
        {
            if (m_backgrounds[0] == null)
            {
                m_backgrounds[0] = new GUIStyle();
                m_backgrounds[1] = new GUIStyle();
                var light = new Texture2D(1, 1);
                light.SetPixel(1, 1, new Color32(87, 87, 87, 255));
                light.Apply();
                var dark = new Texture2D(1, 1);
                dark.SetPixel(1, 1, new Color32(48, 48, 48, 255));
                dark.Apply();
                m_backgrounds[0].normal.background = light;
                m_backgrounds[1].normal.background = dark;
            }
            return m_backgrounds[EditorGUIUtility.isProSkin ? 1 : 0];
        }
    }

    static GUIStyle m_state = null;

    public static GUIStyle state
    {
        get
        {
            if (m_state == null)
            {
                m_state = "window";

                //                var stateImage =
                //                    AssetDatabase.LoadAssetAtPath(
                //                        "Assets/Editor/state.png", typeof (Texture2D)) as Texture2D;
                //                m_state.normal.background = stateImage;
                //                m_state.onNormal.background = stateImage;
            }
            return m_state;
        }
    }
}