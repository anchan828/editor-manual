using UnityEngine;

public class Character : MonoBehaviour
{
	public int 基本攻撃力;
	public int 剣の強さ;
	public int ちから;
	public int hp;
	public Example example;
	public int 攻撃力 {
		get { 
			return 基本攻撃力 + Mathf.FloorToInt (基本攻撃力 * (剣の強さ + ちから - 8) / 16);
		}
	}
}


[System.Serializable]
public class Example
{
	public int minHp;
	public int maxHp;
}