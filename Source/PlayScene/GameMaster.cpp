#include "GameMaster.h"
#include "../../MyLibrary/Light.h"
#include "../../MyLibrary/Collision.h"
#include "../../MyLibrary/Observer.h"
#include "../../ImGui/imgui.h"
#include "Stage/Stage.h"
//#include "Stage/WayInfo.h"
#include "Stage/StageSearch.h"
#include "Camera/Camera.h"
#include "Enemy/Enemy.h"
#include "Player/Player.h"

namespace GameMaster
{
	void CreateNewStage();
	void DevelopmentInput();

	Stage* stage = nullptr;
	Player* player = nullptr;
	Camera* camera = nullptr;

	bool isCreateEnemy;			// 敵を生成するフラグ　開発時のみ使用予定
	int createEnemyNumber;		// 生成する敵の番号
	int playerPhaseCount;		// プレイヤーの移動フェーズのカウント数
	int prevPlayerPhaseCount;	// 前回のプレイヤーの移動フェーズのカウント数
	int stageNumber; // 生成するステージの番号
}

int GameMaster::Init()
{
	Observer::Init(); // 前回のプレイデータを消してスタート

	playerPhaseCount = 1; // Stageのインスタンスを作成するより先に代入すること
	prevPlayerPhaseCount = 0;

	stageNumber = 0;

	//WayInfo::Init();
	StageSearch::Init();
	camera = new Camera();
	CreateNewStage();
	Light::Init();

	isCreateEnemy = false;
	createEnemyNumber = 0;

	return 1;
}

int GameMaster::Update()
{
	player = FindGameObject<Player>();

	Light::SetPosition(player->GetTransform().position_);
	camera->SetPlayerPosition(player->GetTransform());

	Light::Update();
	Enemy::Update();

	// 前回とフェーズが異なっていた場合、新しいステージを生成して敵などを発生させる
	if (playerPhaseCount != prevPlayerPhaseCount)
	{
		// 各フェーズで発生させる敵をここにかく
		switch (playerPhaseCount)
		{
		case 0:
		case 2:
		case 3:
			break;
		case 1:
		case 4:
			CreateNewStage();
			break;
		case 5:
		case 6:
			return 0;
			break;
		}
	}

	// 開発時関連
	{
		DevelopmentInput();

		if (isCreateEnemy == true)
		{
			switch (createEnemyNumber)
			{
			case 0:
				new Stage(1); // 石が生成される
				break;
			case 1:
				new Stage(2); // 豆腐が生成される 
				break;
			}
			isCreateEnemy = false;
		}
	}

	if (player->GetHp() <= 0)
	{
		return 0;
	}

	prevPlayerPhaseCount = playerPhaseCount;

	return 1;
}

int GameMaster::Draw()
{
	//WayInfo::WayDraw();
	//StageSearch::Draw();
	return 0;
}

int GameMaster::Release()
{
	Collision::Release();
	Light::Release(); // これを呼ばないと、光源が増殖する
	return 0;
}

int GameMaster::GetPlayerPhaseCount()
{
	return playerPhaseCount;
}

int GameMaster::AddPhaseCount()
{
	playerPhaseCount += 1;
	
	// データの範囲外に行った場合は元の値に戻す処理を追加したほうが安全

	return playerPhaseCount;
}

void GameMaster::CreateNewStage()
{
	new Stage(stageNumber);
	stageNumber += 1;
}

void GameMaster::DevelopmentInput()
{
	ImGui::Begin("GameMaster");
	ImGui::InputInt("enemyNumber", &createEnemyNumber);
	if (ImGui::Button("Create Enemy"))
	{
		isCreateEnemy = true;
	}

	ImGui::InputInt("phaseCount", &playerPhaseCount);

	ImGui::End();
}
