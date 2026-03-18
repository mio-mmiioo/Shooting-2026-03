#include "Data.h"
#include <assert.h>
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
		DISTANCE1,		// 現在地と目的地の距離
		DISTANCE2,		// 自身とプレイヤーの距離
		HP,				// 体力
		SCORE,			// 得点
		MAX_E_DATA_NUM
	};

	enum P_POSITION_DATA_NUM {
		COUNT,	// 回数
		X,		// x座標
		Y,		// y座標
		Z,		// z座標
		MAX_P_POSITION_DATA_NUM
	};

	std::vector<VECTOR3> playerPhasePosition;
	std::map<std::string, EnemyData> enemyDataList;
	std::map<std::string, int> images;

	void InitPlayerPhasePosition();
	void InitEnemyDataList();
	void InitImage();
}

void Data::Init()
{
	InitPlayerPhasePosition();
	InitEnemyDataList();
	InitImage();
}

void Data::SetImage(std::string name, image* i)
{
	(*i).hImage = images[name];
	assert((*i).hImage > 0);
	GetGraphSize((*i).hImage, &(*i).halfWidth, &(*i).halfHeight);
	(*i).halfWidth = (*i).halfWidth / 2;
	(*i).halfHeight = (*i).halfHeight / 2;
}

VECTOR3 Data::GetPlayerNextPosition(int phaseCount)
{
	int max = playerPhasePosition.size() - 1;
	if (phaseCount > max)
	{
		return playerPhasePosition[max];
	}
	return playerPhasePosition[phaseCount];
}

void Data::InitPlayerPhasePosition()
{
	CsvReader* csv = new CsvReader("data/playerPhasePosition.csv");
	VECTOR3 current;
	for (int line = 1; line < csv->GetLines(); line++)
	{
		current.x = csv->GetInt(line, P_POSITION_DATA_NUM::X);
		current.y = csv->GetInt(line, P_POSITION_DATA_NUM::Y);
		current.z = csv->GetInt(line, P_POSITION_DATA_NUM::Z);
		playerPhasePosition.push_back(current);
	}
}

void Data::InitEnemyDataList()
{
	CsvReader* csv = new CsvReader("data/enemy.csv");
	EnemyData data;
	for (int line = 1; line < csv->GetLines(); line++)
	{
		std::string name = csv->GetString(line, E_DATA_NUM::NAME);
		data.gravity = csv->GetFloat(line, E_DATA_NUM::GRAVITY);
		data.moveSpeed = csv->GetFloat(line, E_DATA_NUM::MOVE_SPEED);
		data.rotateSpeed = csv->GetFloat(line, E_DATA_NUM::ROTATE_SPEED);
		data.distanceR = csv->GetFloat(line, E_DATA_NUM::DISTANCE_R);
		data.distanceCurrentAndGo = csv->GetFloat(line, E_DATA_NUM::DISTANCE1);
		data.distanceThisAndPlayer = csv->GetFloat(line, E_DATA_NUM::DISTANCE2);
		data.hp = csv->GetInt(line, E_DATA_NUM::HP);
		data.score = csv->GetInt(line, E_DATA_NUM::SCORE);
		enemyDataList[name] = data;
	}

	delete csv;
}

void Data::InitImage()
{
	images["aiming"] = LoadGraph("data/image/pointer1.png");
	images["hitAiming"] = LoadGraph("data/image/pointer2.png");
	images["reload"] = LoadGraph("data/image/reload.png");
}
