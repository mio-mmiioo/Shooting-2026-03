#include "Stone.h"
#include <assert.h>
#include "../../../MyLibrary/Collision.h"
#include "../../../MyLibrary/Observer.h"
#include "../../../MyLibrary/Color.h"

namespace STONE
{
	const VECTOR3 ADD_WEAK_POSITION = { 0.0f, 50.0f, 0.0f }; // 加算する弱点の場所
	const float WEAK_POINT_CIRCLE_R = 20.0f; // 弱点のUIの半径

	// 弱点の色の初期設定
	const int RED = 150;
	const int GREEN = 150;
	const int BLUE = 150;
	const int ADD_RGB = 100; // 加算される色 変数名が気に食わない
}

Stone::Stone(Data::ObjectData objectData, Data::EnemyData enemyData)
{
	hModel_ = MV1DuplicateModel(Data::models[objectData.name]);
	hitModel_ = MV1DuplicateModel(Data::models[objectData.name + "_c"]);

	transform_ = objectData.t;
	maxHp_ = enemyData.hp;
	hp_ = enemyData.hp;
	score_ = enemyData.score;

	transform_.MakeLocalMatrix();
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1SetupCollInfo(hitModel_);

	gravity_ = enemyData.gravity;
	moveSpeed_ = enemyData.moveSpeed;
	rotateSpeed_ = enemyData.rotateSpeed;
	distanceR_ = enemyData.distanceR;

	objectNumber_ = OBJECT_SORT::OBJ_CHARA;
	red_ = STONE::RED;
	green_ = STONE::GREEN + STONE::ADD_RGB;
	blue_ = STONE::BLUE;
	weakPointColor_ = GetColor(red_, green_, blue_);


	Collision::AddObject(this);
}

Stone::~Stone()
{
	if (hModel_ > 0)
	{
		MV1DeleteModel(hModel_);
		hModel_ = -1;
	}
	if (hitModel_ > 0)
	{
		MV1DeleteModel(hitModel_);
		hModel_ = -1;
	}
	Observer::EnemyKilled();
}

void Stone::Update()
{
	// 体力が0の場合の処理
	if (hp_ <= 0)
	{
		Enemy::SetObserver("stone", false);
		Collision::DeleteObject(this);
		DestroyMe();
		return;
	}
}

void Stone::Draw()
{
	Object3D::Draw();

	VECTOR3 hit;
	// 敵自身とプレイヤーの直線距離に障害物がないことを確認する
	if (Collision::CheckLineHitObject(transform_.position_, Enemy::GetPlayerPosition() + LOOK_HEIGHT, &hit) == false)
	{
		// ワールドスクリーン座標変換をする
		VECTOR3 weakPoint = ConvWorldPosToScreenPos(transform_.position_ + STONE::ADD_WEAK_POSITION);
		// 撃ってほしい部分を半透明の〇で表示する
		if (!(weakPoint.z <= 0.0f || 1.0f <= weakPoint.z))
		{
			float raitio = (float)hp_ / (float)maxHp_;
			if (raitio > 0.5)
			{
				green_ = STONE::GREEN + (int)(raitio * STONE::ADD_RGB);
				red_ = STONE::RED + (int)((1.0f - raitio) * STONE::ADD_RGB);
			}
			else if (raitio == 0.5)
			{
				green_ = STONE::GREEN + (int)(raitio * STONE::ADD_RGB);
				red_ = STONE::RED + (int)(raitio * STONE::ADD_RGB);
			}
			else
			{
				green_ = STONE::GREEN + (int)(raitio * STONE::ADD_RGB);
				red_ = STONE::RED + (int)((1.0f - raitio) * STONE::ADD_RGB);
			}
			weakPointColor_ = GetColor(red_, green_, blue_);

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawCircle((int)weakPoint.x, (int)weakPoint.y, STONE::WEAK_POINT_CIRCLE_R, weakPointColor_, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}

void Stone::DevelopmentInput()
{
}
