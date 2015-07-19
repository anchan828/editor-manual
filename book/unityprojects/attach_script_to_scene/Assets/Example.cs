using UnityEngine;
using System.Collections;

public class Example : MonoBehaviour
{
	[Range (0, 100)]
	public int @int;

	[TextArea (5, 5)]
	public string textArea;

	public Hoge hoge;

	public enum Hoge
	{
		Foo,
		Bar
	}

	void OnGUI ()
	{
		foreach (var item in FindObjectsOfType<GameObject>()) {
			GUILayout.Label (item.name);
		}
	}
}
