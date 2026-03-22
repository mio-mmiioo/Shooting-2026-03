#include "Touhu.h"
#include <assert.h>
#include "../../../MyLibrary/Color.h"
#include "../../../MyLibrary/Collision.h"
#include "../../Data.h"
//#include "../Stage/WayInfo.h"
#include "../Stage/StageSearch.h"

namespace TOUHU
{
	// 攻撃関連
	const float STAY_TIME = 6.0f;
	const int ATTACK_POWER = 2;

	// 開発時のみ使用する
	const float DIRECTION_LENGTH = 100.0f;
}

Touhu::Touhu(Data::ObjectData objectData, Data::EnemyData enemyData)
{
	// Stoneと全く同じ、他にいい書き方があるかもしれない
	{
		//const std::string folder = "data/model/";
		//hModel_ = MV1LoadModel((folder + objectData.path + ".mv1").c_str());
		//assert(hModel_ > 0);
		//hitModel_ = MV1LoadModel((folder + objectData.path + ".mv1").c_str());
		//assert(hModel_ > 0);
		hModel_ = Data::models[objectData.name];
		hitModel_ = Data::models[objectData.name + "_c"];

		transform_ = objectData.t;
		hp_ = enemyData.hp;
		score_ = enemyData.score;

		transform_.MakeLocalMatrix();
		MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
		MV1SetupCollInfo(hitModel_);

		gravity_ = enemyData.gravity;
		moveSpeed_ = enemyData.moveSpeed;
		rotateSpeed_ = enemyData.rotateSpeed;
		distanceR_ = enemyData.distanceR;
		velocityY_ = 0.0f;

		objectNumber_ = OBJECT_SORT::OBJ_CHARA;
		
		Collision::AddObject(this);
	}
	goPosition_ = transform_.position_;
	state_ = TOUHU_STATE::STAY;
	isArrive_ = true;
	stateTimer_ = 0.0f;
}

Touhu::~Touhu()
{
	if (hModel_ > 0)
	{
		hModel_ = -1;
	}
	if (hitModel_ > 0)
	{
		hitModel_ = -1;
	}
}

void Touhu::Update()
{
	// 体力が0の場合
	if (hp_ <= 0)
	{
		Enemy::SetObserver("touhu", true);
		Collision::DeleteObject(this);
		DestroyMe();
		return;
	}

	// 状態によって更新処理を行う
	switch (state_)
	{
	case TOUHU_STATE::STAY:
		StayUpdate();
		break;
	case TOUHU_STATE::ATTACK:
		AttackUpdate();
		break;
	case TOUHU_STATE::WALK:
		WalkUpdate();
		break;
	}

	// 重力を加える
	transform_.position_.y -= velocityY_;
	velocityY_ += gravity_;

	// 各オブジェクトとの距離を確認し、めり込みをなくす
	{
		VECTOR3 currentPosition = transform_.position_;
		transform_.position_ = Collision::CheckOnGround(this, &isOnGround_); // 床とのめり込みをチェックする
		if (isOnGround_ == true)
		{
			velocityY_ = 0.0f;
		}
		transform_.position_ = Collision::CheckPushObject(this);
	}

	// 位置情報の更新
	transform_.MakeLocalMatrix();
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hitModel_);
}

void Touhu::Draw()
{
	Object3D::Draw();

	// 向いている方向
	DrawLine3D(transform_.position_ + LOOK_HEIGHT,
		transform_.position_ + LOOK_HEIGHT + VECTOR3(0, 0, 1) * TOUHU::DIRECTION_LENGTH * MGetRotY(transform_.rotation_.y), Color::BLACK);
}

void Touhu::DevelopmentInput()
{
}

void Touhu::WalkUpdate()
{
	VECTOR3 e = transform_.position_; // 自身の場所
	VECTOR3 p = Enemy::GetPlayerPosition();	// プレイヤーの場所
	float distance;
	if (isArrive_ == true)
	{
		//goPosition_ = WayInfo::GetShortestWayPosition(e, p);
		goPosition_ = StageSearch::GetShortestWayPosition(e, p);
		isArrive_ = false;
	}
	else
	{
		distance = Data::enemyDataList["touhu"].distanceCurrentAndGo;
		if (Collision::CheckDistanceVertexAndVertex(e, goPosition_, distance) == true)
		{
			isArrive_ = true;
		}
	}

	distance = Data::enemyDataList["touhu"].distanceThisAndPlayer;
	// プレイヤーとの距離が攻撃できる距離の場合、状態を攻撃に遷移させる
	if (Collision::CheckDistanceVertexAndVertex(e, p, distance) == true)
	{
		state_ = TOUHU_STATE::ATTACK;
	}

	if (isArrive_ == false)
	{
		SetMove(goPosition_);
	}
}

void Touhu::StayUpdate()
{
	// プレイヤーとの距離が開いた場合、状態を移動にする
	{
		VECTOR3 p = Enemy::GetPlayerPosition();
		VECTOR3 e = transform_.position_;
		float distance = Data::enemyDataList["touhu"].distanceThisAndPlayer;
		if (Collision::CheckDistanceVertexAndVertex(e, p, distance) == false)
		{
			state_ = TOUHU_STATE::WALK;
		}
	}

	stateTimer_ += Time::DeltaTime();
	if (stateTimer_ > TOUHU::STAY_TIME)
	{
		stateTimer_ -= TOUHU::STAY_TIME;
		state_ = TOUHU_STATE::ATTACK;
	}
}

void Touhu::AttackUpdate()
{
	// 一回攻撃をしたらSTAYに戻る
	Enemy::AttackPlayer(TOUHU::ATTACK_POWER);
	state_ = TOUHU_STATE::STAY; 
}
