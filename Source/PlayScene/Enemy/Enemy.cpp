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
	const int BLEND_PARAM = 128;
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
		new Stone(objectData, Data::characterDataList["stone"]);
		break;
	case E_SORT::TOUHU:
		new Touhu(objectData, Data::characterDataList["touhu"]);
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
	int score = Data::characterDataList[name].score;
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
	float raitio = (float)hp / (float)Data::characterDataList[name].hp; // HPの割合

	// 割合が半分以上なら、緑が強め
	if (raitio > 0.5)
	{
		green = GREEN + (int)(raitio * ADD_RGB);
		red = RED + ((int)(1.0f - raitio) * ADD_RGB);
	}
	// 半分なら、黄色
	else if (raitio == 0.5)
	{
		green = GREEN + (int)(raitio * ADD_RGB);
		red = RED + (int)(raitio * ADD_RGB);
	}
	// 半分以下なら、赤が強め
	else
	{
		green = GREEN + (int)(raitio * ADD_RGB);
		red = RED + ((int)(1.0f - raitio) * ADD_RGB);
	}
	int color = GetColor(red, green, blue);

	// この書き方では遠近にかかわらず、同じサイズで描画され、違和感がある
	// プレイヤーと自身との距離を計算 → 半径に割合をかける修正を加えるべき
	DrawCircle((int)weakPoint.x, (int)weakPoint.y, WEAK_POINT_CIRCLE_R, Color::BLACK, FALSE); // 枠線
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BLEND_PARAM);
	DrawCircle((int)weakPoint.x, (int)weakPoint.y, WEAK_POINT_CIRCLE_R, color, TRUE); // HPの割合の色
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
