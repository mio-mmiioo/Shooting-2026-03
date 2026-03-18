#include "Observer.h"

namespace Observer
{
	int enemyKilledCounter;
	int score;
}

void Observer::Init()
{
	enemyKilledCounter = 0;
	score = 0;
}

void Observer::AddScore(int addScore)
{
	score += addScore;
}

void Observer::EnemyKilled()
{
	enemyKilledCounter += 1;
}

int Observer::GetEnemyKilled()
{
	return enemyKilledCounter;
}

int Observer::GetScore()
{
	return score;
}
