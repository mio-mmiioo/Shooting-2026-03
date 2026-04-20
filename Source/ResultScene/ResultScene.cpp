#include "ResultScene.h"
#include "../../MyLibrary/Input.h"
#include "../../MyLibrary/Color.h"
#include "../../MyLibrary/Observer.h"

namespace RESULTSCENE
{
	const float TIME = 10.0f;
}

ResultScene::ResultScene()
{
	timer_ = RESULTSCENE::TIME;
}

ResultScene::~ResultScene()
{
}

void ResultScene::Update()
{
	timer_ -= Time::DeltaTime();

	if (timer_ <= 0.0f)
	{
		SceneManager::ChangeScene("TITLE");
		timer_ = 0.0f;
	}
}

void ResultScene::Draw()
{
	DrawFormatString(0, 300, Color::TEXT, "score : %d", Observer::GetScore());
	DrawFormatString(0, 330, Color::TEXT, "“|‚µ‚½“G‚Ì” : %d", Observer::GetEnemyKilled());
}