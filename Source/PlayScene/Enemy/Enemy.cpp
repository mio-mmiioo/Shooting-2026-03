#include "Enemy.h"
#include "../Player/Player.h"
#include "../../../MyLibrary/Observer.h"

// 各敵のヘッダー
#include "Stone.h"
#include "Touhu.h"

namespace Enemy
{
	// 弱点の色の初期設定
	const int RED = 150;
	const int GREEN = 150;
	const int BLUE = 150;
	const int ADD_RGB = 100; // 加算される色 変数名が気に食わない
	const float WEAK_POINT_CIRCLE_R = 20.0f;

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

void Enemy::DrawWeakPoint(std::string name, int hp, VECTOR3 weakPoint)
{
	int red = 0;
	int green = 0;
	int blue = 0;
	float raitio = (float)hp / (float)Data::enemyDataList[name].hp;

	if (raitio > 0.5)
	{
		green = GREEN + (int)(raitio * ADD_RGB);
		red = RED + ((int)(1.0f - raitio) * ADD_RGB);
	}
	else if (raitio == 0.5)
	{
		green = GREEN + (int)(raitio * ADD_RGB);
		red = RED + (int)(raitio * ADD_RGB);
	}
	else
	{
		green = GREEN + (int)(raitio * ADD_RGB);
		red = RED + ((int)(1.0f - raitio) * ADD_RGB);
	}
	int color = GetColor(red, green, blue);
	DrawCircle((int)weakPoint.x, (int)weakPoint.y, WEAK_POINT_CIRCLE_R, Color::BLACK, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawCircle((int)weakPoint.x, (int)weakPoint.y, WEAK_POINT_CIRCLE_R, color, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
