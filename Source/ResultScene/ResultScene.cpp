#include "ResultScene.h"
#include "../../MyLibrary/ClickArea.h"
#include "../../MyLibrary/Input.h"
#include "../../MyLibrary/Color.h"
#include "../../MyLibrary/Observer.h"

namespace RESULTSCENE
{
	const float TIME = 30.0f;
	const int SCORE_X = 0;
	const int SCORE_Y = 300;
	const int KILLED_ENEMY_NUMBER_X = 0;
	const int KILLED_ENEMY_NUMBER_Y = 330;

}

ResultScene::ResultScene()
{
	Data::SetImage("aiming", &aiming_);
	Data::SetImage("hitAiming", &hitAiming_);

	int normalButtonImage = LoadGraph("data/image/ok.png");
	int selectButtonImage = LoadGraph("data/image/ok-selected.png");
	area normalArea = { {900, 600}, {1100, 700}, normalButtonImage};
	area selectArea = { {900, 600}, {1100, 700}, selectButtonImage };
	titleButton_ = new Button(normalArea, selectArea);
	timer_ = RESULTSCENE::TIME;
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
		SceneManager::ChangeScene("TITLE");
	}
}

void ResultScene::Draw()
{
	titleButton_->Draw();

	DrawFormatString(RESULTSCENE::SCORE_X, RESULTSCENE::SCORE_Y, Color::TEXT, "score : %d", Observer::GetScore());
	DrawFormatString(RESULTSCENE::KILLED_ENEMY_NUMBER_X, RESULTSCENE::KILLED_ENEMY_NUMBER_Y, Color::TEXT, "倒した敵の数 : %d", Observer::GetEnemyKilled());

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