using System.Linq;
using UnityEditor;
namespace EditorManual.StateMachine
{
    public class StateMachineUtil
    {
        public static StateMachine GetStateMachine(string stateMachineID)
        {
            StateMachine stateMachine = null;
            var guids = AssetDatabase.FindAssets("t:EditorManual.StateMachine.StateMachine");
            foreach (var path in
                guids.Where(guid => stateMachineID == guid).Select(guid => AssetDatabase.GUIDToAssetPath(guid)))
            {
                stateMachine = AssetDatabase.LoadAssetAtPath(path, typeof(StateMachine)) as StateMachine;
            }

            return stateMachine;
        }

        public static StateMachine Load()
        {
            var obj = Selection.activeObject;
            return obj && obj is StateMachine ? (StateMachine)obj : null;
        }
    }
}