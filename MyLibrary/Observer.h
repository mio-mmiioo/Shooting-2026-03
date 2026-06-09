#pragma once

namespace Observer
{
	void Init();
	
	void AddScore(int addScore); // スコアを加算
	void EnemyKilled(); // 敵を倒した数を加算
	int GetEnemyKilled(); // 敵を倒した数を取得
	int GetScore(); // スコアを取得

	int SetCourse(int number); // プレイヤーが選んだコースをセット
	int GetCourse(); // プレイヤーが選んだコースをゲット

	void PlayerDamaged(); // プレイヤーが攻撃された
	bool GetIsPlayerNoDamage(); // true → プレイヤーがノーダメ
}