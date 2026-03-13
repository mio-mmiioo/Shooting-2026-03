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
	GameMaster::Update();
	if (Input::IsKeyDown("next")) {
		SceneManager::ChangeScene("RESULT");
	}
}

void PlayScene::Draw()
{
	GameMaster::Draw();
	DrawString(0, 0, "PLAY SCENE", Color::TEXT);
}
