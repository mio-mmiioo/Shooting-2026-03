#include "ResultScene.h"
#include "../../MyLibrary/ClickArea.h"
#include "../../MyLibrary/Input.h"
#include "../../MyLibrary/Color.h"
#include "../../MyLibrary/Observer.h"

namespace RESULT_SCENE
{
	const float TIME = 30.0f; // リザルトシーンが表示される最大の時間

	// 得点関連
	const int NO_DAMAGE_BONUS = 1000;

	// 結果を表示する位置
	const int SCORE_X = 1000; // スコア
	const int SCORE_Y = 530;
	const int KILLED_ENEMY_NUMBER_X = 1000; // 敵を倒した数
	const int KILLED_ENEMY_NUMBER_Y = 200;
	const int TIMER_X = 1100; // タイマー
	const int TIMER_Y = 50;
	const int COURSE_X = 400; // コース
	const int COURSE_Y = 70;
	const int NO_DAMAGE_X = 1000; // ノーダメージ
	const int NO_DAMAGE_Y = 390;

	const int FONT_SIZE = 64; // 文字サイズ
}

ResultScene::ResultScene()
{
	// 画像のセット
	hBackground_ = Data::images["resultBackground"];
	Data::SetImage("aiming", &aiming_);
	Data::SetImage("hitAiming", &hitAiming_);

	int normalButtonImage = LoadGraph("data/image/ok.png");
	int selectButtonImage = LoadGraph("data/image/ok-selected.png");
	area normalArea = { {1050, 600}, {1250, 700}, normalButtonImage};
	area selectArea = { {1050, 600}, {1250, 700}, selectButtonImage };
	titleButton_ = new Button(normalArea, selectArea);
	timer_ = RESULT_SCENE::TIME;

	// Observerから取得する情報関連のセット
	{
		// コース名をセット
		{
			int courseNumber = Observer::GetCourse();
			Data::COURSE selected = (Data::COURSE)courseNumber;
			switch (selected)
			{
			case Data::COURSE::COURSE1:
				courseText_ = "コース1";
				break;
			case Data::COURSE::COURSE2:
				courseText_ = "コース2";
				break;
			case Data::COURSE::COURSE3:
				courseText_ = "コース3";
				break;
			case Data::COURSE::TUTORIAL:
				courseText_ = "チュートリアル";
				break;
			default:
				courseText_ = "範囲外";
			}
		}
		score_ = Observer::GetScore(); // 得点をセット
		enemyKilled_ = Observer::GetEnemyKilled(); // 敵を倒した数

		// ノーダメージだった場合、得点を加算させる
		if (Observer::GetIsPlayerNoDamage() == true)
		{
			noDamageBonus_ = RESULT_SCENE::NO_DAMAGE_BONUS;
			score_ += noDamageBonus_;
		}
		else
		{
			noDamageBonus_ = 0;
		}
	}

	currentScore_ = 0;
	SetFontSize(RESULT_SCENE::FONT_SIZE);
}

ResultScene::~ResultScene()
{
	delete titleButton_;
}

void ResultScene::Update()
{
	timer_ -= Time::DeltaTime();
	if (currentScore_ < score_)
	{
		currentScore_ += 1;
	}

	// 一定時間が経過したら、強制的にタイトルに遷移
	if (timer_ <= 0.0f)
	{
		SceneManager::ChangeScene("TITLE");
		timer_ = 0.0f;
	}

	titleButton_->Update();

	if (titleButton_->GetIsOnArea() == true)
	{
		PlaySoundMem(Data::se["select"], DX_PLAYTYPE_BACK, TRUE);
	}

	if (Input::IsKeyDown("outBullet") == true)
	{
		currentScore_ = score_;
	}

	// ボタンが押されたら、コース選択に遷移
	if (titleButton_->GetIsClickArea() == true)
	{
		PlaySoundMem(Data::se["outBullet"], DX_PLAYTYPE_BACK, TRUE);
		SceneManager::ChangeScene("MENU");
	}
}

void ResultScene::Draw()
{
	DrawGraph(0, 0, hBackground_, TRUE);

	titleButton_->Draw();

	// プレイ結果の表示など
	DrawFormatString(RESULT_SCENE::COURSE_X, RESULT_SCENE::COURSE_Y, Color::TEXT, courseText_.c_str());
	DrawFormatString(RESULT_SCENE::SCORE_X, RESULT_SCENE::SCORE_Y, Color::TEXT, "%05d", currentScore_);
	DrawFormatString(RESULT_SCENE::KILLED_ENEMY_NUMBER_X, RESULT_SCENE::KILLED_ENEMY_NUMBER_Y, Color::TEXT, "%05d", enemyKilled_);
	DrawFormatString(RESULT_SCENE::TIMER_X, RESULT_SCENE::TIMER_Y, Color::TEXT, "%02d", (int)timer_);
	DrawFormatString(RESULT_SCENE::NO_DAMAGE_X, RESULT_SCENE::NO_DAMAGE_Y, Color::TEXT, "%05d", noDamageBonus_);

	// マウスの座標を取得
	int x = (int)Input::GetMousePosition().x;
	int y = (int)Input::GetMousePosition().y;

	// 照準を描画
	if (titleButton_->GetIsKeepOnArea() == true)
	{
		DrawGraph(x - hitAiming_.halfWidth, y - hitAiming_.halfHeight, hitAiming_.hImage, TRUE);
	}
	else
	{
		DrawGraph(x - aiming_.halfWidth, y - aiming_.halfHeight, aiming_.hImage, TRUE);
	}
}