#include "Player.h"
#include <assert.h>
#include "PlayerHp.h"
#include "../Camera/Camera.h"
#include "../GameMaster.h"
#include "../../../MyLibrary/Color.h"
#include "../../../MyLibrary/Input.h"
#include "../../../MyLibrary/Light.h"
#include "../../../MyLibrary/Collision.h"
#include "../../../ImGui/imgui.h"

namespace PLAYER
{
	const float ROTATE_SPEED = 0.005f;	// 回転速度
	const float MOVE_SPEED = 1.0f;		// 移動速度

	const float DISTANCE_R = 100.0f; // 当たり判定の半径
	const float GRAVITY = 0.05f;
	
	// 開発時のみ
	const float DIRECTION_LENGTH = 100.0f;
}

Player::Player(Data::ObjectData objectData)
{
	objectNumber_ = OBJECT_SORT::OBJ_PLAYER;
	transform_.position_ = objectData.t.position_;
	hp_ = objectData.hp;

	hModel_ = MV1DuplicateModel(Data::models[objectData.name]);
	hitModel_ = MV1DuplicateModel(Data::models[objectData.name + "_c"]);

	transform_.MakeLocalMatrix();
	MV1SetupCollInfo(hModel_);

	// 照準(aiming)
	Data::SetImage("aiming", &aiming_);
	Data::SetImage("hitAiming", &hitAiming_);
	Data::SetImage("reload", &reload_);

	rotateSpeed_ = PLAYER::ROTATE_SPEED;
	moveSpeed_ = PLAYER::MOVE_SPEED;
	distanceR_ = PLAYER::DISTANCE_R;
	gravity_ = PLAYER::GRAVITY;
	velocityY_ = 0.0f;
	
	camera_ = FindGameObject<Camera>();
	gun_ = new Gun();
	playerHp_ = new PlayerHp(objectData.hp);

	currentGunType_ = GUN::TYPE::HAND;
	gun_->SetGunType(currentGunType_); // 使用する銃の種類をセット
	isHit_ = false;

	// プレイヤーの移動フェーズ関連
	int count = GameMaster::GetPlayerPhaseCount();
	Data::GetPlayerPhase(count, &phaseData_);
	Data::GetPlayerPhase(count + 1, &nextPhaseData_);
	state_ = phaseData_.state;
	currentRotationY_ = 0.0f;

	Collision::AddObject(this);

	SetDrawOrder(-100);
}

