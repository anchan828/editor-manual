using UnityEngine;
using System.Collections;

public class Easing
{

	public static float easeInQuad (float t)
	{
		return easeInQuad (t, 0f, 1f, 1f);
	}

	public static float easeOutQuad (float t)
	{
		return easeInQuad (t, 0f, 1f, 1f);
	}

	public static float easeInOutQuad (float t)
	{
		return easeInOutQuad (t, 0f, 1f, 1f);
	}

	public static float easeInCubic (float t)
	{
		return easeInCubic (t, 0f, 1f, 1f);
	}

	public static float easeOutCubic (float t)
	{
		return easeOutCubic (t, 0f, 1f, 1f);
	}

	public static float easeInOutCubic (float t)
	{
		return easeInOutCubic (t, 0f, 1f, 1f);
	}

	public static float easeInQuart (float t)
	{
		return easeInQuart (t, 0f, 1f, 1f);
	}

	public static float easeOutQuart (float t)
	{
		return easeOutQuart (t, 0f, 1f, 1f);
	}

	public static float easeInOutQuart (float t)
	{
		return easeInOutQuart (t, 0f, 1f, 1f);
	}

	public static float easeInQuint (float t)
	{
		return easeInQuint (t, 0f, 1f, 1f);
	}

	public static float easeOutQuint (float t)
	{
		return easeOutQuint (t, 0f, 1f, 1f);
	}

	public static float easeInOutQuint (float t)
	{
		return easeInOutQuint (t, 0f, 1f, 1f);
	}

	public static float easeInSine (float t)
	{
		return easeInSine (t, 0f, 1f, 1f);
	}

	public static float easeOutSine (float t)
	{
		return easeOutSine (t, 0f, 1f, 1f);
	}

	public static float easeInOutSine (float t)
	{
		return easeInOutSine (t, 0f, 1f, 1f);
	}

	public static float easeInExpo (float t)
	{
		return easeInExpo (t, 0f, 1f, 1f);
	}

	public static float easeOutExpo (float t)
	{
		return easeOutExpo (t, 0f, 1f, 1f);
	}

	public static float easeInOutExpo (float t)
	{
		return easeInOutExpo (t, 0f, 1f, 1f);
	}

	public static float easeInCirc (float t)
	{
		return easeInCirc (t, 0f, 1f, 1f);
	}

	public static float easeOutCirc (float t)
	{
		return easeOutCirc (t, 0f, 1f, 1f);
	}

	public static float easeInOutCirc (float t)
	{
		return easeInOutCirc (t, 0f, 1f, 1f);
	}

	public static float easeInElastic (float t)
	{
		return easeInElastic (t, 0f, 1f, 1f);
	}

	public static float easeOutElastic (float t)
	{
		return easeOutElastic (t, 0f, 1f, 1f);
	}

	public static float easeInOutElastic (float t)
	{
		return easeInOutElastic (t, 0f, 1f, 1f);
	}

	public static float easeInBack (float t)
	{
		return easeInBack (t, 0f, 1f, 1f, 1.70158f);
	}

	public static float easeOutBack (float t)
	{
		return easeOutBack (t, 0f, 1f, 1f, 1.70158f);
	}

	public static float easeInOutBack (float t)
	{
		return easeInOutBack (t, 0f, 1f, 1f, 1.70158f);
	}

	public static float easeInBounce (float t)
	{
		return easeInBounce (t, 0f, 1f, 1f);
	}

	public static float easeOutBounce (float t)
	{
		return easeOutBounce (t, 0f, 1f, 1f);
	}

	public static float easeInOutBounce (float t)
	{
		return easeInOutBounce (t, 0f, 1f, 1f);
	}

	private static float easeInQuad (float t, float b, float c, float d)
	{
		return c * (t /= d) * t + b;
	}

	private static float easeOutQuad (float t, float b, float c, float d)
	{
		return -c * (t /= d) * (t - 2f) + b;
	}

