using System.IO;
using System.Text;
using UnityEditor;
using UnityEditor.ProjectWindowCallback;
using UnityEditor.Callbacks;

public class DoCreateScriptAsset : EndNameEditAction
{
	
	public override void Action (int instanceId, string pathName, string resourceFile)
	{
		var text = File.ReadAllText (resourceFile);
		var className = Path.GetFileNameWithoutExtension (pathName);
		className = className.Replace (" ", "");
		text = text.Replace ("#SCRIPTNAME#", className);

		text += "\n//コード追加！";
		
		var encoding = new UTF8Encoding (true, false);
		
		File.WriteAllText (pathName, text, encoding);

		AssetDatabase.ImportAsset (pathName);
		var asset = AssetDatabase.LoadAssetAtPath<MonoScript> (pathName);
		ProjectWindowUtil.ShowCreatedAsset (asset);
	}
}