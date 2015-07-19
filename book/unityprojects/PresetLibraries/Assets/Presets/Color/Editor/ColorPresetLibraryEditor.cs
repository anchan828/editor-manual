using UnityEngine;
using System.Collections;
using UnityEditor;
using System;
using System.Linq;
using System.Collections.Generic;


public class ColorPresetLibraryEditorWidnow : EditorWindow
{
	[MenuItem ("Window/ColorPresetLibrary")]
	static void Open ()
	{
		GetWindow<ColorPresetLibraryEditorWidnow> ();
	}

	int paletteID = 0;

	int selectedPanel = 0;
	ColorPalette singleColorPalette;
	ColorPalette[] topPalette = new ColorPalette[0];
	ColorPalette[] randomPalette = new ColorPalette[0];

	void OnGUI ()
	{
		EditorGUILayout.Space ();

		using (new EditorGUILayout.HorizontalScope ()) {
			GUILayout.Space (position.width * 0.25f);
			selectedPanel = GUILayout.Toolbar (selectedPanel, new string[]{ "Panel", "Top", "Random" }, GUILayout.Width (position.width * 0.5f));

		}


		switch (selectedPanel) {
		case 0:
			DrawSinglePalette ();
			break;
		case 1:
			DrawTopPalettes ();
			break;
		case 2:
			DrawRandomPalettes ();
			break;
			
		}

	}

	void OnEnable ()
	{
		Colourlovers.GetColors (paletteID, (colorPalette) => {
			singleColorPalette = colorPalette;
		});

		Colourlovers.GetTopPalettes (currentPage, (palettes) => {
			this.topPalette = palettes;
		});

		for (int i = 0; i < 10; i++) {
			Colourlovers.GetRandomPalette ((palette) => {
				ArrayUtility.Add (ref randomPalette, palette);
			});
		}
	}

	Vector2 pos = Vector2.zero;
	int currentPage = 0;

	void DrawTopPalettes ()
	{
		EditorGUI.BeginChangeCheck ();
		currentPage = GUILayout.Toolbar (currentPage, new string[] {
			"1",
			"2",
			"3",
			"4",
			"5",
			"6",
			"7",
			"8",
			"9",
			"10"
		});

		if (EditorGUI.EndChangeCheck ()) {
			topPalette = new ColorPalette[0];
			Colourlovers.GetTopPalettes (currentPage, (palettes) => {
				this.topPalette = palettes;
			});
		}

		using (var scope = new EditorGUILayout.ScrollViewScope (pos)) {
			foreach (var palette in topPalette) {
				DrawPalette (palette);
			}
			pos = scope.scrollPosition;
		}
	}

	void DrawRandomPalettes ()
	{
		
		if (GUILayout.Button ("Reload")) {
			randomPalette = new ColorPalette[0];
			for (int i = 0; i < 10; i++) {
				Colourlovers.GetRandomPalette ((palette) => {
					ArrayUtility.Add (ref randomPalette, palette);
					Repaint ();
				});
			}
		}

		using (var scope = new EditorGUILayout.ScrollViewScope (pos)) {
			foreach (var palette in randomPalette) {
				DrawPalette (palette);
			}
			pos = scope.scrollPosition;
		}
	}

	void DrawPalette (ColorPalette palette)
	{
		var _bg = GUI.backgroundColor;

		var hexes = palette.hexes;

		using (new EditorGUILayout.HorizontalScope ()) {

			GUILayout.Space (position.width * 0.05f);

			for (int i = 0; i < hexes.Count; i++) {
				GUI.backgroundColor = hexes [i].color;
				GUILayout.Box ("", Styles.palette, GUILayout.Width (position.width * 0.9f * hexes [i].width));
			}

		}

		GUI.backgroundColor = _bg;

		if (hexes.Count == 0)
			return;

		using (new EditorGUILayout.HorizontalScope ()) {
			GUILayout.Space (position.width * 0.6f);

			var libs = Selection.GetFiltered (ColorPresetLibrary.colorLibType, SelectionMode.TopLevel);
			EditorGUI.BeginDisabledGroup (libs.Length == 0 || libs.Cast<ScriptableObject> ().All (x => ColorPresetLibrary.IsColorPresetLibrary (x)) == false);

			if (GUILayout.Button ("Add", EditorStyles.miniButton)) {
				foreach (var lib in libs) {
					ColorPresetLibrary.Add ((ScriptableObject)lib, hexes.Select (h => h.color).ToArray ());
					ColorPresetLibrary.Save ((ScriptableObject)lib);
				}
			}

			EditorGUI.EndDisabledGroup ();

			if (GUILayout.Button ("Create", EditorStyles.miniButton)) {
				ColorPresetLibrary.Create (hexes.ToColors ());
			}
		}
	}

	void DrawSinglePalette ()
	{
		EditorGUILayout.Space ();

		EditorGUI.BeginChangeCheck ();

		paletteID = EditorGUILayout.IntField ("Palette ID", paletteID);

		if (EditorGUI.EndChangeCheck ()) {
			Colourlovers.GetColors (paletteID, (colorPalette) => {
				singleColorPalette = colorPalette;
			});
		}


		EditorGUILayout.LabelField ("Preview");

		if (singleColorPalette != null)
			DrawPalette (singleColorPalette);

	}

	void OnSelectionChange ()
	{
		Repaint ();
	}


	class Styles
	{
		public static GUIStyle palette;

		static Styles ()
		{
			palette = new GUIStyle (EditorStyles.toolbar);
			palette.stretchHeight = true;
			palette.fixedHeight = 40;
		}
	}
}
