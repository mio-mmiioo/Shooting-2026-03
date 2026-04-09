#include "Stone.h"
#include <assert.h>
#include "../../../MyLibrary/Collision.h"
#include "../../../MyLibrary/Observer.h"

namespace STONE
{

}

Stone::Stone(Data::ObjectData objectData, Data::EnemyData enemyData)
{
	hModel_ = MV1DuplicateModel(Data::models[objectData.name]);
	hitModel_ = MV1DuplicateModel(Data::models[objectData.name + "_c"]);

	transform_ = objectData.t;
	hp_ = enemyData.hp;
	score_ = enemyData.score;

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
		MV1DeleteModel(hModel_);
		hModel_ = -1;
	}
	if (hitModel_ > 0)
	{
		MV1DeleteModel(hitModel_);
		hModel_ = -1;
	}
	Observer::EnemyKilled();
}

void Stone::Update()
{
	// ‘Ì—Í‚ª0‚Ìê‡‚Ìˆ—
	if (hp_ <= 0)
	{
		Enemy::SetObserver("stone", false);
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