	private static float easeInOutQuad (float t, float b, float c, float d)
	{
		if ((t /= d / 2f) < 1)
			return c / 2f * t * t + b;
		return -c / 2f * ((--t) * (t - 2f) - 1) + b;
	}

	private static float easeInCubic (float t, float b, float c, float d)
	{
		return c * (t /= d) * t * t + b;
	}

	private static float easeOutCubic (float t, float b, float c, float d)
	{
		return c * ((t = t / d - 1f) * t * t + 1f) + b;
	}

	private static float easeInOutCubic (float t, float b, float c, float d)
	{
		if ((t /= d / 2f) < 1)
			return c / 2f * t * t * t + b;
		return c / 2f * ((t -= 2f) * t * t + 2f) + b;
	}

	private static float easeInQuart (float t, float b, float c, float d)
	{
		return c * (t /= d) * t * t * t + b;
	}

	private static float easeOutQuart (float t, float b, float c, float d)
	{
		return -c * ((t = t / d - 1f) * t * t * t - 1f) + b;
	}

	private static float easeInOutQuart (float t, float b, float c, float d)
	{
		if ((t /= d / 2f) < 1f)
			return c / 2f * t * t * t * t + b;
		return -c / 2f * ((t -= 2f) * t * t * t - 2f) + b;
	}

	private static float easeInQuint (float t, float b, float c, float d)
	{
		return c * (t /= d) * t * t * t * t + b;
	}

	private static float easeOutQuint (float t, float b, float c, float d)
	{
		return c * ((t = t / d - 1f) * t * t * t * t + 1f) + b;
	}

	private static float easeInOutQuint (float t, float b, float c, float d)
	{
		if ((t /= d / 2f) < 1f)
			return c / 2f * t * t * t * t * t + b;
		return c / 2f * ((t -= 2f) * t * t * t * t + 2f) + b;
	}

	private static float easeInSine (float t, float b, float c, float d)
	{
		return (float)(-c * Mathf.Cos (t / d * (Mathf.PI / 2f)) + c + b);
	}

	private static float easeOutSine (float t, float b, float c, float d)
	{
		return (float)(c * Mathf.Sin (t / d * (Mathf.PI / 2f)) + b);
	}

	private static float easeInOutSine (float t, float b, float c, float d)
	{
		return (float)(-c / 2f * (Mathf.Cos (Mathf.PI * t / d) - 1) + b);
	}

	private static float easeInExpo (float t, float b, float c, float d)
	{
		return (float)((t == 0) ? b : c * Mathf.Pow (2, 10f * (t / d - 1f)) + b);
	}

	private static float easeOutExpo (float t, float b, float c, float d)
	{
		return (float)((t == d) ? b + c : c * (-Mathf.Pow (2, -10f * t / d) + 1f) + b);
	}

	private static float easeInOutExpo (float t, float b, float c, float d)
	{
		if (t == 0)
			return b;
		if (t == d)
			return b + c;
		if ((t /= d / 2f) < 1f)
			return (float)(c / 2f * Mathf.Pow (2, 10f * (t - 1f)) + b);
		return (float)(c / 2f * (-Mathf.Pow (2, -10f * --t) + 2f) + b);
	}

	private static float easeInCirc (float t, float b, float c, float d)
	{
		return (float)(-c * (Mathf.Sqrt (1f - (t /= d) * t) - 1f) + b);
	}

	private static float easeOutCirc (float t, float b, float c, float d)
	{
		return (float)(c * Mathf.Sqrt (1 - (t = t / d - 1) * t) + b);
	}

	private static float easeInOutCirc (float t, float b, float c, float d)
	{
		if ((t /= d / 2) < 1)
			return (float)(-c / 2 * (Mathf.Sqrt (1 - t * t) - 1) + b);
		return (float)(c / 2 * (Mathf.Sqrt (1 - (t -= 2) * t) + 1) + b);
	}

