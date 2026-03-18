#include "ResultScene.h"
#include "../../MyLibrary/Input.h"
#include "../../MyLibrary/Color.h"
#include "../../MyLibrary/Observer.h"

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

	DrawFormatString(0, 300, Color::TEXT, "score : %d", Observer::GetScore());
	DrawFormatString(0, 330, Color::TEXT, "ì|ÇµÇΩìGÇÃêî : %d", Observer::GetEnemyKilled());
}