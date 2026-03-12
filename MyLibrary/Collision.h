#pragma once
#include "Object3D.h"

namespace Collision
{
	int Release();

	void AddObject(Object3D* obj); // 当たり判定に使用するオブジェクトを追加する
	bool CheckHitObject(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit); // true→pos1とpos2の線分に当たるオブジェクトがある
	bool CheckDistanceVertexAndVertex(VECTOR3 pos1, VECTOR3 pos2, float distance);


	int DeleteObject(Object3D* obj); // AddObjectを読んだ場合には必ず呼ぶ解放処理 失敗→-1
}
