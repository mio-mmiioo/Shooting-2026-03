#include "Touhu.h"
#include "../../../MyLibrary/Collision.h"
#include "../../../MyLibrary/Animator.h"
#include "../../../ImGui/imgui.h"
#include "../../Data.h"
#include "../Stage/StageSearch.h"

#include <assert.h>

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

	// Animatorの設定
	{
		animator_ = new Animator(hModel_);
		assert(animator_ != nullptr);

		std::string folder = "data/model/Anim/" + objectData.name;
		int root = MV1SearchFrame(hModel_, "root");
		MV1SetFrameUserLocalMatrix(hModel_, root, MGetRotY(DX_PI_F));

		animator_->AddFile(A_NEUTRAL, folder + "/Anim_Neutral.mv1", true);
		animator_->AddFile(A_WALK,	  folder + "/Anim_Walk.mv1",	true);
		animator_->AddFile(A_ATTACK1, folder + "/Anim_Attack1.mv1", false);
		animator_->AddFile(A_DOWN,	  folder + "/Anim_Down.mv1",	false);
	}
	goPosition_ = transform_.position_;
	state_ = TOUHU_STATE::STAY;
	isArrive_ = true;
	stateTimer_ = 0.0f;
}

Touhu::~Touhu()
{
	if (animator_ != nullptr)
	{
		delete animator_;
		animator_ = nullptr;
	}

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
	animator_->Update();

	// 前回のHPより減っている場合ダメージの音を鳴らす
	if (prevHp_ > hp_ && hp_ >= 0)
	{
		if (hp_ <= 0)
		{
			PlaySoundMem(Data::se["breakEnemy"], DX_PLAYTYPE_BACK, TRUE);
			Collision::DeleteObject(this); // 倒された後は、他のキャラの行動の妨げにならないようにする
			prevHp_ = hp_;
		}
		else
		{
			PlaySoundMem(Data::se["attackEnemy"], DX_PLAYTYPE_BACK, TRUE);
		}
	}

	// 体力が0の場合
	if (hp_ <= 0)
	{
		DownUpdate();
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

	prevHp_ = hp_;
}

void Touhu::Draw()
{
	Object3D::Draw();

	// 向いている方向
	//DrawLine3D(transform_.position_ + LOOK_HEIGHT,
	//	transform_.position_ + LOOK_HEIGHT + VECTOR3(0, 0, 1) * TOUHU::DIRECTION_LENGTH * MGetRotY(transform_.rotation_.y), GetColor(255,255,255));
}

void Touhu::DevelopmentInput()
{
}

void Touhu::WalkUpdate()
{
	animator_->Play(A_WALK);

	VECTOR3 e = transform_.position_; // 自身の場所
	VECTOR3 p = Enemy::GetPlayerPosition();	// プレイヤーの場所
	float distance;
	// 目的地に到着している場合、次に向かうべき場所を取得する
	if (isArrive_ == true)
	{
		goPosition_ = StageSearch::GetShortestWayPosition(e, p); // プレイヤーに最短経路で向かう座標を取得
		isArrive_ = false;
	}
	// 目的地に到着していない場合、目的地に到着したか確認する
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

	// 移動処理
	if (isArrive_ == false)
	{
		VECTOR3 hit;
		// プレイヤーと自身の間に障害物がある場合
		if (Collision::CheckLineHitObjectA(transform_.position_, p + LOOK_HEIGHT) == true)
		{
			SetMove(goPosition_); // プレイヤーに最短経路で向かう座標へ移動
		}
		else
		{
			SetMove(p); // プレイヤーに向かって直進する
		}

	}

	ImGui::Text("hp:%d", hp_);
}

void Touhu::StayUpdate()
{
	animator_->Play(A_NEUTRAL);

	// プレイヤーとの距離が開いた場合、状態をWALKにする
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
	// プレイヤーの付近で、一定時間経過した場合、状態をATTACKにする
	if (stateTimer_ > TOUHU::STAY_TIME)
	{
		stateTimer_ -= TOUHU::STAY_TIME;
		state_ = TOUHU_STATE::ATTACK;
	}
}

void Touhu::AttackUpdate()
{
	animator_->Play(A_ATTACK1);

	// 一回攻撃をしたらSTAYに戻る
	Enemy::AttackPlayer(TOUHU::ATTACK_POWER);
	state_ = TOUHU_STATE::STAY; 
}

void Touhu::DownUpdate()
{
	animator_->Play(A_DOWN);

	if (animator_->IsFinish() == true)
	{
		Enemy::SetObserver("touhu", true);
		DestroyMe();
	}
}
