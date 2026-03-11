#include "StageObject.h"
#include <assert.h>

StageObject::StageObject(Data::ObjectData objectData)
{
	const std::string folder = "data/model/";
	hModel_ = MV1LoadModel((folder + objectData.path + ".mv1").c_str());
	assert(hModel_ > 0);
	hitModel_ = MV1LoadModel((folder + objectData.path + "_c.mv1").c_str());
	assert(hitModel_ > 0);

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

}

StageObject::~StageObject()
{
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
