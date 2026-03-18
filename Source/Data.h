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
		float distanceCurrentAndGo;		// 現在地と目的地の距離
		float distanceThisAndPlayer;	// 自身とプレイヤーの距離
		int hp;		// 体力
		int score;	// 得点
	};

	struct image
	{
		int hImage;		// 画像のハンドル
		int halfWidth;	// 画像の半分の横幅
		int halfHeight; // 画像の半分の縦幅
	};

	void Init(); // 初期化
	void SetImage(std::string name, image* i); // iにnameの画像をセットする
	VECTOR3 GetPlayerNextPosition(int phaseCount);

	extern std::map<std::string, EnemyData> enemyDataList; // 各敵のデータのリスト
	extern std::map<std::string, int> images; // ゲームで使用する画像
}