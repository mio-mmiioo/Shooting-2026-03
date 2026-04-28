#include "TitleScene.h"
#include "../../MyLibrary/Input.h"
#include "../../MyLibrary/Color.h"
#include "../Screen.h"

namespace TITLE_SCENE 
{
	const float MAX_TIME = 3.0f;
}

TitleScene::TitleScene()
{
	Data::SetImage("titleBackground", &background_);
	Data::SetImage("aiming", &aiming_);
	Data::SetImage("startText", &startText_);
	
	timer_ = TITLE_SCENE::MAX_TIME;
}

TitleScene::~TitleScene()
{
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

	// 時間を経過させる処理
	{
		timer_ -= Time::DeltaTime();
		if (timer_ <= 0)
		{
			timer_ += TITLE_SCENE::MAX_TIME;
		}
	}
}

void TitleScene::Draw()
{
	DrawExtendGraph(0, 0, Screen::WIDTH, Screen::HEIGHT, background_.hImage, TRUE); // 背景を表示
	int pal = (int)(200.0f / TITLE_SCENE::MAX_TIME * timer_);
	SetDrawBlendMode(DX_BLENDMODE_INVSRC, pal);
	DrawGraph(Screen::WIDTH / 2 - startText_.halfWidth, Screen::HEIGHT / 2 + startText_.halfHeight, startText_.hImage, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ２つ目の引数は意味がない

	// マウスの座標を取得
	int x = (int)Input::GetMousePosition().x;
	int y = (int)Input::GetMousePosition().y;

	DrawGraph(x - aiming_.halfWidth, y - aiming_.halfHeight, aiming_.hImage, TRUE);
}
