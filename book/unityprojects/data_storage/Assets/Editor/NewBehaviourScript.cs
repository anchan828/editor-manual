using UnityEditor;

public class NewBehaviourScript
{
	[InitializeOnLoadMethod]
	static void SaveConfig ()
	{
		EditorUserSettings.SetConfigValue ("Data 1", "text");		
	}
}
