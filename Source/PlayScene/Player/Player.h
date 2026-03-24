#pragma once
#include "../../../MyLibrary/Object3D.h"
#include "../../Data.h"
#include "Gun.h"

class Camera;
class PlayerHp;

class Player : public Object3D
{
public:
	Player(Data::ObjectData o);
	~Player() override;
	void Update() override;
	void Draw() override;
	void AddHp(int add) override; // 体力を増減させる
	int GetHp() { return hp_; } // 体力を返す

private:
	void DevelopmentInput();
	void AutoMove();

	Camera* camera_;		// カメラ関連
	Gun* gun_;				// 銃弾関連
	PlayerHp* playerHp_;	// hpバーの表示関連

	VECTOR3 wPointerPosition_;	// スクリーン座標の照準をワールド座標に変換した物を代入する変数
	VECTOR3 startPosition_;		// 銃弾の軌道の始点
	
	GUN::TYPE currentGunType_; // 現在の銃の種類
	bool isHit_; // true→銃弾が当たる　照準の画像を切り替えるときに使用

	// 照準の画像関連
	Data::image aiming_;		// 照準の画像
	Data::image hitAiming_;		// 照準が何かにあたるときの画像
	Data::image reload_;		// リロードの画像


	Data::PlayerPhase phaseData_; // プレイヤーの各フェーズの情報
	Data::PlayerPhase nextPhaseData_; // プレイヤーの次のフェーズの情報
	float currentRotationY_;
};