#include "TitleScene.h"
#include "../../MyLibrary/Input.h"
#include "../../MyLibrary/Color.h"
#include "../Screen.h"

TitleScene::TitleScene()
{
	Data::SetImage("aiming", &aiming_);
	Data::SetImage("startText", &startText_);
}

TitleScene::~TitleScene()
{
	DeleteGraph(startText_.hImage);
}

void TitleScene::Update()
{
	if (Input::IsKeyDown("outBullet"))
	{
		SceneManager::ChangeScene("PLAY");
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}

void TitleScene::Draw()
{
	DrawGraph(Screen::WIDTH / 2 - startText_.halfWidth, Screen::HEIGHT / 2 + startText_.halfHeight, startText_.hImage, true);

	// マウスの座標を取得
	int x = (int)Input::GetMousePosition().x;
	int y = (int)Input::GetMousePosition().y;

	DrawGraph(x - aiming_.halfWidth, y - aiming_.halfHeight, aiming_.hImage, TRUE);
}
