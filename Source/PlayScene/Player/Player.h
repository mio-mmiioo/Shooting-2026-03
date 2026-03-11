#pragma once
#include "../../../MyLibrary/Object3D.h"
#include "Gun.h"

class Camera;

struct image
{
	int hImage;		// 画像のハンドル
	int halfWidth;	// 画像の半分の横幅
	int halfHeight; // 画像の半分の縦幅
};

class Player : public Object3D
{
public:
	Player(const VECTOR3& position, int hp);
	~Player() override;
	void Update() override;
	void Draw() override;

private:
	void DevelopmentInput();
	void SetAimingImage(image& i, std::string path);

	Camera* camera_;

	int mouseX_;
	int mouseY_;
	VECTOR3 wPointerPosition_; // スクリーン座標の照準をワールド座標に変換した物を代入する変数
	VECTOR3 startPosition_; // 銃弾の軌道の始点
	
	GUN::TYPE currentGunType_;

	// 照準の画像関連
	image aiming_;		// 照準の画像
	image hitAiming_;	// 照準が何かにあたるときの画像
	image reload_;		// リロードの画像

};