#include "Observer.h"

namespace Observer
{
	int enemyKilledCounter;
	int score;
	int courseNumber; // 遊ぶコースの番号
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

int Observer::SetCourse(int number)
{
	courseNumber = number;
	return courseNumber;
}

int Observer::GetCourse()
{
	return courseNumber;
}
