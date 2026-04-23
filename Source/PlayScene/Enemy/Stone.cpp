#include "Stone.h"
#include "../../../MyLibrary/Collision.h"
#include "../../../MyLibrary/Observer.h"

namespace STONE
{
	const VECTOR3 ADD_WEAK_POSITION = { 0.0f, 50.0f, 0.0f }; // 加算する弱点の場所
}

Stone::Stone(Data::ObjectData objectData, Data::EnemyData enemyData)
{
	hModel_ = MV1DuplicateModel(Data::models[objectData.name]);
	hitModel_ = MV1DuplicateModel(Data::models[objectData.name + "_c"]);

	transform_ = objectData.t;
	hp_ = enemyData.hp;
	prevHp_ = hp_;
	score_ = enemyData.score;

	transform_.MakeLocalMatrix();
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1SetupCollInfo(hitModel_);

	gravity_ = enemyData.gravity;
	moveSpeed_ = enemyData.moveSpeed;
	rotateSpeed_ = enemyData.rotateSpeed;
	distanceR_ = enemyData.distanceR;

	objectNumber_ = OBJECT_SORT::OBJ_CHARA;
	

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
		PlaySoundMem(Data::se["breakEnemy"], DX_PLAYTYPE_BACK, TRUE);
		Enemy::SetObserver("stone", false);
		Collision::DeleteObject(this);
		DestroyMe();
		return;
	}
	// 前回のHPより減っている場合ダメージの音を鳴らす
	if (prevHp_ > hp_)
	{
		PlaySoundMem(Data::se["attackEnemy"], DX_PLAYTYPE_BACK, TRUE);
	}
	prevHp_ = hp_;
}

void Stone::Draw()
{
	Object3D::Draw();
	
	// 撃ってほしい部分を半透明の〇で表示する
	{
		VECTOR3 hit;
		// 敵自身とプレイヤーの直線距離に障害物がないことを確認する
		if (Collision::CheckLineHitObject(transform_.position_, Enemy::GetPlayerPosition() + LOOK_HEIGHT, &hit) == false)
		{
			VECTOR3 weakPoint = ConvWorldPosToScreenPos(transform_.position_ + STONE::ADD_WEAK_POSITION); // ワールドスクリーン座標変換をする
			Enemy::DrawWeakPoint("stone", hp_, weakPoint);
		}
	}
}

void Stone::DevelopmentInput()
{
}
