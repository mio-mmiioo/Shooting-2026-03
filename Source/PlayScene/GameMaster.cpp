#include "GameMaster.h"
#include "../../MyLibrary/Light.h"
#include "../../MyLibrary/Collision.h"
#include "../../ImGui/imgui.h"
#include "Stage/Stage.h"
//#include "Stage/WayInfo.h"
#include "Stage/StageSearch.h"
#include "Camera/Camera.h"
#include "Enemy/Enemy.h"

namespace GameMaster
{
	void DevelopmentInput();

	Stage* stage = nullptr;

	bool isCreateEnemy;
}

int GameMaster::Init()
{
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
	Light::Update();
	Enemy::Update();

	// ŠJ”­ŽžŠÖ˜A
	{
		DevelopmentInput();

		if (isCreateEnemy == true)
		{
			new Stage(13);
			isCreateEnemy = false;
		}
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
	return 0;
}

void GameMaster::DevelopmentInput()
{
	ImGui::Begin("GameMaster");
	if (ImGui::Button("Create Enemy"))
	{
		isCreateEnemy = true;
	}

	ImGui::End();
}
