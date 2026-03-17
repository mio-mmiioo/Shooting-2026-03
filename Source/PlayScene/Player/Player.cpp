#include "Player.h"
#include <assert.h>
#include "../Camera/Camera.h"
#include "../../../MyLibrary/Color.h"
#include "../../../MyLibrary/Input.h"
#include "../../../MyLibrary/Light.h"
#include "../../../MyLibrary/Collision.h"
#include "../../../ImGui/imgui.h"

namespace PLAYER
{
	const float ROTATE_SPEED = 3.0f;	// 回転速度
	const float MOVE_SPEED = 5.0f;		// 移動速度

	const float DISTANCE_R = 100.0f; // 当たり判定の半径
	const float GRAVITY = 0.05f;
	
	// 開発時のみ
	const float DIRECTION_LENGTH = 100.0f;
}

Player::Player(const VECTOR3& position, int hp)
{
	objectNumber_ = OBJECT_SORT::OBJ_PLAYER;
	transform_.position_ = position;
	hp_ = hp;
	hModel_ = MV1LoadModel("data/model/player.mv1");
	assert(hModel_ > 0);
	hitModel_ = MV1LoadModel("data/model/player_c.mv1");
	assert(hitModel_ > 0);

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
	currentGunType_ = GUN::TYPE::HAND;
	gun_->SetGunType(currentGunType_); // 使用する銃の種類をセット

	phaseCount_ = 0;
	phasePosition_ = Data::GetPlayerNextPosition(phaseCount_);

	Collision::AddObject(this);

	SetDrawOrder(-100);
}

Player::~Player()
{
	Collision::DeleteObject(this);

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

	// 重力を加える
	transform_.position_.y -= velocityY_;
	velocityY_ += gravity_;

	// 移動処理
	AutoMove();
	DevelopmentInput();

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
	if (VSize(phasePosition_ - transform_.position_) > 100.0f)
	{
		Object3D::SetMove(phasePosition_);
	}
	if (ImGui::Button("nextPosition"))
	{
		phaseCount_ += 1;
		phasePosition_ = Data::GetPlayerNextPosition(phaseCount_);

		VECTOR3 toGo = Data::GetPlayerNextPosition(phaseCount_ + 1) - transform_.position_;
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
		transform_.position_ += VECTOR3(0, 0, moveSpeed_) * MGetRotY(transform_.rotation_.y);
	}
	ImGui::InputInt("phase count", &phaseCount_);
}
