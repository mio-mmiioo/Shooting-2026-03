#include "Camera.h"
#include "../../../MyLibrary/Input.h"
#include "../../../ImGui/imgui.h"
#include "../../../MyLibrary/Collision.h"
#include "../../Data.h"
#include "../GameMaster.h"
#include "PerlinNoise.h"

namespace
{
	const float CAMERA_MOVE_SPEED = 5.0f; // 移動速度
	const float CAMERA_ROTATE_SPEED = 0.01f; // 回転速度
	const float CAMERA_NEAR = 50.0f;
	const float CAMERA_FAR = 15000.0f;

	// ヘッドボブ関連
	const float HEAD_BOB_TIME = 1.0f;
	const float HEAD_BOB_INTENSITY = 10.0f;

	// パーリンノイズ関連　自然に見えるように揺らす
	const float PERLIN_NOISE_INTENSITY = 10.0f;
	const float PERLIN_NOISE_TIME = 0.5f;

	// 一人称視点関連
	const float FIRST_DISTANCE = 300.0f; // 視点と注視点の距離

	// 三人称視点関連
	const float MAX_ROTATE_X = 80.0f;
	const float MIN_ROTATE_X = -25.0f;
	const VECTOR3 THIRD_BASE_POSITION = { 0.0f, 0.0f, -500.0f };
	const VECTOR3 ADD_HEIGHT = LOOK_HEIGHT + VECTOR3(0.0f, 20.0f, 0.0f); // プレイヤーの頭より少し上を見る

	// 固定視点関連
	const VECTOR3 FIX_BASE_POSITION = { 0.0f, 9000.0f, -1000.0f };
	const float ADD_DISTANCE = 100.0f;
}

Camera::Camera()
{
	GetMousePoint(&prevMouseX_, &prevMouseY_);
	fixAddPosition_ = VECTOR3(-5000.0f, 0.0f, 0.0f);
	prevPlayerPosition_ = VECTOR3(0.0f, 0.0f, 0.0f);
	FixUpdate();
	SetCameraPositionAndTarget_UpVecY(cameraPosition_, targetPosition_);
	state_ = CAM_STATE::FIRST;
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);
	wheelRot_ = 0;
	isMoveFixCamera_ = true;
	transform_.position_ = cameraPosition_;

	headBobTimer_ = HEAD_BOB_TIME;
	noiseTimer_ = PERLIN_NOISE_TIME;
	noise_ = PerlinNoise::Noise(player_.position_);
	isPositive_ = false;
}

Camera::~Camera()
{
}

void Camera::Update()
{
	// カメラ状態の切り替え
	if (Input::IsKeyDown("changeCamera"))
	{
		ChangeCamera();
	}

	if (GameMaster::GetIsDebug())
	{
		ImGuiInput();
	}

	// カメラの状態によって、セットする位置を調整
	switch (state_)
	{
	case CAM_STATE::FIRST:
		FirstUpdate();
		break;
	case CAM_STATE::THIRD:
		ThirdUpdate();
		break;
	case CAM_STATE::FIX:
		FixUpdate();
		break;
	}

	// カメラの位置をセット
	SetCameraPositionAndTarget_UpVecY(cameraPosition_, targetPosition_);
	prevPlayerPosition_ = player_.position_;
}

void Camera::SetPlayerPosition(Transform transform)
{
	player_ = transform;
}

void Camera::FirstUpdate()
{
	cameraPosition_ = player_.position_ + LOOK_HEIGHT; // 目線の高さに合わせている
	targetPosition_ = player_.position_ + LOOK_HEIGHT + VECTOR3(0, 0, 1) * FIRST_DISTANCE * MGetRotY(player_.rotation_.y);

	if (GameMaster::GetPlayerState() == Data::P_STATE::MOVE)
	{
		HeadBob(); // 歩いている風に見せる処理
		PerlinNoise(); // ランダムに揺らして、自然に見せる処理
	}
}

