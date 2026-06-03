#include "MenuScene.h"
#include "../../MyLibrary/Input.h"
#include "../../MyLibrary/Observer.h"

namespace MENU_SCENE
{
	const int MOVIE_MAX_TIME = 20000;	// 1つの動画を再生する最大の時間( ミリ秒 )
	const int SELECT_LINE_WIDTH = 5;	// 選択されているコースを囲む枠の太さ
	const int SELECT_LINE_COLOR = GetColor(255, 0, 0); // 選択されているコースを囲む枠の色
}

MenuScene::MenuScene()
{
	// courses_に値を代入
	{
		area tutorial	= { {235.0f, 125.0f}, {525.0f, 300.0f}, Data::movies["tutorial"] }; // チュートリアル
		area course1	= { {745.0f, 125.0f}, {1030.0f, 300.0f}, Data::movies["course1"] };
		area course2	= { {235.0f, 405.0f}, {525.0f, 580.0f}, Data::movies["course2"] };
		area course3	= { {745.0f, 405.0f}, {1030.0f, 580.0f}, Data::movies["course3"] };

		courses_[Data::COURSE::TUTORIAL] = tutorial;
		courses_[Data::COURSE::COURSE1] = course1;
		courses_[Data::COURSE::COURSE2] = course2;
		courses_[Data::COURSE::COURSE3] = course3;
	}
	
	isMouseOnArea_ = false;

	// 画像をセット
	hBackground_ = Data::images["menuBackground"];
	Data::SetImage("aiming", &aiming_);
	Data::SetImage("hitAiming", &hitAiming_);

	currentSelect_ = courses_[Data::COURSE::TUTORIAL]; // 現在選択しているコースとして、チュートリアルコースをセットする
}

MenuScene::~MenuScene()
{
	// この処理を入れないと動画がプレイシーンでも流れ続ける
	for (int i = 0; i < Data::COURSE::MAX_COURSE; i++)
	{
		PauseMovieToGraph(courses_[i].hImage); // 再生を止める
	}
}

void MenuScene::Update()
{
	isMouseOnArea_ = false;

	// 照準がコースの画像上にある場合、動画を再生する
	{
		for (int i = 0; i < Data::COURSE::MAX_COURSE; i++)
		{
			if (ClickArea::IsMosueInArea(courses_[i]) == true)
			{
				PlayMovieToGraph(courses_[i].hImage);
				// 動画が一定時間再生された場合
				if (TellMovieToGraph(courses_[i].hImage) > MENU_SCENE::MOVIE_MAX_TIME)
				{
					SeekMovieToGraph(courses_[i].hImage, 0); // 開始位置を最初に戻す
				}
				isMouseOnArea_ = true;
				currentSelect_ = courses_[i];
			}
			else
			{
				PauseMovieToGraph(courses_[i].hImage); // 再生を止める
				SeekMovieToGraph(courses_[i].hImage, 0); // 開始位置を最初に戻す
			}
		}
	}

	if (Input::IsKeyDown("outBullet") && isMouseOnArea_ == true)
	{
		for (int i = 0; i < Data::COURSE::MAX_COURSE; i++)
		{
			if (ClickArea::IsMosueInArea(courses_[i]) == true)
			{
				Observer::SetCourse(i);
			}
		}
		SceneManager::ChangeScene("PLAY");
	}
}

void MenuScene::Draw()
{
	// 背景を描画
	DrawGraph(0, 0, hBackground_, TRUE);

	// コースを描画
	for (int i = 0; i < (int)Data::COURSE::MAX_COURSE; i++)
	{
		ClickArea::DrawArea(courses_[i]);
	}

	// マウスの座標を取得
	int x = (int)Input::GetMousePosition().x;
	int y = (int)Input::GetMousePosition().y;

	// 照準を描画
	if (isMouseOnArea_ == true)
	{
		DrawBox(currentSelect_.leftTop.x, currentSelect_.leftTop.y,
			currentSelect_.rightDown.x, currentSelect_.rightDown.y, MENU_SCENE::SELECT_LINE_COLOR, FALSE, MENU_SCENE::SELECT_LINE_WIDTH);
		DrawGraph(x - hitAiming_.halfWidth, y - hitAiming_.halfHeight, hitAiming_.hImage, TRUE);
	}
	else
	{
		DrawGraph(x - aiming_.halfWidth, y - aiming_.halfHeight, aiming_.hImage, TRUE);
	}
}
