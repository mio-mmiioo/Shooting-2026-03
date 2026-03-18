#include "PlayScene.h"
#include "../../MyLibrary/Input.h"
#include "../../MyLibrary/Color.h"
#include "GameMaster.h"

PlayScene::PlayScene()
{
	GameMaster::Init();
}

PlayScene::~PlayScene()
{
	GameMaster::Release();
}

void PlayScene::Update()
{
	if (GameMaster::Update() == 0)
	{
		SceneManager::ChangeScene("RESULT");
	}
	if (Input::IsKeyDown("next")) {
		SceneManager::ChangeScene("RESULT");
	}
}

void PlayScene::Draw()
{
	GameMaster::Draw();
	DrawString(0, 0, "PLAY SCENE", Color::TEXT);
}
