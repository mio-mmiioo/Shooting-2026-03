#include "Enemy.h"

// 各敵のヘッダー
#include "Stone.h"
#include "Touhu.h"
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
