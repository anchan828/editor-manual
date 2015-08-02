using UnityEngine;
using UnityEditor;

[CreateAssetMenu (menuName = "Example/Create ExampleAsset Instance", order = 2000)]
public class NewBehaviourScript : ScriptableObject
{
	[Range (0, 10)]
	public int number = 3;

	public bool toggle = false;

	public string[] texts = new string[5];
}
