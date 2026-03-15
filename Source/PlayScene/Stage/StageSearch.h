#pragma once
#include "../../../MyLibrary/Object3D.h"
#include <vector>

namespace StageSearch {
	const int BOX_SIZE = 100;

	void Init();
	void Draw(); // マップの情報を視覚的に描画

	VECTOR3 GetShortestWayPosition(VECTOR3 currentPos, VECTOR3 goalPos); // ( 現在地、目的地 )
}

