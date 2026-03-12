#include "Collision.h"
#include <assert.h>
#include <algorithm>

namespace Collision
{
	const VECTOR3 CHECK_ONGROUND_LENGTH = { 0.0f, 5000.0f, 0.0f };
	const float HIGHEST_Y = 5000.0f; // 一番高いオブジェクトのY座標
	std::list<Object3D*> allObjectList; // すべての銃弾が当たるオブジェクト
	Object3D* hitObject = nullptr;
}

int Collision::Release()
{
	for (auto itr = allObjectList.begin(); itr != allObjectList.end(); itr++)
	{
		*itr = nullptr;
	}
	allObjectList.clear();
	return 0;
}

void Collision::AddObject(Object3D* obj)
{
	if (obj != nullptr)
	{
		allObjectList.push_back(obj);
	}
}

bool Collision::CheckLineHitObject(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit)
{
	bool found = false;
	VECTOR3 now;
	float nowVal = ((VECTOR3)(pos2 - pos1)).Size();
	for (Object3D* obj : allObjectList)
	{
		if (obj == nullptr) // この書き方はよくない
		{
			continue;
		}
		if (obj->GetObjectNumber() != OBJECT_SORT::OBJ_PLAYER)
		{
			VECTOR3 ret;
			if (obj->Object3D::CollideLine(pos1, pos2, &ret))
			{
				found = true;
				VECTOR3 v = pos1 - ret;
				float len = v.Size();
				if (len < nowVal)
				{
					nowVal = len;
					now = ret;
					hitObject = obj;
				}
			}
		}
	}
	if (hit != nullptr)
	{
		*hit = now;
	}

	// 当たったオブジェクトの種類から、判別
	if (hitObject != nullptr)
	{
		if (hitObject->GetObjectNumber() == OBJECT_SORT::OBJ_OBJECT)
		{
			found = false;
		}
	}
	return found;
}

bool Collision::CheckDistanceVertexAndVertex(VECTOR3 pos1, VECTOR3 pos2, float distance)
{
	if (VSize(pos1 - pos2) < distance)
	{
		return true;
	}
	return false;
}

VECTOR3 Collision::CheckPushObject(Object3D* obj)
{
	VECTOR3 direction; // 押し返す方向のベクトル
	VECTOR3 hit;
	VECTOR3 normal;
	VECTOR3 pos1 = obj->GetTransform().position_;
	VECTOR3 pushBack = { 0, 0, 0 };
	VECTOR3 pos2;
	float distance;
	for (Object3D* o : allObjectList)
	{
		if (o == obj)
		{
			continue;
		}

		pos2 = o->GetTransform().position_;
		if (o->CollideLine(pos1, pos2, &hit, &normal))
		{
			distance = obj->GetDistanceR();
			if (VSize(pos1 - hit) < distance)
			{
				direction = normal; // 押し返す方向のベクトル
				direction.y = 0; // y方向はCheckOnGroundで確認しているので無視する
				pushBack += direction * (distance - VSize(pos1 - hit)); // ( 押し返す方向 ) * ( 押し返したい距離 )
			}
		}
	}
	return pos1 + pushBack;
}

VECTOR3 Collision::CheckOnGround(Object3D* obj, bool* isOnGround)
{
	VECTOR3 hit;
	VECTOR3 position = obj->GetTransform().position_;
	VECTOR3 highest = { 0, -HIGHEST_Y, 0 };
	VECTOR3 ret = position;
	*isOnGround = false;

	VECTOR3 pos1 = position + CHECK_ONGROUND_LENGTH;
	VECTOR3 pos2 = position - CHECK_ONGROUND_LENGTH;

	for (Object3D* o : allObjectList)
	{
		if (obj == o)
		{
			continue;
		}

		if (o->CollideLine(pos1, pos2, &hit))
		{
			if (highest.y < hit.y)
			{
				// めり込んでいる
				highest = hit;
			}
		}
	}

	// めり込んでいる
	if (position.y < highest.y)
	{
		ret.y = hit.y;
		*isOnGround = true;
	}

	return ret;
}

int Collision::DeleteObject(Object3D* obj)
{
	int ret = -1; // 解放できなかった時に返される値
	for (auto itr = allObjectList.begin(); itr != allObjectList.end(); itr++)
	{
		if (*itr == obj)
		{
			*itr = nullptr;
			ret = 1; // 解放処理をした場合に返される値
		}
	}
	allObjectList.remove(nullptr);
	return ret;
}

Object3D* Collision::GetHitObject()
{
	return hitObject;
}