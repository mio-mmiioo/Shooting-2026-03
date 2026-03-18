#include "Enemy.h"
#include "../Player/Player.h"
#include "../../../MyLibrary/Observer.h"

// 各敵のヘッダー
#include "Stone.h"
#include "Touhu.h"

namespace Enemy
{
	Player* player;
}

void Enemy::Update()
{
	player = FindGameObject<Player>();
}

void Enemy::Release()
{
	player = nullptr;
	delete player;
}

void Enemy::CreateEnemy(Data::ObjectData objectData)
{
	switch (objectData.objectNumber)
	{
	case E_SORT::STONE:
		new Stone(objectData, Data::enemyDataList["stone"]);
		break;
	case E_SORT::TOUHU:
		new Touhu(objectData, Data::enemyDataList["touhu"]);
		break;
	}
}

VECTOR3 Enemy::GetPlayerPosition()
{
	return player->GetTransform().position_;
}

void Enemy::AttackPlayer(int attackPower)
{
	player->AddHp(-attackPower);
}

void Enemy::SetObserver(std::string name, bool isEnemy)
{
	int score = Data::enemyDataList[name].score;
	Observer::AddScore(score);

	if (isEnemy == true)
	{
		Observer::EnemyKilled();
	}
}
