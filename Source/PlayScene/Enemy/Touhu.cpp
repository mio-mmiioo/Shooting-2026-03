#include "Touhu.h"
#include <assert.h>
#include "../../../MyLibrary/Color.h"

namespace TOUHU
{
	// 開発時のみ使用する
	const float DIRECTION_LENGTH = 100.0f;
}

Touhu::Touhu(Data::ObjectData objectData, Data::EnemyData enemyData)
{
	// Stoneと全く同じ、他にいい書き方があるかもしれない
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
	}
}

Touhu::~Touhu()
{
	if (hModel_ > 0)
	{
		hModel_ = -1;
	}
	if (hitModel_ > 0)
	{
		hitModel_ = -1;
	}
}

void Touhu::Update()
{
	// 体力が0の場合
	if (hp_ <= 0)
	{
		DestroyMe();
		return;
	}

	// 位置情報の更新
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hitModel_);
}

void Touhu::Draw()
{
	Object3D::Draw();

	// 向いている方向
	DrawLine3D(transform_.position_ + LOOK_HEIGHT,
		transform_.position_ + LOOK_HEIGHT + VECTOR3(0, 0, 1) * TOUHU::DIRECTION_LENGTH * MGetRotY(transform_.rotation_.y), Color::BLACK);
}

void Touhu::DevelopmentInput()
{
}
