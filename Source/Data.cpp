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
		TURN_START,		// 曲がり始める距離
		CHANGE_NEXT,	// 次の場所に切り替える距離
		MAX_P_POSITION_DATA_NUM
	};

	std::vector<PlayerPhase> playerPhase;
	std::map<std::string, EnemyData> enemyDataList;
	std::map<std::string, int> images;

	void InitPlayerPhase();
	void InitEnemyDataList();
	void InitImage();
}

void Data::Init()
{
	InitPlayerPhase();
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

int Data::GetPlayerPhase(int phaseCount, PlayerPhase* phase)
{
	int max = (int)playerPhase.size() - 1;
	if (phaseCount > max)
	{
		phase->position = playerPhase[max].position;
		phase->distance1 = playerPhase[max].distance1;
		phase->distance2 = playerPhase[max].distance2;
		return 0;
	}
	phase->position = playerPhase[phaseCount].position;
	phase->distance1 = playerPhase[phaseCount].distance1;
	phase->distance2 = playerPhase[phaseCount].distance2;
	return 1;
}

void Data::InitPlayerPhase()
{
	CsvReader* csv = new CsvReader("data/playerPhasePosition.csv");
	PlayerPhase current;
	for (int line = 1; line < csv->GetLines(); line++)
	{
		current.position.x = csv->GetFloat(line, P_POSITION_DATA_NUM::X);
		current.position.y = csv->GetFloat(line, P_POSITION_DATA_NUM::Y);
		current.position.z = csv->GetFloat(line, P_POSITION_DATA_NUM::Z);
		current.distance1  = csv->GetFloat(line, P_POSITION_DATA_NUM::TURN_START);
		current.distance2  = csv->GetFloat(line, P_POSITION_DATA_NUM::CHANGE_NEXT);
		playerPhase.push_back(current);
	}
}

void Data::InitEnemyDataList()
{
	CsvReader* csv = new CsvReader("data/enemy.csv");
	EnemyData data;
	for (int line = 1; line < csv->GetLines(); line++)
	{
		std::string name			= csv->GetString(line, E_DATA_NUM::NAME);
		data.gravity				= csv->GetFloat(line, E_DATA_NUM::GRAVITY);
		data.moveSpeed				= csv->GetFloat(line, E_DATA_NUM::MOVE_SPEED);
		data.rotateSpeed			= csv->GetFloat(line, E_DATA_NUM::ROTATE_SPEED);
		data.distanceR				= csv->GetFloat(line, E_DATA_NUM::DISTANCE_R);
		data.distanceCurrentAndGo	= csv->GetFloat(line, E_DATA_NUM::DISTANCE1);
		data.distanceThisAndPlayer	= csv->GetFloat(line, E_DATA_NUM::DISTANCE2);
		data.hp						= csv->GetInt(line, E_DATA_NUM::HP);
		data.score					= csv->GetInt(line, E_DATA_NUM::SCORE);
		enemyDataList[name] = data;
	}

	delete csv;
}

void Data::InitImage()
{
	images["aiming"]	= LoadGraph("data/image/pointer1.png");
	images["hitAiming"] = LoadGraph("data/image/pointer2.png");
	images["reload"]	= LoadGraph("data/image/reload.png");
}
