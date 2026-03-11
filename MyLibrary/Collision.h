#pragma once
#include "Object3D.h"

namespace Collision
{
	void AddObject(Object3D* obj);
	bool CheckHitObject(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit); // èeíeÇÃìñÇΩÇËîªíËÇ…égóp
	bool CheckDistanceVertexAndVertex(VECTOR3 pos1, VECTOR3 pos2, float distance);
}
