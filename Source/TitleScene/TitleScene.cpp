#include "TitleScene.h"
#include "../../MyLibrary/Input.h"
#include "../../MyLibrary/Color.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
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
}
