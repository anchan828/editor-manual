using UnityEngine;
using System.Collections;
using UnityEngine.Events;
using UnityEditor;
using System.Xml.Linq;
using System.Linq;
using System.Collections.Generic;

public class Colourlovers
{
	const string URL = "http://www.colourlovers.com/api";
	//	http://www.colourlovers.com/api/palettes/random
	public static void GetColors (int paletteID, UnityAction<ColorPalette> callback)
	{
		GetPalette (paletteID, (element) => {
			callback (GetColorPalette (element));
		});
	}

	public static void GetTopPalettes (int page, UnityAction<ColorPalette[]> callback)
	{
		var www = new WWW (string.Format ("{0}/palettes/top?showPaletteWidths=1&resultOffset={1}", URL, page));
		GetPalettes (www, page, callback);
	}

	private static void GetPalettes (WWW www, int page, UnityAction<ColorPalette[]> callback)
	{
		EditorApplication.CallbackFunction action = null;
		action = () => {
			if (www.isDone && string.IsNullOrEmpty (www.error)) {
				EditorApplication.update -= action;


				var palettes = XDocument.Parse (www.text)
					.Descendants ()
					.Where (x => x.Name == "palette")
					.Select (x => GetColorPalette (x))
					.ToArray ();

				callback (palettes);
			}
		};

		EditorApplication.update += action;
	}

	private static void GetPalette (WWW www, UnityAction<ColorPalette> callback)
	{
		EditorApplication.CallbackFunction action = null;
		action = () => {
			if (www.isDone && string.IsNullOrEmpty (www.error)) {
				EditorApplication.update -= action;
				callback (GetColorPalette (XDocument.Parse (www.text).Root));
			}
		};

		EditorApplication.update += action;
		
	}

	public static void GetRandomPalette (UnityAction<ColorPalette> callback)
	{
		var www = new WWW (string.Format ("{0}/palettes/random?showPaletteWidths=1", URL));
		GetPalette (www, callback);
	}

	private static ColorPalette GetColorPalette (XElement element)
	{
		var hexes = element
			.Descendants ()
			.Where (x => x.Name == "hex")
			.Select (x => x.Value)
			.ToArray ();

		var widths = element
			.Descendants ()
			.Where (x => x.Name == "colorWidths")
			.SelectMany (x => x.Value.Split (','))
			.Select (w => float.Parse (w))
			.ToArray ();

		if (hexes.Length != widths.Length) {
			Debug.LogWarning ("hexes.Length != widths.Length");
		}

		return new ColorPalette (hexes, widths);
	}

	public static void GetPalette (int paletteID, UnityAction<XElement> callback)
	{
		var www = new WWW (string.Format ("{0}/palette/{1}?showPaletteWidths=1", URL, paletteID));
		EditorApplication.CallbackFunction action = null;

		action = () => {
			if (www.isDone && string.IsNullOrEmpty (www.error)) {
				EditorApplication.update -= action;
				callback (XDocument.Parse (www.text).Root);
			}
		};

		EditorApplication.update += action;
	}
}

public class ColorPalette
{
	public ColorPalette (string[] hexStrings, float[] widths)
	{
		for (int i = 0; i < hexStrings.Length; i++) {
			
			Color color;
			if (Color.TryParseHexString (hexStrings [i], out color)) {
				Add (color, widths [i]);
			}
		}
	}

	public List<ColourloversHex> hexes = new List<ColourloversHex> ();

	public void Add (Color color, float width)
	{
		hexes.Add (new ColourloversHex (color, width));
	}
}

public class ColourloversHex
{
	public Color color { get; set; }

	public float width { get; set; }

	public ColourloversHex (Color color, float width)
	{
		this.color = color;
		this.width = width;
	}
}


public static class ColourloversHexEx
{
	public static Color[] ToColors (this List<ColourloversHex> hexes)
	{
		return hexes.Select (h => h.color).ToArray ();
	}
}