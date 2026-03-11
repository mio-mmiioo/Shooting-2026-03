#include "Data.h"
#include "../MyLibrary/CsvReader.h"

namespace Data
{
	enum E_DATA_NUM {
		NAME,			// 名前
		SORT,			// 種類
		GRAVITY,		// 重力
		MOVE_SPEED,		// 移動速度
		ROTATE_SPEED,	// 回転速度
		DISTANCE_R,		// 当たり判定の半径
		MAX_E_DATA_NUM
	};
	std::map<std::string, EnemyData> enemyDataList;

	void InitEnemyDataList();
}

void Data::Init()
{
	InitEnemyDataList();
}

void Data::InitEnemyDataList()
{
	CsvReader* csv = new CsvReader("data/enemy.csv");
	EnemyData data;
	for (int line = 1; line < csv->GetLines(); line++)
	{
		std::string name = csv->GetString(line, E_DATA_NUM::NAME);
		//int sortNumber = csv->GetInt(line, E_DATA_NUM::SORT);
		data.gravity = csv->GetFloat(line, E_DATA_NUM::GRAVITY);
		data.moveSpeed = csv->GetFloat(line, E_DATA_NUM::MOVE_SPEED);
		data.rotateSpeed = csv->GetFloat(line, E_DATA_NUM::ROTATE_SPEED);
		data.distanceR = csv->GetFloat(line, E_DATA_NUM::DISTANCE_R);
		enemyDataList[name] = data;
	}
}
