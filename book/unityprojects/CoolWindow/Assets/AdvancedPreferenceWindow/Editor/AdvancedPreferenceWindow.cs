using UnityEngine;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine.Events;
using System.Linq;
public class AdvancedPreferenceWindow : EditorWindow
{

    [MenuItem("Window/AdvancedPreferenceWindow")]
    static void Open()
    {
        var window = CreateInstance<AdvancedPreferenceWindow>();
        window.minSize = new Vector2(600, 400);
        window.position = new Rect(300, 300, window.minSize.x, window.minSize.y);
        window.ShowUtility();
    }

    GUISkin skin;
    bool toggle;
    void OnEnable()
    {
        skin = AssetDatabase.LoadAssetAtPath("Assets/AdvancedPreferenceWindow/Editor/GUISkin.guiskin", typeof(GUISkin)) as GUISkin;

        var text = "";
        menuItems = new KeyValuePair<GUIContent, UnityAction>[]
        {
           new KeyValuePair<GUIContent, UnityAction>(GetGUIContent("General"), () =>
           {
                text = TextField("名前","ここに名前を入力してください", text);
                TextField("API Key","ここに API Key を入力してください", "");

                toggle =   GUILayout.Toggle(toggle,"チェックマーク１");
                toggle =   !GUILayout.Toggle(!toggle,"チェックマーク２");


                GUILayout.Box("Box");

                HelpBox("Message", MessageType.Info);
                 HelpBox("Message", MessageType.Warning);
                  HelpBox("Message", MessageType.Error);


        EditorGUILayout.Space();

        using (new EditorGUILayout.HorizontalScope())
        {
            GUILayout.Space(position.width - 160 - 150);
            GUILayout.Button("button", GUILayout.Width(90));
        }

    }),
            new KeyValuePair<GUIContent, UnityAction>(GetGUIContent("Settings"), () =>
            {

           }),
            new KeyValuePair<GUIContent, UnityAction>(GetGUIContent("Info"), () =>
            {

           }),
        };

    }

    void HelpBox(string message, MessageType type)
    {
        GUIStyle style = "box";
        var _color = GUI.backgroundColor;
        switch (type)
        {
            case MessageType.Info:
                GUI.backgroundColor = Color.cyan;
                break;
            case MessageType.Warning:
                GUI.backgroundColor = Color.yellow;
                break;
            case MessageType.Error:
                GUI.backgroundColor = Color.red;
                break;
        }

        EditorGUILayout.HelpBox(message, type);


        GUI.backgroundColor = _color;
    }

    string TextField(string label, string placeholder, string text)
    {
        using (new EditorGUILayout.HorizontalScope(GUILayout.Width(position.width - 160 - 50)))
        {
            GUILayout.Label(label, GUI.skin.GetStyle("textField_label"));

            var _text = GUILayout.TextField(text);

            if (string.IsNullOrEmpty(_text))
            {
                var placeholderRect = GUILayoutUtility.GetLastRect();
                GUI.Label(placeholderRect, placeholder, GUI.skin.GetStyle("textField_placeholder"));
            }

            return _text;
        }
    }

    int selected = 0;

    KeyValuePair<GUIContent, UnityAction>[] menuItems;
    void OnGUI()
    {
        if (Event.current.keyCode == KeyCode.Escape)
        {
            Close();
        }

        var _skin = GUI.skin;
        GUI.skin = skin;
        using (new EditorGUILayout.HorizontalScope(skin.GetStyle("main_background"), GUILayout.Width(position.width), GUILayout.Height(position.height)))
        {
            using (new EditorGUILayout.VerticalScope(skin.GetStyle("menuItem_background"), GUILayout.Width(160), GUILayout.Height(position.height)))
            {
                selected = GUILayout.SelectionGrid(selected, menuItems.Select(kv => kv.Key).ToArray(), 1, skin.GetStyle("menuItem"), GUILayout.Height(80 * menuItems.Length));
            }

            using (new EditorGUILayout.VerticalScope())
            {
                var menuItem = menuItems[selected];
                GUILayout.Label(menuItem.Key.text, skin.GetStyle("header"));
                menuItem.Value();
            }
        }
        GUI.skin = _skin;
    }

    private GUIContent GetGUIContent(string name)
    {
        return new GUIContent(name, (Texture2D)AssetDatabase.LoadAssetAtPath("Assets/AdvancedPreferenceWindow/Editor/Textures/" + name.ToLower() + ".tiff", typeof(Texture2D)));
    }
}
