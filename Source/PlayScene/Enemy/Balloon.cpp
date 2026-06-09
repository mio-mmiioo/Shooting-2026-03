#include "Balloon.h"
#include "../../../MyLibrary/Collision.h"
#include "../Camera/PerlinNoise.h"
#include "../../../ImGui/imgui.h"

// 目標：空中を浮遊させること
namespace BALLOON
{
	float FLOATING_INTENSITY_Y = 40.0f; // 加算する浮遊の量
}

Balloon::Balloon(Data::ObjectData objectData, Data::CharacterData characterData)
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

	objectNumber_ = OBJECT_SORT::OBJ_CHARA;


	Collision::AddObject(this);


	noise_ = 0.0f;
	isArrive_ = true;
}

Balloon::~Balloon()
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
}

void Balloon::Update()
{
	// 体力が0の場合の処理
	if (hp_ <= 0)
	{
		PlaySoundMem(Data::se["breakEnemy"], DX_PLAYTYPE_BACK, TRUE);
		Enemy::SetObserver("balloon", true);
		Collision::DeleteObject(this);
		DestroyMe();
		return;
	}
	// 前回のHPより減っている場合ダメージの音を鳴らす
	if (prevHp_ > hp_)
	{
		PlaySoundMem(Data::se["attackEnemy"], DX_PLAYTYPE_BACK, TRUE);
	}

	//// 重力を加算
	//transform_.position_.y -= velocityY_;
	//velocityY_ += gravity_;

	// 1 ランダムで一定の値を出す
	// 2 1で出した値をもとに場所をずらしていく
	// 3 ずらしたい場所までずらせたら、1に戻る
	if (isArrive_ == true)
	{
		noise_ = PerlinNoise::Noise(transform_.position_);
		noise_ -= 0.5f; // 値を　-0.5f ~ 0.5f にする
		toPosition_ = VECTOR3(0.0f, noise_ * BALLOON::FLOATING_INTENSITY_Y, 0.0f) + transform_.position_;
		isArrive_ = false;
	}
	else
	{
		SetMove(toPosition_);
	}
	
	ImGui::Text("noise::%f", noise_);

	float distance = VSize(transform_.position_ - toPosition_);

	if (distance < 1.0f)
	{
		isArrive_ = true;
	}

	

	prevHp_ = hp_;
}

void Balloon::Draw()
{
	Object3D::Draw();
}

void Balloon::DevelopmentInput()
{
}
