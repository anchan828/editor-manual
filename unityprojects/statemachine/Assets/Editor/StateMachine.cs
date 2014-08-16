using System;
using System.IO;
using System.Linq;
using UnityEditor;
using UnityEngine;
using System.Collections.Generic;
using UnityEngine.Events;
using Random = UnityEngine.Random;

namespace EditorManual.StateMachine
{
    public class StateMachine : ScriptableObject
    {
        public List<State> states = new List<State>();
//        public List<TransitionGUI> transitionGUIs = new List<TransitionGUI>();

        private string stateMachineID
        {
            get { return AssetDatabase.AssetPathToGUID(AssetDatabase.GetAssetPath(this)); }
        }

        [MenuItem("Assets/Create/StateMachine")]
        static void Create()
        {
            var stateMachine = CreateInstance<StateMachine>();
            ProjectWindowUtil.CreateAsset(stateMachine, "New StateMachine.asset");
        }
        public void AddState(string name)
        {
            states.Add(new State(stateMachineID, name));
        }

        public void AddState(string name, Vector2 pos)
        {
            states.Add(new State(stateMachineID, name, pos));
        }

        public void AddState(State state)
        {
            states.Add(state);
        }

//        public void AddTransitionGUI(TransitionGUI transitionGUI)
//        {
//            transitionGUIs.Add(transitionGUI);
//        }
//        public TransitionGUI GetTransitionGUI(State from, State to)
//        {
//            TransitionGUI transitionGUI = null;
//            if (transitionGUIs.Count(t => t.fromStateID == @from.uniqueID && t.toStateID == to.uniqueID) != 0)
//            {
//                transitionGUI = transitionGUIs.First(t => t.fromStateID == @from.uniqueID && t.toStateID == to.uniqueID);
//            }
//            return transitionGUI;
//        }

        public State GetState(int stateID)
        {
            Debug.Log(stateID);
            return states.First(s => s.uniqueID == stateID);
        }

        public void OnGUI()
        {
            var e = Event.current;

            if (rightClicked)
            {
                var state = GetState(e.mousePosition);
                if (state == null)
                {
                    ShowStateMachineMenu(type =>
                    {
                        switch (type)
                        {
                            case StateMachineEventType.NewState:
                                AddState("New State", e.mousePosition);
                                break;
                            case StateMachineEventType.Paste:
                                AddState(State.copyState.Duplicate());
                                break;
                            default:
                                throw new ArgumentOutOfRangeException("type");
                        }
                    });
                }
                else
                {
                    state.ShowStateMenu(null, (type, s) =>
                    {
                        switch (type)
                        {
                            case StateEventType.MakeTransition:
                                TransitionGUI.making = new TransitionGUI(stateMachineID, s.uniqueID);
                                break;
                            case StateEventType.Copy:
                                State.copyState = s;
                                break;
                            case StateEventType.Delete:
                                s.UnLink();
                                states.Remove(s);
                                break;
                            case StateEventType.Duplicate:
                                AddState(s.Duplicate());



                                break;
                            default:
                                throw new ArgumentOutOfRangeException("type");
                        }
                    });
                }
            }

            if (TransitionGUI.making != null)
            {
                TransitionGUI.making.OnGUI(e.mousePosition);
            }

            foreach (var state in states)
            {
                state.OnGUI();
            }
        }


        private bool Contains(Vector2 pos)
        {
            return states.Count(s => s.windowRect.Contains(pos)) != 0;
        }

        public State GetState(Vector2 pos)
        {
            State state = null;

            if (Contains(pos))
            {
                state = states.First(s => s.windowRect.Contains(pos));
            }

            return state;
        }

        private bool rightClicked
        {
            get
            {
                var e = Event.current;
                return e.button == 1 && e.clickCount == 1;
            }
        }

        public void ShowStateMachineMenu(UnityAction<StateMachineEventType> action)
        {
            var menu = new GenericMenu();
            menu.AddItem(new GUIContent("New State"), false, () => action(StateMachineEventType.NewState));
            if (State.copyState == null)
            {
                menu.AddDisabledItem(new GUIContent("Paste"));
            }
            else
            {
                menu.AddItem(new GUIContent("Paste"), false, () => action(StateMachineEventType.Paste));
            }
            menu.ShowAsContext();
        }


    }
}