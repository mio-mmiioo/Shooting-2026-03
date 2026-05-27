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
	// 選択するコース
	enum COURSE {
		TUTORIAL,	// チュートリアル
		COURSE1,	// コース1
		COURSE2,	// コース2
		COURSE3,	// コース3
		MAX_COURSE	// コースの最大数
	};

	area courses_[MAX_COURSE];	// コースの描画を管理する配列
	bool isMouseOnArea_;		// true -> マウスがコースの画像上にある

	// 照準
	Data::image aiming_;
	Data::image hitAiming_;

	area currentSelect_; // 選択中のコース
};

