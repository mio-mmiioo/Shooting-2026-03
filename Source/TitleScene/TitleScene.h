#pragma once
#include "../../Library/SceneBase.h"
#include "../../MyLibrary/ClickArea.h"
#include "../Data.h"

class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();
	void Update() override;
	void Draw() override;

private:
	Data::image background_;	// 背景
	Data::image aiming_;		// 照準
	Data::image startText_;		// ゲームを開始してもらうための文

	float timer_; // タイマー 時間経過で画面を変化させるために使用
};
