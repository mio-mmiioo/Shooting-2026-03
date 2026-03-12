#include "Stone.h"
#include <assert.h>
#include "../../../MyLibrary/Collision.h"

namespace STONE
{

}

Stone::Stone(Data::ObjectData objectData, Data::EnemyData enemyData)
{
	const std::string folder = "data/model/";
	hModel_ = MV1LoadModel((folder + objectData.path + ".mv1").c_str());
	assert(hModel_ > 0);
	hitModel_ = MV1LoadModel((folder + objectData.path + ".mv1").c_str());
	assert(hModel_ > 0);

	transform_ = objectData.t;
	hp_ = objectData.hp;
	score_ = objectData.score;

	transform_.MakeLocalMatrix();
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1SetupCollInfo(hitModel_);

	gravity_ = enemyData.gravity;
	moveSpeed_ = enemyData.moveSpeed;
	rotateSpeed_ = enemyData.rotateSpeed;
	distanceR_ = enemyData.distanceR;

	objectNumber_ = OBJECT_SORT::OBJ_CHARA;

	Collision::AddObject(this);
}

Stone::~Stone()
{
	if (hModel_ > 0)
	{
		hModel_ = -1;
	}
	if (hitModel_ > 0)
	{
		hModel_ = -1;
	}
}

void Stone::Update()
{
	// ‘Ì—Í‚ª0‚Ìê‡‚Ìˆ—
	if (hp_ <= 0)
	{
		Collision::DeleteObject(this);
		DestroyMe();
		return;
	}
}

void Stone::Draw()
{
	Object3D::Draw();
}

void Stone::DevelopmentInput()
{
}
