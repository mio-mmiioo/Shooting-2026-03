#include "MenuScene.h"
#include "../../MyLibrary/Input.h"

MenuScene::MenuScene()
{
}

MenuScene::~MenuScene()
{
}

void MenuScene::Update()
{
	if (Input::IsKeyDown("outBullet"))
	{
		SceneManager::ChangeScene("PLAY");
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}

void MenuScene::Draw()
{
}
