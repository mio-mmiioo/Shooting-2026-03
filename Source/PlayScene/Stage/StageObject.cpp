#include "StageObject.h"
#include <assert.h>
#include "../../../MyLibrary/Collision.h"

StageObject::StageObject(Data::ObjectData objectData)
{
	// 1回ロードしたモデルを使いまわす
	hModel_ = MV1DuplicateModel(Data::models[objectData.name]);
	hitModel_ = MV1DuplicateModel(Data::models[objectData.name + "_c"]);

	transform_ = objectData.t;
	transform_.MakeLocalMatrix();
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1SetupCollInfo(hitModel_);

	hp_ = objectData.hp;
	if (hp_ > 0)
	{
		isDestructible_ = true;
		objectNumber_ = OBJECT_SORT::OBJ_OBJECT_D;
	}
	else
	{
		isDestructible_ = false;
		objectNumber_ = OBJECT_SORT::OBJ_OBJECT;
	}

	score_ = objectData.score;
	Collision::AddObject(this);
}

StageObject::~StageObject()
{
	Collision::DeleteObject(this);
	if (hModel_ > 0)
	{
		MV1DeleteModel(hModel_);
		hModel_ = -1;
	}
	if (hitModel_ > 0)
	{
		MV1DeleteModel(hitModel_);
		hitModel_ = -1;
	}
}

void StageObject::Update()
{
	if (isDestructible_ == true && hp_ <= 0)
	{
		DestroyMe();
		return;
	}
}
