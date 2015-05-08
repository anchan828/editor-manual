using UnityEngine;

public class Player : MonoBehaviour
{
	public PlayerInfo info;
}

[System.Serializable]
public class PlayerInfo
{
	public string name;
	public int hp;
}