	private static float easeInElastic (float t, float b, float c, float d)
	{
		float s = 1.70158f;
		float p = 0;
		float a = c;
		if (t == 0)
			return b;
		if ((t /= d) == 1)
			return b + c;
		p = d * 0.3f;
		if (a < Mathf.Abs (c)) {
			a = c;
			s = p / 4f;
		} else
			s = (float)(p / (2f * Mathf.PI) * Mathf.Asin (c / a));
		return (float)(-(a * Mathf.Pow (2, 10f * (t -= 1f)) * Mathf.Sin ((t * d - s) * (2f * Mathf.PI) / p)) + b);
	}

	private static float easeOutElastic (float t, float b, float c, float d)
	{
		float s = 1.70158f;
		float p = 0;
		float a = c;
		if (t == 0)
			return b;
		if ((t /= d) == 1)
			return b + c;
		p = d * 0.3f;
		if (a < Mathf.Abs (c)) {
			a = c;
			s = p / 4f;
		} else
			s = (float)(p / (2f * Mathf.PI) * Mathf.Asin (c / a));
		return (float)(a * Mathf.Pow (2, -10f * t) * Mathf.Sin ((t * d - s) * (2f * Mathf.PI) / p) + c + b);
	}

	private static float easeInOutElastic (float t, float b, float c, float d)
	{
		float s = 1.70158f;
		float p = 0;
		float a = c;
		if (t == 0)
			return b;
		if ((t /= d / 2) == 2)
			return b + c;
		p = d * (0.3f * 1.5f);
		if (a < Mathf.Abs (c)) {
			a = c;
			s = p / 4f;
		} else
			s = (float)(p / (2f * Mathf.PI) * Mathf.Asin (c / a));
		if (t < 1)
			return (float)(-0.5f * (a * Mathf.Pow (2, 10f * (t -= 1f)) * Mathf.Sin ((t * d - s) * (2f * Mathf.PI) / p)) + b);
		return (float)(a * Mathf.Pow (2, -10f * (t -= 1f)) * Mathf.Sin ((t * d - s) * (2f * Mathf.PI) / p) * 0.5f + c + b);
	}

	private static float easeInBack (float t, float b, float c, float d, float s)
	{
		return c * (t /= d) * t * ((s + 1f) * t - s) + b;
	}

	private static float easeOutBack (float t, float b, float c, float d, float s)
	{
		return c * ((t = t / d - 1f) * t * ((s + 1f) * t + s) + 1f) + b;
	}

	private static float easeInOutBack (float t, float b, float c, float d, float s)
	{
		if ((t /= d / 2f) < 1)
			return c / 2f * (t * t * (((s *= (1.525f)) + 1f) * t - s)) + b;
		return c / 2f * ((t -= 2f) * t * (((s *= (1.525f)) + 1f) * t + s) + 2f) + b;
	}

	private static float easeInBounce (float t, float b, float c, float d)
	{
		return c - easeOutBounce (d - t, 0, c, d) + b;
	}

	private static float easeOutBounce (float t, float b, float c, float d)
	{
		if ((t /= d) < (1f / 2.75f)) {
			return c * (7.5625f * t * t) + b;
		} else if (t < (2f / 2.75f)) {
			return c * (7.5625f * (t -= (1.5f / 2.75f)) * t + 0.75f) + b;
		} else if (t < (2.5f / 2.75f)) {
			return c * (7.5625f * (t -= (2.25f / 2.75f)) * t + 0.9375f) + b;
		} else {
			return c * (7.5625f * (t -= (2.625f / 2.75f)) * t + 0.984375f) + b;
		}
	}

	private static float easeInOutBounce (float t, float b, float c, float d)
	{
		if (t < d / 2f)
			return easeInBounce (t * 2f, 0, c, d) * 0.5f + b;
		return easeOutBounce (t * 2f - d, 0, c, d) * 0.5f + c * 0.5f + b;
	}
}
