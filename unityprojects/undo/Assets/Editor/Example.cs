using UnityEngine;
using UnityEditor;

public class Example
{
	[MenuItem("Example/Create Cube")]
	static void CreateCube ()
	{
		var cube = GameObject.CreatePrimitive (PrimitiveType.Cube);
		Undo.RegisterCreatedObjectUndo (cube, "Create Cube");
	}
}
