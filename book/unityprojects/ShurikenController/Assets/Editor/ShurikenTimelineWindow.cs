using UnityEngine;
using System.Collections.Generic;
using UnityEditor;
using System.Reflection;
using System.Linq;

public class ShurikenTimelineWindow : EditorWindow
{
	[MenuItem ("Window/Shuriken Timeline")]
	static void Open ()
	{
		GetWindow<ShurikenTimelineWindow> (true).titleContent = new GUIContent ("Shuriken Timeline");
	}

	TimeControl timeControl = new TimeControl ();
	Dictionary<ParticleSystem,bool> playlist = new Dictionary<ParticleSystem, bool> ();

	void OnEnable ()
	{
		Initialize ();
	}

	void Initialize ()
	{
		Selection.selectionChanged += PlaylistUpdate;
		EditorApplication.hierarchyWindowChanged += PlaylistUpdate;
		EditorApplication.projectWindowChanged += PlaylistUpdate;
		PlaylistUpdate ();
	}

	bool IsRoot (ParticleSystem ps)
	{
		var parent = ps.transform.parent;

		if (parent == null)
			return true;

		return parent.GetComponent<ParticleSystem> () == false;
	}

	void PlaylistUpdate ()
	{
		var _playlist = new Dictionary<ParticleSystem, bool> ();

		foreach (var ps in FindObjectsOfType<ParticleSystem> ()) {

			if (IsRoot (ps)) {
				var on = true;

				if (playlist.ContainsKey (ps)) {
					on = playlist [ps];
				}
				if (_playlist.ContainsKey (ps) == false) {
					_playlist.Add (ps, on);
				}
			
			}
		}  

		playlist = _playlist;

		RefreshMaxTime ();
		Repaint ();
	}


	void RefreshMaxTime ()
	{
		float maxTime = 0;
		
		foreach (var item in playlist) {
			if (item.Value == false)
				continue;
			maxTime = Mathf.Max (maxTime, item.Key.duration + item.Key.startLifetime);
			timeControl.SetMinMaxTime (0, maxTime);
		}
	}

	void OnGUI ()
	{
		EditorGUI.BeginChangeCheck ();
		DrawTimeSlider ();

		var particleSystems = new List<ParticleSystem> (playlist.Keys);

		DrawPlaylist (particleSystems);

		if (EditorGUI.EndChangeCheck () || timeControl.isPlaying) {

			foreach (var particleSystem in particleSystems) {

				if (playlist [particleSystem]) {
					particleSystem.Simulate (timeControl.currentTime, true, true);
				} else
					particleSystem.Simulate (0, true, true);
			}
			SceneView.RepaintAll ();
		}
	}

	void DrawPlaylist (List<ParticleSystem> particleSystems)
	{
		GUILayout.Space (24);
		EditorGUILayout.LabelField ("Playlist");


		using (new EditorGUILayout.VerticalScope ("box", GUILayout.MinHeight (50))) {

			var lookup = particleSystems.Select ((Item, Index) => new { Item, Index })
				.ToLookup (arg => arg.Index / 3, arg => arg.Item);

			foreach (var keys in lookup) {
				using (new EditorGUILayout.HorizontalScope ()) {
					foreach (var key in keys) {
						EditorGUI.BeginChangeCheck ();

						playlist [key] = GUILayout.Toggle (playlist [key], key.name, EditorStyles.miniButton, GUILayout.MaxWidth (position.width / 3));
						if (EditorGUI.EndChangeCheck ())
							RefreshMaxTime ();

					}
				}
			}
		}


	}

	void DrawTimeSlider ()
	{
		EditorGUILayout.Space ();
		if (Event.current.type == EventType.KeyDown) {
			timeControl.Pause ();
			if (Event.current.keyCode == KeyCode.RightArrow)
				timeControl.currentTime += 0.01f;
			if (Event.current.keyCode == KeyCode.LeftArrow)
				timeControl.currentTime -= 0.01f;
			
			GUI.changed = true;
			Event.current.Use ();
			Repaint ();
		}
		using (new EditorGUILayout.HorizontalScope ()) {

			EditorGUI.BeginChangeCheck ();

			var _isPlaying = GUILayout.Toggle (timeControl.isPlaying, "Play", "button", GUILayout.Width (60), GUILayout.Height (40));

			if (EditorGUI.EndChangeCheck ()) {
				GUI.FocusControl ("");
				if (_isPlaying)
					timeControl.Play ();
				else
					timeControl.Pause ();
			}

			EditorGUI.BeginChangeCheck ();

			timeControl.currentTime = GUILayout.HorizontalSlider (timeControl.currentTime, timeControl.minTime, timeControl.maxTime, "box", "box", GUILayout.Height (40), GUILayout.ExpandWidth (true));

			var lastRect = GUILayoutUtility.GetLastRect ();

			var timeLength = timeControl.maxTime - timeControl.minTime;

			var gridline = timeLength * 2;

			GUI.skin.label.alignment = TextAnchor.MiddleRight;
			var sliderRect = new Rect (lastRect);
			for (int i = 1; i < gridline; i++) {
				var x = (sliderRect.width / gridline) * i;
				x += 4f - 1.5f * (i - 1);
				Handles.DrawLine (new Vector2 (sliderRect.x + x, sliderRect.y), new Vector2 (sliderRect.x + x, sliderRect.y + sliderRect.height));
				Handles.Label (new Vector2 (sliderRect.x + x - 10, sliderRect.y - 10), (timeLength / gridline * i).ToString ("0.0"));
			}

			if (EditorGUI.EndChangeCheck ()) {
				timeControl.Pause ();
				GUI.FocusControl ("");
			}

			if (timeControl.isPlaying)
				Repaint ();




			GUI.Label (new Rect (55, lastRect.y + 40, 20, 20), "0");

			EditorGUI.BeginChangeCheck ();

			var _currentTime = EditorGUI.FloatField (new Rect (position.width * 0.5f, lastRect.y + 43, 40, 20), (float)System.Math.Round (timeControl.currentTime, 2), EditorStyles.miniTextField);

			if (EditorGUI.EndChangeCheck ()) {
				timeControl.Pause ();
				timeControl.currentTime = _currentTime;
			}

			GUI.Label (new Rect (position.width - 40, lastRect.y + 40, 40, 20), timeControl.maxTime.ToString ());
		}

	}
}
