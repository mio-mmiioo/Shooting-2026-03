#pragma once
#include "Object3D.h"

namespace Collision
{
	int Release(); // 各オブジェクトで解放処理を忘れていた場合などに対応する解放処理

	void AddObject(Object3D* obj); // 当たり判定に使用するオブジェクトを追加する
	bool CheckLineHitObject(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit); // true→pos1とpos2の線分に当たるオブジェクトがある
	bool CheckDistanceVertexAndVertex(VECTOR3 pos1, VECTOR3 pos2, float distance); 
	VECTOR3 CheckPushObject(Object3D* obj); // 他のオブジェクトにめり込んでいない場所に移動させた値を返す
	VECTOR3 CheckPushObjectBySphere(Object3D* obj); // モデルと球体で確認する
	VECTOR3 CheckOnGround(Object3D* obj); // 地面に足がついている場合、押し返して移動させた値を返す
	int DeleteObject(Object3D* obj); // AddObjectを読んだ場合には必ず呼ぶ解放処理 失敗→-1

	Object3D* GetHitObject();
}
