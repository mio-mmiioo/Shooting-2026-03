#include "TitleScene.h"
#include "../../MyLibrary/Input.h"
#include "../../MyLibrary/Color.h"

TitleScene::TitleScene()
{
	Data::SetImage("aiming", &aiming_);
	Data::SetImage("hitAiming", &hitAiming_);

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
	if (Input::IsKeyDown("next")) {
		SceneManager::ChangeScene("PLAY");
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}

void TitleScene::Draw()
{
	DrawString(0, 0, "TITLE SCENE", Color::TEXT);

	// •`‰æ‚³‚ê‚é‚©Šm”F
	int x = (int)Input::GetMousePosition().x;
	int y = (int)Input::GetMousePosition().y;
	DrawGraph(x - aiming_.halfWidth, y - aiming_.halfHeight, aiming_.hImage, TRUE);
}
