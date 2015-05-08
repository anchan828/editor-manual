using System;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using System.Collections;

namespace EditorManual.StateMachine
{
    [System.Serializable]
    public class TransitionGUI
    {
        public static TransitionGUI making;
        public int fromStateID, toStateID;
        public string stateMachineID;

        private StateMachine m_stateMachine;

        private StateMachine stateMachine
        {
            get
            {
                if (m_stateMachine == null)
                {
                    m_stateMachine = StateMachineUtil.GetStateMachine(stateMachineID);
                }
                return m_stateMachine;
            }
        }
        [NonSerialized]
        public State m_from, m_to;

        private State from
        {
            get
            {
                if (m_from == null)
                {
                    m_from = stateMachine.GetState(fromStateID);
                }
                return m_from;
            }
        }


        private State to
        {
            get
            {
                if (m_to == null && toStateID != 0)
                {
                    m_to = stateMachine.GetState(toStateID);
                }
                return m_to;
            }
        }

        public TransitionGUI(string stateMachineID, int fromStateID)
            : this(stateMachineID, fromStateID, 0)
        {

        }
        public TransitionGUI(string stateMachineID, int fromStateID, int toStateID)
        {
            this.stateMachineID = stateMachineID;
            this.fromStateID = fromStateID;
            this.toStateID = toStateID;
        }

        public void OnGUI()
        {
            if (from == null || to == null)
            {
                return;
            }

            OnGUI(from.windowRect, to.windowRect);
        }


        public void OnGUI(Vector2 pos)
        {
            if (from == null)
            {
                return;
            }

            OnGUI(from.windowRect, new Rect(pos.x, pos.y, 0, 0));
        }

        private void OnGUI(Rect from, Rect to)
        {
            var e = Event.current;

            DrawNodeCurve(from, to);

            if (making != null && e.button == 0 && e.clickCount == 1)
            {
                var state = stateMachine.GetState(e.mousePosition);

                if (state != null)
                {
                    var transitionGUI = this.@from.GetTransitionGUI(state.uniqueID);

                    if (transitionGUI == null)
                    {
                        transitionGUI = new TransitionGUI(stateMachineID, this.from.uniqueID, state.uniqueID);
                        this.@from.AddTransitionGUI(transitionGUI);
                    }
                    this.from.AddOutTrasition(state.uniqueID);
                    state.AddInTrasition(fromStateID);
                }

                making = null;
            }
        }

        //ステート同士をつなげる前は end はマウスの位置になる
        private void DrawNodeCurve(Rect start, Rect end)
        {
            Handles.color = Color.white;
            var startPos = new Vector3(start.x + start.width * 0.5f, start.y + start.height * 0.5f, 0);
            var endPos = new Vector3(end.x + end.width * 0.5f, end.y + end.height * 0.5f, 0);
            //直線の描画
            Handles.DrawAAPolyLine(EditorGUIUtility.whiteTexture, 2f, new[] { startPos, endPos });
            var cross = Vector3.Cross((startPos - endPos).normalized, Vector3.forward);
            var vector = endPos - startPos;
            var direction = vector.normalized;
            var center = vector * 0.5f + startPos;
            center -= cross * 0.5f;
            center += 13 * direction;
            var arraws = new List<Vector3>();
            for (var i = 0; i < 15; i++)
            {
                Vector3[] arraowPos =
                {
                    center + direction * i,(center - direction * i) + cross * i,
                    (center - direction * i) - cross * i,
                    center + direction * i
                };
                arraws.AddRange(arraowPos);
            }
            //矢印の描画、異なる大きさの三角形をいくつも描画することで矢印を実現させている
            Handles.DrawAAPolyLine(EditorGUIUtility.whiteTexture, 2f, arraws.ToArray());
            EditorWindow.focusedWindow.Repaint();
        }
    }
}