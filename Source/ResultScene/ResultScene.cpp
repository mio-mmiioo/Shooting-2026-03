#include "ResultScene.h"
#include "../../MyLibrary/ClickArea.h"
#include "../../MyLibrary/Input.h"
#include "../../MyLibrary/Color.h"
#include "../../MyLibrary/Observer.h"

namespace RESULTSCENE
{
	const float TIME = 30.0f;

	// 数字を表示する位置
	const int SCORE_X = 1000; // スコア
	const int SCORE_Y = 530;
	const int KILLED_ENEMY_NUMBER_X = 1000; // 敵を倒した数
	const int KILLED_ENEMY_NUMBER_Y = 200;
	const int TIMER_X = 1100; // タイマー
	const int TIMER_Y = 50;
	const int COURSE_X = 500; // コース
	const int COURSE_Y = 100;

	const int FONT_SIZE = 64;
}

ResultScene::ResultScene()
{
	// 画像のセット
	hBackground_ = Data::images["resultBackground"];
	Data::SetImage("aiming", &aiming_);
	Data::SetImage("hitAiming", &hitAiming_);

	int normalButtonImage = LoadGraph("data/image/ok.png");
	int selectButtonImage = LoadGraph("data/image/ok-selected.png");
	area normalArea = { {1050, 600}, {1250, 700}, normalButtonImage};
	area selectArea = { {1050, 600}, {1250, 700}, selectButtonImage };
	titleButton_ = new Button(normalArea, selectArea);
	timer_ = RESULTSCENE::TIME;

	SetFontSize(RESULTSCENE::FONT_SIZE);
}

ResultScene::~ResultScene()
{
	delete titleButton_;
}

void ResultScene::Update()
{
	timer_ -= Time::DeltaTime();

	// 一定時間が経過したら、タイトルに戻る
	if (timer_ <= 0.0f)
	{
		SceneManager::ChangeScene("TITLE");
		timer_ = 0.0f;
	}

	titleButton_->Update();

	if (titleButton_->GetIsClickArea() == true)
	{
		PlaySoundMem(Data::se["outBullet"], DX_PLAYTYPE_BACK, TRUE);
		SceneManager::ChangeScene("TITLE");
	}
}

void ResultScene::Draw()
{
	DrawGraph(0, 0, hBackground_, TRUE);

	titleButton_->Draw();

	DrawFormatString(RESULTSCENE::SCORE_X, RESULTSCENE::SCORE_Y, Color::TEXT, "%d", Observer::GetScore());
	DrawFormatString(RESULTSCENE::KILLED_ENEMY_NUMBER_X, RESULTSCENE::KILLED_ENEMY_NUMBER_Y, Color::TEXT, "%d", Observer::GetEnemyKilled());
	DrawFormatString(RESULTSCENE::TIMER_X, RESULTSCENE::TIMER_Y, Color::TEXT, "%02d", (int)timer_);

	// マウスの座標を取得
	int x = (int)Input::GetMousePosition().x;
	int y = (int)Input::GetMousePosition().y;

	// 照準を描画
	if (titleButton_->GetIsOnArea() == true)
	{
		DrawGraph(x - hitAiming_.halfWidth, y - hitAiming_.halfHeight, hitAiming_.hImage, TRUE);
	}
	else
	{
		DrawGraph(x - aiming_.halfWidth, y - aiming_.halfHeight, aiming_.hImage, TRUE);
	}
}