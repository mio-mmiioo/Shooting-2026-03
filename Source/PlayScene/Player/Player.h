#pragma once
#include "../../../MyLibrary/Object3D.h"
#include "../../Data.h"
#include "Gun.h"

class Camera;

class Player : public Object3D
{
public:
	Player(const VECTOR3& position, int hp);
	~Player() override;
	void Update() override;
	void Draw() override;

private:
	void DevelopmentInput();
	//void SetAimingImage(image& i, std::string path);

	Camera* camera_;
	Gun* gun_;

	VECTOR3 wPointerPosition_; // スクリーン座標の照準をワールド座標に変換した物を代入する変数
	VECTOR3 startPosition_; // 銃弾の軌道の始点
	
	GUN::TYPE currentGunType_; // 現在の銃の種類
	bool isHit_; // true→銃弾が当たる　照準の画像を切り替えるときに使用

	// 照準の画像関連
	Data::image aiming_;		// 照準の画像
	Data::image hitAiming_;	// 照準が何かにあたるときの画像
	Data::image reload_;		// リロードの画像

};