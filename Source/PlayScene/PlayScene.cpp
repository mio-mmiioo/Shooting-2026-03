#include "PlayScene.h"
#include "../../MyLibrary/Input.h"
#include "../../MyLibrary/Color.h"
#include "../../MyLibrary/Light.h"
#include "Stage/Stage.h"
#include "Camera/Camera.h"
#include "../../MyLibrary/Collision.h"

PlayScene::PlayScene()
{
	new Camera();
	new Stage(12);
	Light::Init();
}

PlayScene::~PlayScene()
{
	Collision::Release();
}

void PlayScene::Update()
{
	Light::Update();
	if (Input::IsKeyDown("next")) {
		SceneManager::ChangeScene("RESULT");
	}
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", Color::TEXT);
}
