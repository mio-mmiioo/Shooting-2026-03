#include "ResultScene.h"
#include "../../MyLibrary/Input.h"
#include "../../MyLibrary/Color.h"

ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Update()
{
	if (Input::IsKeyDown("next")) {
		SceneManager::ChangeScene("TITLE");
	}
}

void ResultScene::Draw()
{
	DrawString(0, 0, "RESULT SCENE", Color::TEXT);
}