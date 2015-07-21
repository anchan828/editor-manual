using UnityEngine;
using System;
using UnityEditor;

public class Example : MonoBehaviour
{
	[SerializeField]
	Character[] characters;
}


[Serializable]
public class Character
{
	[SerializeField]
	Texture icon;
	
	[SerializeField]
	string name;

	[SerializeField]
	int hp;

	[SerializeField]
	int power;

}


[CustomPropertyDrawer (typeof(Character))]
public class CharacterDrawer : PropertyDrawer
{
	private Character character;


	public override void OnGUI (Rect position, SerializedProperty property, GUIContent label)
	{

		using (new EditorGUI.PropertyScope (position, label, property)) {
			EditorGUIUtility.labelWidth = 50;
			position.height = EditorGUIUtility.singleLineHeight;

			var iconRect = new Rect (position) {
				width = 64,
				height = 64
			};

			var halfWidth = position.width * 0.5f;

			var nameRect = new Rect (position) {
				width = position.width - 64,
				x = position.x + 64
			};

			var hpRect = new Rect (nameRect) {
				y = nameRect.y + EditorGUIUtility.singleLineHeight + 2
			};

			var powerRect = new Rect (hpRect) {
				y = hpRect.y + EditorGUIUtility.singleLineHeight + 2
			};

			var iconProperty = property.FindPropertyRelative ("icon");
			var nameProperty = property.FindPropertyRelative ("name");
			var hpProperty = property.FindPropertyRelative ("hp");
			var powerProperty = property.FindPropertyRelative ("power");

			iconProperty.objectReferenceValue = EditorGUI.ObjectField (iconRect, iconProperty.objectReferenceValue, typeof(Texture), false);

			nameProperty.stringValue = EditorGUI.TextField (nameRect, nameProperty.displayName, nameProperty.stringValue);
			EditorGUI.IntSlider (hpRect, hpProperty, 0, 100);
			EditorGUI.IntSlider (powerRect, powerProperty, 0, 10);

		}
	}
}