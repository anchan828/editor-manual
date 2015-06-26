using UnityEngine;
using System.Collections;
using UnityEditor;

public class ExampleScope : EditorWindow
{

	[MenuItem ("Window/ExampleScope")]
	static void Open ()
	{
		GetWindow<ExampleScope> ();
	}

	void Sample1 ()
	{
		//#@@range_begin(HorizontalScope)
		using (new EditorGUILayout.HorizontalScope ()) {
			GUILayout.Button ("Button1");
			GUILayout.Button ("Button2");
		}
		//#@@range_end(HorizontalScope)
	}

	void OnGUI ()
	{
		//Sample1 ();
		//#@@range_begin(BackgroundColorScope_Example)
		using (new BackgroundColorScope (Color.green)) {
			// 緑色のボタン
			GUILayout.Button ("Button");

			using (new BackgroundColorScope (Color.yellow)) {
				// 黄色のボタン
				GUILayout.Button ("Button");
			}
		}
		//#@@range_end(BackgroundColorScope_Example)
	}


}

public class BackgroundColorScope : GUI.Scope
{
	private readonly Color color;

	public BackgroundColorScope (Color color)
	{
		this.color = GUI.backgroundColor;
		GUI.backgroundColor = color;
	}

	protected override void CloseScope ()
	{
		GUI.backgroundColor = color;
	}
}

/*
//#@@range_begin(BackgroundColorScope)
using UnityEngine;
 
public class BackgroundColorScope : GUI.Scope
{
    private readonly Color color;
    public BackgroundColorScope(Color color)
    {
        this.color = GUI.backgroundColor;
        GUI.backgroundColor = color;
    }


    protected override void CloseScope()
    {
        GUI.backgroundColor = color;
    }
}
//#@@range_end(BackgroundColorScope)
*/

/*
//#@@range_begin(HorizontalScopeCode)
public class HorizontalScope : GUI.Scope
{
	public HorizontalScope()
	{
		EditorGUILayout.BeginHorizontal();
	}

	protected override void CloseScope()
	{
		EditorGUILayout.EndHorizontal();
	}

	//略
}
//#@@range_end(HorizontalScopeCode)
*/