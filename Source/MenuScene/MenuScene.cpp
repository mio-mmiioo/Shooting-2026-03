#include "MenuScene.h"
#include "../../MyLibrary/Input.h"

namespace MENU_SCENE
{
	const int MOVIE_MAX_TIME = 20000; // 1つの動画を再生する最大の時間( ミリ秒 )
}

MenuScene::MenuScene()
{
	area normal;
	normal.hImage = LoadGraph("data/movie/tutorial.mp4");
	normal.leftTop = { 100.0f, 100.0f };
	normal.rightDown = { 400.0f, 300.0f };
	area select;
	select.hImage = LoadGraph("data/movie/tutorial.mp4");
	select.leftTop = { 50.0f, 50.0f };
	select.rightDown = { 450.0f, 350.0f };

	// この初期化、見た目が悪い
	course_[COURSE::TUTORIAL] = new Button(normal, select);
	course_[COURSE::COURSE1] = nullptr;
	course_[COURSE::COURSE2] = nullptr;
	course_[COURSE::COURSE3] = nullptr;
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

	for (int i = 0; i < 5; i++)
	{
		if (course_[i] == nullptr)
		{
			return;
		}

		course_[i]->Update();

		// 照準がステージの画像上にある場合、動画を再生する
		if (course_[i]->GetIsOnArea())
		{
			PlayMovieToGraph(course_[i]->GetSelectGraph());
			// 動画が一定時間再生された場合
			if (TellMovieToGraph(course_[i]->GetSelectGraph()) > MENU_SCENE::MOVIE_MAX_TIME)
			{
				SeekMovieToGraph(course_[i]->GetSelectGraph(), 0); // 開始位置を最初に戻す
			}
		}
		else
		{
			SeekMovieToGraph(course_[i]->GetSelectGraph(), 0); // 開始位置を最初に戻す
		}
	}
}

void MenuScene::Draw()
{
	for (int i = 0; i < (int)COURSE::MAX_COURSE; i++)
	{
		if (course_[i] == nullptr)
		{
			return;
		}
		course_[i]->Draw();
	}
}
