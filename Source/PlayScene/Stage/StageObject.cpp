#include "StageObject.h"
#include <assert.h>

StageObject::StageObject(const std::string& fileName, const Transform& t, int hp, int score)
{
	const std::string folder = "data/model/";
	hModel_ = MV1LoadModel((folder + fileName + ".mv1").c_str());
	assert(hModel_ > 0);
	hitModel_ = MV1LoadModel((folder + fileName + "_c.mv1").c_str());
	assert(hitModel_ > 0);

	transform_ = t;
	transform_.MakeLocalMatrix();
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1SetupCollInfo(hitModel_);

	hp_ = hp;
	if (hp > 0)
	{
		isDestructible_ = true;
		objectNumber_ = OBJECT_SORT::OBJ_OBJECT_D;
	}
	else
	{
		isDestructible_ = false;
		objectNumber_ = OBJECT_SORT::OBJ_OBJECT;
	}

	score_ = score;

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
