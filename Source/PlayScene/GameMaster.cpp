#include "GameMaster.h"
#include "../../MyLibrary/Light.h"
#include "../../MyLibrary/Collision.h"
#include "../../ImGui/imgui.h"
#include "Stage/Stage.h"
//#include "Stage/WayInfo.h"
#include "Stage/StageSearch.h"
#include "Camera/Camera.h"
#include "Enemy/Enemy.h"
#include "Player/Player.h"

namespace GameMaster
{
	void DevelopmentInput();

	Stage* stage = nullptr;
	Player* player = nullptr;

	bool isCreateEnemy; // 敵を生成するフラグ　開発時のみ使用予定
	int playerPhaseCount; // プレイヤーの移動フェーズのカウント数;
}

int GameMaster::Init()
{
	playerPhaseCount = 1; // Stageのインスタンスを作成するより先に代入すること

	//WayInfo::Init();
	StageSearch::Init();
	new Camera();
	stage = new Stage(12);
	Light::Init();

	isCreateEnemy = false;


	return 1;
}

int GameMaster::Update()
{
	player = FindGameObject<Player>();
	Light::Update();
	Enemy::Update();

	// 開発時関連
	{
		DevelopmentInput();

		if (isCreateEnemy == true)
		{
			new Stage(13);
			isCreateEnemy = false;
		}
	}

	if (player->GetHp() <= 0)
	{
		return 0;
	}

	return 1;
}

int GameMaster::Draw()
{
	//WayInfo::WayDraw();
	StageSearch::Draw();
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

void GameMaster::DevelopmentInput()
{
	ImGui::Begin("GameMaster");
	if (ImGui::Button("Create Enemy"))
	{
		isCreateEnemy = true;
	}

	ImGui::InputInt("phaseCount", &playerPhaseCount);

	ImGui::End();
}
