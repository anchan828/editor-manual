using System;
using System.Collections.Generic;
using System.Linq;
using UnityEditor;
using UnityEngine;
using UnityEngine.Events;
using Random = UnityEngine.Random;

namespace EditorManual.StateMachine
{
    [System.Serializable]
    public class State
    {
        public static State copyState;
        public string name;
        public Rect windowRect;
        public int uniqueID;
        public string stateMachineID;

        public List<Transition> outTransitions = new List<Transition>();

        public List<Transition> inTransitions = new List<Transition>();

        private List<TransitionGUI> transitionGUIs;


        public State(string stateMachineID, string name)
            : this(stateMachineID, name, new Vector2(200, 250))
        {

        }


        public State(string stateMachineID, string name, Vector2 pos)
        {
            this.name = name;
            this.windowRect = new Rect(pos.x - 100, pos.y - 50, 200, 100);
            this.uniqueID = Random.Range(int.MinValue, int.MaxValue);
            this.stateMachineID = stateMachineID;
        }

        private void ResetTransitionGUIs()
        {
            transitionGUIs = new List<TransitionGUI>();

            foreach (var outTransition in outTransitions)
            {
                if (
                    transitionGUIs.Count(
                        t => t.fromStateID == outTransition.fromStateID && t.toStateID == outTransition.toStateID) == 0)
                {
                    var transitionGUI = new TransitionGUI(stateMachineID, outTransition.fromStateID, outTransition.toStateID);
                    transitionGUIs.Add(transitionGUI);
                }
            }
        }

        public void OnGUI()
        {
            if (transitionGUIs == null)
            {
                ResetTransitionGUIs();
            }

            foreach (var transitionGui in transitionGUIs)
            {
                transitionGui.OnGUI();
            }

            windowRect = GUI.Window(uniqueID, windowRect, (id) =>
            {
                EditorGUILayout.LabelField(":)");
                GUI.DragWindow();
            }, name, Style.state);
        }

        public void UnLink()
        {
            var stateMachine = StateMachineUtil.GetStateMachine(stateMachineID);

            var outStateIDs = outTransitions.Select(t => t.toStateID).ToArray();
            foreach (var _state in outStateIDs.Select(outStateID => stateMachine.GetState(outStateID)))
            {
                _state.inTransitions.RemoveAll(t => t.fromStateID == uniqueID);
                _state.ResetTransitionGUIs();
            }

            var inStateIDs = inTransitions.Select(t => t.fromStateID).ToArray();

            foreach (var inStateID in inStateIDs)
            {
                var _state = stateMachine.GetState(inStateID);
                _state.outTransitions.RemoveAll(t => t.fromStateID == inStateID);
                _state.ResetTransitionGUIs();
            }
        }
        public void AddTransitionGUI(TransitionGUI transitionGUI)
        {
            transitionGUIs.Add(transitionGUI);
        }

        public void AddOutTrasition(int toStateID)
        {
            outTransitions.Add(new Transition(stateMachineID, uniqueID, toStateID));
            ResetTransitionGUIs();
        }

        public void AddInTrasition(int fromStateID)
        {
            inTransitions.Add(new Transition(stateMachineID, fromStateID, uniqueID));
        }

        public TransitionGUI GetTransitionGUI(int toStateID)
        {
            TransitionGUI transitionGUI = null;
            foreach (var outTransition in outTransitions.Where(outTransition => transitionGUIs.Count(
                t => t.fromStateID == outTransition.fromStateID && t.toStateID == toStateID) != 0))
            {
                transitionGUI = transitionGUIs.First(
                    t => t.fromStateID == outTransition.fromStateID && t.toStateID == toStateID);
            }
            return transitionGUI;
        }

        public void ShowStateMenu(GenericMenu menu, UnityAction<StateEventType, State> action)
        {
            if (menu == null)
            {
                menu = new GenericMenu();
            }

            menu.AddItem(new GUIContent("Make Transition"), false, () => action(StateEventType.MakeTransition, this));
            menu.AddItem(new GUIContent("Copy"), false, () => action(StateEventType.Copy, this));
            menu.AddItem(new GUIContent("Duplicate"), false, () => action(StateEventType.Duplicate, this));
            menu.AddItem(new GUIContent("Delete"), false, () => action(StateEventType.Delete, this));
            menu.ShowAsContext();
        }


        public State Duplicate()
        {
            var state = new State(stateMachineID, name) { windowRect = windowRect };
            var transitions = new List<Transition>();
            transitions.AddRange(
                outTransitions.Select(outTransition
                    => new Transition(stateMachineID, state.uniqueID, outTransition.toStateID)
                    ));

            state.outTransitions = transitions;

            transitions = new List<Transition>();
            transitions.AddRange(
                inTransitions.Select(inTransition
                    =>
                {
                    var stateMachine = StateMachineUtil.GetStateMachine(stateMachineID);

                    var _state = stateMachine.GetState(inTransition.fromStateID);
                    _state.AddOutTrasition(state.uniqueID);
                    return new Transition(stateMachineID, inTransition.fromStateID, state.uniqueID);
                }
                    ));


            state.inTransitions = transitions;
            state.ResetTransitionGUIs();
            return state;
        }
    }
}