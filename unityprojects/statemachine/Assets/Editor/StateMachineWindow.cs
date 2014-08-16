using UnityEditor;
using UnityEngine;

namespace EditorManual.StateMachine
{
    public class StateMachineWindow : EditorWindow
    {
        StateMachine stateMachine;

        [MenuItem("Window/StateMachine")]
        private static void Open()
        {
            GetWindow<StateMachineWindow>();
        }
        void OnEnable()
        {
            stateMachine = StateMachineUtil.Load();
        }

        private void OnGUI()
        {
            if (Event.current.type == EventType.Repaint)
            {
                Style.backgorund.Draw(new Rect(0, 0, position.width, position.height),
                false, false, false, false);
            }
            DrawGrid(12);
            if (stateMachine)
            {
                BeginWindows();
                stateMachine.OnGUI();
                EndWindows();
            }
            Toolbar.OnGUI(stateMachine);
        }

        void OnSelectionChange()
        {
           var _stateMachine = StateMachineUtil.Load();

            if (_stateMachine)
            {
                stateMachine = _stateMachine;
                Repaint();
            }
        }
        private void DrawGrid(float gridSize)
        {
            DrawGridLines(gridSize,
                EditorGUIUtility.isProSkin ? new Color32(32, 32, 32, 255) : new Color32(60, 60, 60, 255));
            DrawGridLines(gridSize * 10, Color.black);
        }

        private void DrawGridLines(float gridSize, Color gridColor)
        {
            var xMin = 0f;
            var yMin = 0f;
            var xMax = position.width;
            var yMax = position.height;
            Handles.color = gridColor;
            var x = xMin - xMin % gridSize;
            while (x < xMax)
            {
                Handles.DrawLine(new Vector2(x, yMin), new Vector2(x, yMax));
                x += gridSize;
            }
            var y = yMin - yMin % gridSize;
            while (y < yMax)
            {
                Handles.DrawLine(new Vector3(xMin, y), new Vector3(xMax, y));
                y += gridSize;
            }
        }
    }
}