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
	SetAimingImage(aiming_, "data/image/pointer1.png");
	SetAimingImage(hitAiming_, "data/image/pointer2.png");
	SetAimingImage(reload_, "data/image/reload.png");

	rotateSpeed_ = PLAYER::ROTATE_SPEED;
	moveSpeed_ = PLAYER::MOVE_SPEED;
	distanceR_ = PLAYER::DISTANCE_R;
	gravity_ = PLAYER::GRAVITY;
	velocityY_ = 0.0f;
	

	camera_ = FindGameObject<Camera>();
	gun_ = new Gun();
	currentGunType_ = GUN::TYPE::HAND;
	gun_->SetGunType(currentGunType_); // 使用する銃の種類をセット

	Collision::AddObject(this);

	SetDrawOrder(-100);
}

Player::~Player()
{
	Collision::DeleteObject(this);

}

void Player::Update()
{
	GetMousePoint(&mouseX_, &mouseY_);

	// 移動処理
	DevelopmentInput();

	// リロード
	if (Input::IsKeyDown("reload"))
	{
		gun_->ReloadBullet();
	}

	// 照準の当たり判定
	{
		VECTOR ScreenPosition = { (float)mouseX_,(float)mouseY_, 1.0f };
		wPointerPosition_ = ConvScreenPosToWorldPos(ScreenPosition);
		startPosition_ = transform_.position_ + LOOK_HEIGHT;
		VECTOR3 hit;
		if (Collision::CheckLineHitObject(startPosition_, wPointerPosition_, &hit) == true)
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

	// 各オブジェクトとの距離を確認し、めり込みをなくす
	{
		VECTOR3 currentPosition = transform_.position_;
		transform_.position_ = Collision::CheckOnGround(this);
		transform_.position_ = Collision::CheckPushObject(this);

		// ここに入らない
		if (VSize(currentPosition - transform_.position_) <= 0)
		{
			velocityY_ = 0.0f;
		}
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
	if (isHit_ == true)
	{
		DrawGraph(mouseX_ - hitAiming_.halfWidth, mouseY_ - hitAiming_.halfHeight, hitAiming_.hImage, TRUE);
	}
	else
	{
		DrawGraph(mouseX_ - aiming_.halfWidth, mouseY_ - aiming_.halfHeight, aiming_.hImage, TRUE);
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

void Player::SetAimingImage(image& i, std::string path)
{
	i.hImage = LoadGraph(path.c_str());
	assert(i.hImage > 0);
	GetGraphSize(i.hImage, &i.halfWidth, &i.halfHeight);
	i.halfWidth = i.halfWidth / 2;
	i.halfHeight = i.halfHeight / 2;
}