Player::~Player()
{
	Collision::DeleteObject(this);
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

void Player::Update()
{
	// リロード
	if (Input::IsKeyDown("reload"))
	{
		gun_->ReloadBullet();
	}

	// 照準の当たり判定
	{
		VECTOR ScreenPosition = Input::GetMousePosition(); // { x座標, y座標, 1.0f }が代入される
		wPointerPosition_ = ConvScreenPosToWorldPos(ScreenPosition);
		startPosition_ = transform_.position_ + LOOK_HEIGHT;
		VECTOR3 hit;
		if (Collision::CheckBulletLineHitObject(startPosition_, wPointerPosition_, &hit) == true)
		{
			// 銃弾が、敵or破壊可能オブジェクトにあたる
			isHit_ = true;
		}
		else
		{
			isHit_ = false;
		}
	}

	// 発砲
	if (Input::IsKeyDown("outBullet") && isHit_ == true)
	{
		if (gun_->OutBullet() == true)
		{
			// 銃弾がヒットするオブジェクトを攻撃する(HPを減らす)
			Object3D* attackObject = Collision::GetHitObject();
			if (attackObject != nullptr)
			{
				attackObject->AddHp(-gun_->GetAttack());
			}
		}
	}

	switch (state_)
	{
	case Data::P_STATE::MOVE: // 移動処理
		AutoMove();
		break;
	case Data::P_STATE::STAY:
		// 指定した条件をクリアした場合、状態をMOVEに変える
		break;

	}

	DevelopmentInput();

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

	camera_->SetPlayerPosition(transform_);
	Light::SetPosition(transform_.position_);
	playerHp_->Update();

	// 位置情報の更新
	transform_.MakeLocalMatrix();
	MV1SetMatrix(hModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hModel_);
}

void Player::Draw()
{
	Object3D::Draw();

	// 向いている方向を示す
	VECTOR3 addPlayerHeight = LOOK_HEIGHT * transform_.GetRotationMatrix();
	DrawLine3D(transform_.position_ + addPlayerHeight, transform_.position_ + addPlayerHeight + VECTOR3(0, 0, 1) * PLAYER::DIRECTION_LENGTH * transform_.GetRotationMatrix(), Color::WHITE);

	// 2Dの描画
	playerHp_->Draw();

	// 照準の描画
	int x = (int)Input::GetMousePosition().x;
	int y = (int)Input::GetMousePosition().y;
	if (isHit_ == true)
	{
		DrawGraph(x - hitAiming_.halfWidth, y - hitAiming_.halfHeight, hitAiming_.hImage, TRUE);
	}
	else
	{
		DrawGraph(x - aiming_.halfWidth, y - aiming_.halfHeight, aiming_.hImage, TRUE);
	}
}

void Player::AddHp(int add)
{
	playerHp_->AddHp(add, &hp_);
}

void Player::DevelopmentInput()
{
	// ImGuiでの入力
	{
		Transform t = transform_;
		ImGui::Begin("Player");
		ImGui::Text("position");
		float p[3] = { t.position_.x, t.position_.y, t.position_.z };
		ImGui::SliderFloat3("position", p, 0.0f, 10000.0f);

		ImGui::Text("rotation");
		float r[3] = { t.rotation_.x, t.rotation_.y, t.rotation_.z };
		ImGui::SliderFloat3("rotation", r, -DX_PI_F, DX_PI_F);

		// hp関連
		int damage = 2;
		ImGui::InputInt("Damage Power", &damage);
		ImGui::Text("hp:%d", hp_);
		if (ImGui::Button("DamagedPlayer"))
		{
			playerHp_->AddHp(-damage, &hp_);
		}

		// PhaseData関連
		{
			// 次に向かう場所(Position)
			ImGui::Text("PhasePosition.x : %04f", phaseData_.position.x);
			ImGui::Text("PhasePosition.y : %04f", phaseData_.position.y);
			ImGui::Text("PhasePosition.z : %04f", phaseData_.position.z);

			// 状態
			if (phaseData_.state == Data::P_STATE::MOVE)
			{
				ImGui::Text("State : MOVE");
			}
			else if (phaseData_.state == Data::P_STATE::STAY)
			{
				ImGui::Text("State : STAY");
			}
		}

		ImGui::End();
		transform_.position_ = VECTOR3(p[0], p[1], p[2]);
		transform_.rotation_ = VECTOR3(r[0], r[1], r[2]);
	}

	// 入力回転
	{
		if (Input::IsKeyKeepDown("rotateRight"))
		{
			transform_.rotation_.y += rotateSpeed_ * DegToRad;
		}
		if (Input::IsKeyKeepDown("rotateLeft"))
		{
			transform_.rotation_.y -= rotateSpeed_ * DegToRad;
		}
	}

	// 入力移動
	{
		VECTOR3 velocity; // 移動ベクトル velocity→進行方向
		velocity = VECTOR3(0, 0, 1) * moveSpeed_ * MGetRotY(transform_.rotation_.y); // 移動方向を書いた後、移動距離、回転行列
		if (Input::IsKeyKeepDown("moveFront"))
		{
			transform_.position_ += velocity;
		}
		else if (Input::IsKeyKeepDown("moveBack"))
		{
			transform_.position_ -= velocity;
		}
	}
}

void Player::AutoMove()
{
	int phaseCount = GameMaster::GetPlayerPhaseCount();
	Data::GetPlayerPhase(phaseCount, &phaseData_);
	float distance = VSize(phaseData_.position - transform_.position_);

	if (distance >= phaseData_.distance1)
	{
		transform_.position_ += VECTOR3(0, 0, moveSpeed_) * MGetRotY(transform_.rotation_.y);
	}
	else if (distance < phaseData_.distance1)
	{
		float min = phaseData_.distance1;
		Data::GetPlayerPhase(phaseCount + 1, &nextPhaseData_);
		//float t = distance / min;
		//VECTOR3 position = phaseData_.position * t + nextPhaseData_.position * (1.0f - t);

		VECTOR3 toGo = nextPhaseData_.position - transform_.position_;
		VECTOR3 front = VECTOR3(0, 0, 1) * MGetRotY(transform_.rotation_.y); // 正面
		VECTOR3 right = VECTOR3(1, 0, 0) * MGetRotY(transform_.rotation_.y); // 右　回転の確認に使用

		if (VDot(front, toGo.Normalize()) >= cos(rotateSpeed_))
		{
			transform_.rotation_.y = atan2f(toGo.x, toGo.z);
		}
		else if (VDot(right, toGo) > 0)
		{
			transform_.rotation_.y += rotateSpeed_;
		}
		else
		{
			transform_.rotation_.y -= rotateSpeed_;
		}

		transform_.position_ += VECTOR3(0, 0, moveSpeed_) * MGetRotY(currentRotationY_);
		if (distance < phaseData_.distance2)
		{
			phaseCount = GameMaster::AddPhaseCount();
			Data::GetPlayerPhase(phaseCount, &phaseData_);
			state_ = phaseData_.state;
			currentRotationY_ = transform_.rotation_.y;
		}
	}
	if (ImGui::Button("nextPosition"))
	{
		int count = GameMaster::AddPhaseCount();
		Data::GetPlayerPhase(count, &phaseData_);
	}
}
