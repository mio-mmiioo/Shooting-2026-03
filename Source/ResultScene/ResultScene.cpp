#include "ResultScene.h"
#include "../../MyLibrary/Input.h"
#include "../../MyLibrary/Color.h"
#include "../../MyLibrary/Observer.h"

namespace RESULTSCENE
{
	const float TIME = 30.0f;
}

ResultScene::ResultScene()
{
	Data::SetImage("aiming", &aiming_);
	Data::SetImage("hitAiming", &hitAiming_);

	int titleButtonImage = LoadGraph("data/image/start.png");
	area titleArea = { {900, 600}, {1100, 700}, titleButtonImage, -1 };
	titleButton_ = { titleArea, false };

	timer_ = RESULTSCENE::TIME;
}

ResultScene::~ResultScene()
{
	DeleteGraph(titleButton_.buttonArea.hImage);
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

	if (Input::IsKeyDown("outBullet"))
	{
		ClickArea::SetClickArea(&titleButton_);
	}
	if (titleButton_.isClickArea == true)
	{
		SceneManager::ChangeScene("TITLE");
	}
}

void ResultScene::Draw()
{
	ClickArea::DrawArea(titleButton_.buttonArea, -1);

	DrawFormatString(0, 300, Color::TEXT, "score : %d", Observer::GetScore());
	DrawFormatString(0, 330, Color::TEXT, "倒した敵の数 : %d", Observer::GetEnemyKilled());

	// マウスの座標を取得
	int x = (int)Input::GetMousePosition().x;
	int y = (int)Input::GetMousePosition().y;

	// 照準を描画
	if (ClickArea::IsMosueInArea(titleButton_.buttonArea) == true)
	{
		DrawGraph(x - hitAiming_.halfWidth, y - hitAiming_.halfHeight, hitAiming_.hImage, TRUE);
	}
	else
	{
		DrawGraph(x - aiming_.halfWidth, y - aiming_.halfHeight, aiming_.hImage, TRUE);
	}
}