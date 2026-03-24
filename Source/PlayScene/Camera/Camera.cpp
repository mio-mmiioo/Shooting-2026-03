#include "Camera.h"
#include "../../../MyLibrary/Input.h"
#include "../../../ImGui/imgui.h"
#include "../../../MyLibrary/Collision.h"
#include "../GameMaster.h"
#include "../../Data.h"

namespace
{
	const float CAMERA_MOVE_SPEED = 5.0f;
	const float CAMERA_ROTATE_SPEED = 0.01f;
	const float CAMERA_NEAR = 50.0f;
	const float CAMERA_FAR = 15000.0f;

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
	fixAddPosition_ = VECTOR3(0.0f, 0.0f, 0.0f);
	prevPlayerPosition_ = VECTOR3(0.0f, 0.0f, 0.0f);
	FixUpdate();
	SetCameraPositionAndTarget_UpVecY(cameraPosition_, targetPosition_);
	state_ = CAM_STATE::FIX;
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);
	wheelRot_ = 0;
	isMoveFixCamera_ = true;
	transform_.position_ = cameraPosition_;
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
	ImGuiInput();

	// カメラの状態によって、セットする位置を調整
	switch (state_)
	{
	case CAM_STATE::FIRST:
		FirstUpdate();
		//ModifiedFirstUpdate();
		break;
	case CAM_STATE::THIRD:
		ThirdUpdate();
		//ModifiedThirdUpdate();
		break;
	case CAM_STATE::FIX:
		FixUpdate();
		break;
	}

	// カメラの位置をセット
	SetCameraPositionAndTarget_UpVecY(cameraPosition_, targetPosition_);
}

void Camera::SetPlayerPosition(Transform transform)
{
	player_ = transform;
}

void Camera::FirstUpdate()
{
	cameraPosition_ = player_.position_ + LOOK_HEIGHT; // 目線の高さに合わせている
	targetPosition_ = player_.position_ + LOOK_HEIGHT + VECTOR3(0, 0, 1) * FIRST_DISTANCE * MGetRotY(player_.rotation_.y);
}

void Camera::ModifiedFirstUpdate()
{
	int phaseCount = GameMaster::GetPlayerPhaseCount();
	Data::GetPlayerPhase(phaseCount, &phaseData_);
	float distance = VSize(phaseData_.position - transform_.position_);

	// プレイヤーが向かっている場所との距離が一定以内の場合
	if (distance < phaseData_.distance1)
	{
		// 次の場所と向かっている場所の距離を補間する
		float min = phaseData_.distance1;
		Data::GetPlayerPhase(phaseCount + 1, &nextPhaseData_);
		float t = distance / min;
		//VECTOR3 rotation = phaseData_.position * t + nextPhaseData_.position * (1.0f - t) + ;
		if (t > 0.9f)
		{
			Data::GetPlayerPhase(phaseCount, &phaseData_);
		}
	}

	// 移動処理
	{
		VECTOR3 toGo = player_.position_ + LOOK_HEIGHT - cameraPosition_;
		VECTOR3 front = VECTOR3(0, 0, 1) * MGetRotY(player_.rotation_.y); // 正面
		VECTOR3 right = VECTOR3(1, 0, 0) * MGetRotY(player_.rotation_.y); // 右　回転の確認に使用

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
		cameraPosition_ = player_.position_ + LOOK_HEIGHT + VECTOR3(0, 0, moveSpeed_) * MGetRotY(transform_.rotation_.y);
	}

	targetPosition_ = player_.position_ + LOOK_HEIGHT + VECTOR3(0, 0, 1) * FIRST_DISTANCE * MGetRotY(player_.rotation_.y);
}

void Camera::ThirdUpdate()
{
	int moveX = Input::GetMousePosition().x - prevMouseX_;
	int moveY = Input::GetMousePosition().y - prevMouseY_;

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

	prevMouseX_ = Input::GetMousePosition().x;
	prevMouseY_ = Input::GetMousePosition().y;
}

void Camera::ModifiedThirdUpdate()
{
	float moveX = player_.position_.x - prevPlayerPosition_.x;
	float moveY = player_.position_.y - prevPlayerPosition_.y;

	VECTOR3& rot = player_.rotation_;
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
	{
		VECTOR3 toGo = cameraPosition_ - prevPlayerPosition_;
		VECTOR3 front = VECTOR3(0, 0, 1) * MGetRotY(player_.rotation_.y); // 正面
		VECTOR3 right = VECTOR3(1, 0, 0) * MGetRotY(player_.rotation_.y); // 右　回転の確認に使用

		if (VDot(front, toGo.Normalize()) >= cos(rotateSpeed_))
		{
			player_.rotation_.y = atan2f(toGo.x, toGo.z);
		}
		else if (VDot(right, toGo) > 0)
		{
			player_.rotation_.y += rotateSpeed_;
		}
		else
		{
			player_.rotation_.y -= rotateSpeed_;
		}
		cameraPosition_ += VECTOR3(0, 0, moveSpeed_) * MGetRotY(player_.rotation_.y);
	}

	VECTOR3 hit;
	if (Collision::CheckLineHitObject(cameraPosition_, player_.position_, &hit))
	{
		cameraPosition_ = hit;
	}

	targetPosition_ = player_.position_ + ADD_HEIGHT;

	prevPlayerPosition_ = player_.position_;
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
