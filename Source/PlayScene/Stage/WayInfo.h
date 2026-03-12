#pragma once
#include "../../../MyLibrary/Object3D.h"
#include <vector>

namespace WayInfo {
	const int BOX_SIZE = 100;

	void Init();
	void WayDraw(); // マップの情報を視覚的に描画
	void DrawVertex(); // 頂点リストの頂点を描画

	VECTOR3 SetVertexPosition(VECTOR3 position, int num);
	int CheckVertexNum(VECTOR3 position); // 場所から、頂点番号を割り出す 開発時のみ使用予定
	bool IsVertexPosition(VECTOR3 position);
	VECTOR3 GetShortestWayPosition(VECTOR3 currentPos, VECTOR3 goalPos); // ( 現在地、目的地 )
}
