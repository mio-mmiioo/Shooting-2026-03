#pragma once
#include "../../Data.h"

namespace Enemy
{
	enum E_SORT {
		STONE,
		TOUHU,
		MAX_E_SORT
	};

	void Update(); // 更新処理
	void Release(); // 解放処理

	void CreateEnemy(Data::ObjectData objectData); // 敵を生成する
	VECTOR3 GetPlayerPosition(); // プレイヤーの位置を返す
	void AttackPlayer(int attackPower); // プレイヤーに攻撃する
	void SetObserver(std::string name, bool isEnemy); // 自身が死ぬときにオブザーバーに得点を報告する
	void DrawWeakPoint(std::string name, int hp, VECTOR3 weakPoint);
}