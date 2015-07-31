using UnityEngine;

public class Character : MonoBehaviour
{
	public Example example;
}


[System.Serializable]
public class Example
{
	public int minHp;
	public int maxHp;
}