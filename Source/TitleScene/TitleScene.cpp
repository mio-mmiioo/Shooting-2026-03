#include "TitleScene.h"
#include "../../MyLibrary/Input.h"
#include "../../MyLibrary/Color.h"

TitleScene::TitleScene()
{
	Data::SetImage("aiming", &aiming_);
	Data::SetImage("hitAiming", &hitAiming_);
	int startImage = LoadGraph("data/image/start.png");
	area startArea = { {100, 100}, {300, 300}, startImage, -1 };
	start_ = { startArea, false };
}

TitleScene::~TitleScene()
{
	DeleteGraph(start_.buttonArea.hImage);
}

void TitleScene::Update()
{
	if (Input::IsKeyDown("outBullet"))
	{
		ClickArea::SetClickArea(&start_);
	}
	if (start_.isClickArea == true)
	{
		SceneManager::ChangeScene("PLAY");
	}

	//if (Input::IsKeyDown("next")) {
	//	SceneManager::ChangeScene("PLAY");
	//}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}

void TitleScene::Draw()
{
	ClickArea::DrawArea(start_.buttonArea, -1);

	// マウスの座標を取得
	int x = (int)Input::GetMousePosition().x;
	int y = (int)Input::GetMousePosition().y;

	// 照準を描画
	if (ClickArea::IsMosueInArea(start_.buttonArea) == true)
	{
		DrawGraph(x - hitAiming_.halfWidth, y - hitAiming_.halfHeight, hitAiming_.hImage, TRUE);
	}
	else
	{
		DrawGraph(x - aiming_.halfWidth, y - aiming_.halfHeight, aiming_.hImage, TRUE);
	}
}
