#pragma once
#include "../../Library/SceneBase.h"
#include "../Data.h"

class Button;

class ResultScene : public SceneBase
{
public:
	ResultScene();
	~ResultScene();
	void Update() override;
	void Draw() override;
private:

	// 画像
	int hBackground_;
	Data::image aiming_;
	Data::image hitAiming_;

	Button* titleButton_;
	float timer_;

	// Observerから取得する情報関連
	std::string courseText_; // 選択したコース
	int enemyKilled_; // たおした敵の数
	int score_; // 得点
};
