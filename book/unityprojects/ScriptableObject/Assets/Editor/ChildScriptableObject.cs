using UnityEngine;

public class ChildScriptableObject : ScriptableObject
{
	[SerializeField]
	string str;

	public ChildScriptableObject ()
	{
		name = "New ChildScriptableObject";
	}
}
