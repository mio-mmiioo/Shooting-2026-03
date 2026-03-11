#include "PlayScene.h"
#include "../../MyLibrary/Input.h"
#include "../../MyLibrary/Color.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (Input::IsKeyDown("next")) {
		SceneManager::ChangeScene("RESULT");
	}
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", Color::TEXT);
}
