#pragma once
#include "../../Library/SceneBase.h"
#include "../../MyLibrary/ClickArea.h"
#include "../Data.h"

class MenuScene : public SceneBase
{
public:
	MenuScene();
	~MenuScene();
	void Update() override;
	void Draw() override;

private:

	area courses_[Data::COURSE::MAX_COURSE];	// コースの描画を管理する配列
	bool isMouseOnArea_;		// true -> マウスがコースの画像上にある

	// 画像
	int hBackground_;		// 背景
	Data::image aiming_;	// 照準
	Data::image hitAiming_;	// 選択できる

	area currentSelect_; // 選択中のコース
};

