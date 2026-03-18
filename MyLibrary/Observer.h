#pragma once

namespace Observer
{
	void Init();
	
	void AddScore(int addScore);
	void EnemyKilled();
	int GetEnemyKilled();
	int GetScore();
}