void Camera::ThirdUpdate()
{
	int moveX = (int)Input::GetMousePosition().x - prevMouseX_;
	int moveY = (int)Input::GetMousePosition().y - prevMouseY_;

	VECTOR3& rot = transform_.rotation_;
	rot.y += CAMERA_ROTATE_SPEED * moveX;
	rot.x -= CAMERA_ROTATE_SPEED * moveY;

	if (rot.x >= MAX_ROTATE_X * DegToRad)
	{
		rot.x = MAX_ROTATE_X * DegToRad;
	}
	if (rot.x < MIN_ROTATE_X * DegToRad)
	{
		rot.x = MIN_ROTATE_X * DegToRad;
	}

	VECTOR camPos = THIRD_BASE_POSITION * MGetRotX(rot.x) * MGetRotY(rot.y);
	cameraPosition_ = player_.position_ + camPos + LOOK_HEIGHT;
	targetPosition_ = player_.position_ + ADD_HEIGHT;

	prevMouseX_ = (int)Input::GetMousePosition().x;
	prevMouseY_ = (int)Input::GetMousePosition().y;
}

void Camera::FixUpdate()
{
	wheelRot_ += GetMouseWheelRotVol();

	// 移動処理 ImGuiInputに記述

	cameraPosition_ = FIX_BASE_POSITION + VECTOR3(0.0f, -(float)(wheelRot_ * ADD_DISTANCE), 0.0f) + fixAddPosition_;
	targetPosition_ = fixAddPosition_;
}

void Camera::ChangeCamera()
{
	switch (state_)
	{
	case CAM_STATE::FIRST:
		state_ = CAM_STATE::THIRD;
		break;
	case CAM_STATE::THIRD:
		state_ = CAM_STATE::FIX;
		break;
	case CAM_STATE::FIX:
		state_ = CAM_STATE::FIRST;
		break;
	}
}

void Camera::HeadBob()
{
	// タイマーを増加させる
	if (isPositive_ == true)
	{
		headBobTimer_ += Time::DeltaTime();
	}
	// タイマーを減少させる
	else
	{
		headBobTimer_ -= Time::DeltaTime();
	}
	float addHeadBob = sin(headBobTimer_ / HEAD_BOB_TIME * 2.0f - 1.0f) * HEAD_BOB_INTENSITY;
	cameraPosition_.y += addHeadBob;
	targetPosition_.y += addHeadBob;

	if (headBobTimer_ <= 0.0f)
	{
		isPositive_ = true;
	}
	else if (headBobTimer_ >= HEAD_BOB_TIME)
	{
		isPositive_ = false;
	}
}

void Camera::PerlinNoise()
{
	noiseTimer_ -= Time::DeltaTime();
	if (noiseTimer_ <= 0.0f)
	{
		noise_ = PerlinNoise::Noise(cameraPosition_);
		noiseTimer_ = PERLIN_NOISE_TIME;
	}

	VECTOR3 move = player_.position_ - prevPlayerPosition_;
	VECTOR3 normal = { -move.z, 0.0f, move.x }; // 法線ベクトル　成分を入れ替えて、片方の符号を反転
	normal = VNorm(normal);
	// カメラの位置　+= 正規化した方向ベクトル * (-0.5~0.5) * 定数 * ((最大の時間　- 時間) / 最大の時間)
	cameraPosition_ += normal * (noise_ - 0.5f) * PERLIN_NOISE_INTENSITY * ((PERLIN_NOISE_TIME - noiseTimer_) / PERLIN_NOISE_TIME);
}

void Camera::PerlinNoise2()
{
	
}

void Camera::ImGuiInput()
{
	static int state = 0;
	ImGui::Begin("Camera");

	// カメラの切り替え
	ImGui::RadioButton("First", &state, CAM_STATE::FIRST);
	ImGui::RadioButton("Third", &state, CAM_STATE::THIRD);
	ImGui::RadioButton("Fix", &state, CAM_STATE::FIX);

	VECTOR3 p = cameraPosition_;
	ImGui::Text("x : %04d, y : %04d, z : %04d", (int)p.x, (int)p.y, (int)p.z);
	ImGui::Text("noise : %f", noise_);
	switch (state)
	{
	case CAM_STATE::FIRST:
		state_ = CAM_STATE::FIRST;
		break;
	case CAM_STATE::THIRD:
		state_ = CAM_STATE::THIRD;
		break;
	case CAM_STATE::FIX:
		state_ = CAM_STATE::FIX;

		int x = (int)fixAddPosition_.x;
		int y = (int)fixAddPosition_.y;
		int z = (int)fixAddPosition_.z;

		ImGui::InputInt("x", &x);
		ImGui::InputInt("y", &y);
		ImGui::InputInt("z", &z);

		fixAddPosition_ = VECTOR3((float)x, (float)y, (float)z);

		break;
	}
	ImGui::End();
}
