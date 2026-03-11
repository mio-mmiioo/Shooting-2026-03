#pragma once
#include "../MyLibrary/Object3D.h"
#include <vector>
#include <string>
#include <map>

namespace Data
{
	struct ObjectData
	{
		int objectSortNumber;	// オブジェクトの種類
		std::string name;		// オブジェクトの名前
		std::string path;		// オブジェクトのファイルのパス
		int objectNumber;		// オブジェクトの番号
		Transform t;			// 位置、回転、拡縮の情報
		int hp;					// 体力
		int score;				// 得点
	};

	struct EnemyData
	{
		float gravity;		// 重力
		float moveSpeed;	// 移動速度
		float rotateSpeed;	// 回転速度
		float distanceR;	// 当たり判定の半径
	};

	void Init(); // 初期化

	extern std::map<std::string, EnemyData> enemyDataList;
}