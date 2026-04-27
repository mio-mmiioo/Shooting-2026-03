#include "Touhu.h"
#include "../../../MyLibrary/Collision.h"
#include "../../Data.h"
//#include "../Stage/WayInfo.h"
#include "../Stage/StageSearch.h"

namespace TOUHU
{
	const VECTOR3 ADD_WEAK_POSITION = { 0.0f, 150.0f, 0.0f }; // 加算する弱点の場所

	// 攻撃関連
	const float STAY_TIME = 6.0f;
	const int ATTACK_POWER = 2;

	// 開発時のみ使用する
	const float DIRECTION_LENGTH = 100.0f;
}

Touhu::Touhu(Data::ObjectData objectData, Data::CharacterData characterData)
{
	// Stoneと全く同じ、他にいい書き方があるかもしれない
	{
		hModel_ = MV1DuplicateModel(Data::models[objectData.name]);
		hitModel_ = MV1DuplicateModel(Data::models[objectData.name + "_c"]);

		transform_ = objectData.t;
		hp_ = characterData.hp;
		prevHp_ = hp_;
		score_ = characterData.score;

		transform_.MakeLocalMatrix();
		MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
		MV1SetupCollInfo(hitModel_);

		gravity_ = characterData.gravity;
		moveSpeed_ = characterData.moveSpeed;
		rotateSpeed_ = characterData.rotateSpeed;
		distanceR_ = characterData.distanceR;
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
		MV1DeleteModel(hModel_);
		hModel_ = -1;
	}
	if (hitModel_ > 0)
	{
		MV1DeleteModel(hitModel_);
		hitModel_ = -1;
	}
}

void Touhu::Update()
{
	// 体力が0の場合
	if (hp_ <= 0)
	{
		PlaySoundMem(Data::se["breakEnemy"], DX_PLAYTYPE_BACK, TRUE);
		Enemy::SetObserver("touhu", true);
		Collision::DeleteObject(this);
		DestroyMe();
		return;
	}
	// 前回のHPより減っている場合ダメージの音を鳴らす
	if (prevHp_ > hp_)
	{
		PlaySoundMem(Data::se["attackEnemy"], DX_PLAYTYPE_BACK, TRUE);
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

	prevHp_ = hp_;
}

void Touhu::Draw()
{
	Object3D::Draw();

	// 向いている方向
	//DrawLine3D(transform_.position_ + LOOK_HEIGHT,
		//transform_.position_ + LOOK_HEIGHT + VECTOR3(0, 0, 1) * TOUHU::DIRECTION_LENGTH * MGetRotY(transform_.rotation_.y), Color::BLACK);

	// 撃ってほしい部分を半透明の〇で表示する if文の条件と〇のサイズで不具合発生中
	{
		//VECTOR3 hit;
		//// 敵自身とプレイヤーの直線距離に障害物がないことを確認する
		//if (Collision::CheckLineHitObjectA(transform_.position_, Enemy::GetPlayerPosition() + LOOK_HEIGHT) == false)
		//{
		//	VECTOR3 weakPoint = ConvWorldPosToScreenPos(transform_.position_ + TOUHU::ADD_WEAK_POSITION); // ワールドスクリーン座標変換をする
		//	Enemy::DrawWeakPoint("touhu", hp_, weakPoint);
		//}
	}
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
		distance = Data::characterDataList["touhu"].distanceCurrentAndGo;
		if (Collision::CheckDistanceVertexAndVertex(e, goPosition_, distance) == true)
		{
			isArrive_ = true;
		}
	}

	distance = Data::characterDataList["touhu"].distanceThisAndPlayer;
	// プレイヤーとの距離が攻撃できる距離の場合、状態を攻撃に遷移させる
	if (Collision::CheckDistanceVertexAndVertex(e, p, distance) == true)
	{
		state_ = TOUHU_STATE::ATTACK;
	}

	if (isArrive_ == false)
	{
		VECTOR3 hit;
		if (Collision::CheckLineHitObjectA(transform_.position_, p + LOOK_HEIGHT) == true)
		{
			SetMove(goPosition_);
		}
		else
		{
			SetMove(p);
		}

	}
}

void Touhu::StayUpdate()
{
	// プレイヤーとの距離が開いた場合、状態を移動にする
	{
		VECTOR3 p = Enemy::GetPlayerPosition();
		VECTOR3 e = transform_.position_;
		float distance = Data::characterDataList["touhu"].distanceThisAndPlayer;
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
