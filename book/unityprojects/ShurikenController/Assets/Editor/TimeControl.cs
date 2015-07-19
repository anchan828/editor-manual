using UnityEditor;
using UnityEngine;

public class TimeControl
{

	private float m_currentTime = 0;
	private double m_lastFrameEditorTime = 0;

	public float minTime { get; private set; }

	public float maxTime { get; private set; }

	public bool isPlaying { get; private set; }

	public float speed { get; set; }

	public float currentTime {
		get { 
			m_currentTime = Mathf.Repeat (m_currentTime, maxTime);
			m_currentTime = Mathf.Clamp (m_currentTime, minTime, maxTime);
			return m_currentTime;
		}
		set { 
			m_currentTime = value;
		}
	}

	public TimeControl ()
	{
		speed = 1;
		EditorApplication.update += TimeUpdate;
	}

	public void TimeUpdate ()
	{
		if (isPlaying) {
			var timeSinceStartup = EditorApplication.timeSinceStartup;
			var deltaTime = timeSinceStartup - m_lastFrameEditorTime;
			m_lastFrameEditorTime = timeSinceStartup;
			currentTime += (float)deltaTime * speed;
		}
	}

	public void Play ()
	{
		isPlaying = true;
		m_lastFrameEditorTime = EditorApplication.timeSinceStartup;
	}

	public void Pause ()
	{
		isPlaying = false;
	}

	public void Stop ()
	{
		isPlaying = false;
		currentTime = 0;
	}

	public void SetMinMaxTime (float minTime, float maxTime)
	{
		this.minTime = 0;
		this.maxTime = maxTime;
	}
}
