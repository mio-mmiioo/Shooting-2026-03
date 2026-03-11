#include "Camera.h"
#include "../../../MyLibrary/Input.h"
#include "../../../ImGui/imgui.h"

namespace
{
	const float CAMERA_MOVE_SPEED = 5.0f;
	const float CAMERA_ROTATE_SPEED = 0.01f;
	const float CAMERA_NEAR = 50.0f;
	const float CAMERA_FAR = 15000.0f;

	// 一人称視点関連
	const float FIRST_DISTANCE = 1000.0f; // 視点と注視点の距離

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
	FixUpdate();
	SetCameraPositionAndTarget_UpVecY(cameraPosition_, targetPosition_);
	state_ = CAM_STATE::FIX;
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);
	wheelRot_ = 0;
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

void Camera::ThirdUpdate()
{
	GetMousePoint(&mouseX_, &mouseY_);
	int moveX = mouseX_ - prevMouseX_;
	int moveY = mouseY_ - prevMouseY_;

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

	prevMouseX_ = mouseX_;
	prevMouseY_ = mouseY_;
}

void Camera::FixUpdate()
{
	wheelRot_ += GetMouseWheelRotVol();

	// 移動処理
	{

	}

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
		break;
	}
	ImGui::End();
}
