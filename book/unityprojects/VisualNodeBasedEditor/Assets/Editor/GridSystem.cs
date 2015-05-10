using UnityEngine;
using System.Collections;
using UnityEditor;

public class GridSystem
{

    private EditorWindow window;
    private float gridSize;

    public GridSystem(EditorWindow window, float gridSize = 12)
    {
        this.window = window;
        this.gridSize = gridSize;
    }


    public void DrawGrid()
    {
        DrawBackground();
        DrawGridLines(gridSize, EditorGUIUtility.isProSkin ?
            new Color32(32, 32, 32, 255) : new Color32(60, 60, 60, 255));
        DrawGridLines(gridSize * 10, Color.black);
    }

    private void DrawBackground()
    {

        if (Event.current.type == EventType.Repaint)
        {
            Style.background.Draw(new Rect(0, 0, window.position.width, window.position.height),
                                    false, false, false, false);
        }
    }

    private void DrawGridLines(float gridSize, Color gridColor)
    {
        float xMin = 0;
        float yMin = 0;
        float xMax = window.position.width;
        float yMax = window.position.height;

        Handles.color = gridColor;

        float x = xMin - xMin % gridSize;

        while (x < xMax)
        {
            Handles.DrawLine(new Vector2(x, yMin), new Vector2(x, yMax));
            x += gridSize;
        }

        float y = yMin - yMin % gridSize;

        while (y < yMax)
        {
            Handles.DrawLine(new Vector3(xMin, y), new Vector3(xMax, y));
            y += gridSize;
        }
    }

    class Style
    {
        public static GUIStyle background;
        static Style()
        {
            var bg = AssetDatabase.LoadAssetAtPath<Texture2D>("Assets/Editor/grid_bg.tiff");
            background = new GUIStyle
            {
                border = new RectOffset(12, 12, 12, 12),
                normal = { background = bg }
            };

//            background = "flow background";
//            Selection.activeObject = EditorGUIUtility.GetBuiltinSkin(EditorSkin.Inspector);
        }
    }

}

