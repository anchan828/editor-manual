using System.Xml;
using UnityEditor;
using UnityEngine;
using System.Collections.Generic;

namespace EditorManual.StateMachine
{
    [System.Serializable]
    public class Transition
    {
        public int fromStateID, toStateID;

        public string stateMachineID;
        public Transition(string stateMachineID, int fromStateID)
            : this(stateMachineID, fromStateID, 0)
        {

        }


        public Transition(string stateMachineID, int fromStateID, int toStateID)
        {
            this.stateMachineID = stateMachineID;
            this.fromStateID = fromStateID;
            this.toStateID = toStateID;
        }
    }
}