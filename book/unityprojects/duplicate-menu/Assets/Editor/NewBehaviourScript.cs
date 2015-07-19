using UnityEngine;
using System.Collections;
using UnityEditor;
using System.Linq;

public class NewBehaviourScript
{
	static string[] DetectRemovedComponents ()
	{
		// Prefab 取得
		var prefab = PrefabUtility.GetPrefabParent (Selection.activeGameObject);

		// Prefab にアタッチされているコンポーネント取得（コンポーネント名の配列）
		var prefab_dependencies = EditorUtility.CollectDeepHierarchy (new Object[]{ prefab })
			.Where (o => o is Component)
			.Select (c => c.ToString ())
			.ToArray ();

		// GameObject にアタッチされているコンポーネント取得（コンポーネント名の配列）
		var gameObject_dependencies = EditorUtility.CollectDeepHierarchy (Selection.GetFiltered (typeof(GameObject), SelectionMode.DeepAssets))
			.Where (o => o is Component)
			.Select (c => c.ToString ())
			.ToArray ();

		var removed_components = prefab_dependencies;

		// ２つの配列で重複するものを削除。 => 残ったものが削除したコンポーネント
		foreach (var gameObject_dependency in gameObject_dependencies) {
			ArrayUtility.Remove (ref removed_components, gameObject_dependency);
		}

		return removed_components;
	}
}
