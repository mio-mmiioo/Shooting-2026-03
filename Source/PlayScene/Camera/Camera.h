#pragma once
#include "../../../MyLibrary/Object3D.h"
#include "../../Data.h"

enum CAM_STATE
{
	FIRST,	// 一人称
	THIRD,	// 三人称
	FIX,	// 固定
	MAX_CAM_STATE
};

class Camera : public Object3D
{
public:
	Camera();
	~Camera();
	void Update() override;
	void SetPlayerPosition(Transform transform);

private:
	void FirstUpdate();		// 一人称カメラの更新処理
	void ThirdUpdate();		// 三人称カメラの更新処理
	void FixUpdate();		// 固定カメラの更新処理
	void ChangeCamera();	// カメラの切り替え処理

	void HeadBob();			// 歩いている風に見せる処理
	void PerlinNoise();		// 自然に見せるための処理

	void ImGuiInput();		// 開発時のみ使用する

	Transform player_;			 // プレイヤーの情報
	VECTOR3 prevPlayerPosition_; // 前回のプレイヤーの位置
	int mouseX_;				 // マウスのx座標
	int mouseY_;				 // マウスのy座標
	int prevMouseX_;			 // 前のマウスのx座標
	int prevMouseY_;			 // 前のマウスのy座標

	CAM_STATE state_; // カメラの状態

	VECTOR3 cameraPosition_; // 視点の位置
	VECTOR3 targetPosition_; // 注視点の位置
	VECTOR3 fixAddPosition_; // 固定カメラの時に使用する、加算された位置

	// カメラを上下左右に動かす
	float headBobTimer_;	// ヘッドボブ
	float noiseTimer_;		// パーリンノイズ
	float noise_;			// パーリンノイズで取得した、ランダムな 0 ~ 1 の値
	bool isPositive_;		// 正の数か

	int wheelRot_; // マウスホイール
	bool isMoveFixCamera_; // 固定カメラの移動フラグ
};
