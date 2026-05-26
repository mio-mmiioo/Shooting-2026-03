#pragma once
#include "../../Library/SceneBase.h"
#include "../../MyLibrary/ClickArea.h"

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

	Button* course_[]; // コースを選択するボタン
};

