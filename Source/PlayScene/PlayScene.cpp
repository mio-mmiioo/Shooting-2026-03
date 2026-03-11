#include "PlayScene.h"
#include "../../MyLibrary/Input.h"
#include "../../MyLibrary/Color.h"
#include "Stage/Stage.h"
#include "Camera/Camera.h"

PlayScene::PlayScene()
{
	new Stage(0);
	new Camera();
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
