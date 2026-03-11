#include "Collision.h"

namespace Collision
{
	const VECTOR3 CHECK_ONGROUND_LENGTH = { 0.0f, 5000.0f, 0.0f };
	std::list<Object3D*> allObjectList; // すべての銃弾が当たるオブジェクト
	Object3D* checkObject;
}

void Collision::AddObject(Object3D* obj)
{
	if (obj != nullptr)
	{
		allObjectList.push_back(obj);
	}
}

bool Collision::CheckHitObject(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit)
{
	bool found = false;
	VECTOR3 now;
	float nowVal = ((VECTOR3)(pos2 - pos1)).Size();
	for (Object3D* obj : allObjectList)
	{
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
					checkObject = obj;
				}
			}
		}
	}
	if (hit != nullptr)
	{
		*hit = now;
	}

	// 当たったオブジェクトの種類から、判別
	if (checkObject != nullptr)
	{
		if (checkObject->GetObjectNumber() == OBJECT_SORT::OBJ_OBJECT)
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
