#pragma once
#include "../MyLibrary/Object3D.h"
#include <vector>
#include <string>
#include <map>

namespace Data
{
	enum P_STATE { // プレイヤーの状態
		MOVE, // 移動
		STAY, // 待機
		P_STATE_MAX
	};

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

	struct CharacterData
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

	struct PlayerPhase
	{
		VECTOR3 position;	// このフェーズでプレイヤーが移動する位置
		float distance1;	// 曲がり始める距離
		float distance2;	// 次のフェーズに切り替える距離
		P_STATE state;	// プレイヤーの状態
		int enemyNum;	// 次のフェーズに行くために必要な倒す敵の数
		float time;		// 次のフェーズに遷移するまでにかかる時間
	};

	struct image
	{
		int hImage;		// 画像のハンドル
		int halfWidth;	// 画像の半分の横幅
		int halfHeight; // 画像の半分の縦幅
	};

	void Init(); // 初期化
	void SetImage(std::string name, image* i); // iにnameの画像をセットする
	int GetPlayerPhase(int phaseCount, PlayerPhase* phase);
	extern std::map<std::string, CharacterData> characterDataList; // 各キャラクターのデータのリスト
	extern std::map<std::string, int> images; // ゲームで使用する画像
	extern std::map<std::string, int> models; // 使用するモデル
	extern std::map<std::string, int> se; // 使用する音源
}