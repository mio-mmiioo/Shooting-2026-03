#include "StageSearch.h"
#include <string>
#include <queue>
#include "../../../MyLibrary/CsvReader.h"
#include "../../../MyLibrary/Color.h"

struct point
{
	int x;
	int z;
};

namespace StageSearch {
	struct NodeAStar 
	{
		int totalCost; // 予想距離
		int distance; // 距離
		point position; // 位置
		bool operator>(const NodeAStar& n) const
		{
			return totalCost > n.totalCost;
		}
	};

	// データから読み込まれるマップの情報
	enum MAP_NUM {
		EMPTY,
		WALL,
		BRANCH,
		OBJECT_SPACE,
		MAX_MAP_NUM
	};

	// 方向
	enum DIR {
		RIGHT,
		DOWN,
		LEFT,
		UP,
		MAX_DIR
	};


	const int FILE_DATA_SIZE = 64; // ファイル名に使用するデータのサイズ
	const std::string wayDataName = "data/stage/wayInfo/wayInfo03.csv";
	const point direction[DIR::MAX_DIR] = { { 1, 0 }, { 0, 1 }, {-1, 0}, {0, -1} }; // 方向
	const int INIT_DISTANCE = 5000;
	const VECTOR3 ADD_WAY_INFO_POS = { 5000.0f, 0.0f, 5000.0f }; // 中心を0にするために加える数
	const float ADD_DRAW_WAY_HEIGHT = 5.0f; // 道情報の描画のy座標
	const VECTOR3 ADD_HALF_BOX_POS = { (float)(BOX_SIZE / 2), 0.0f, (float)(BOX_SIZE / 2) };

	void ReadMap(std::string fileName);			// マップの読み込み
	void DrawSearchWay();						// 探索した道の距離などの描画
	int SearchData(VECTOR3 start, VECTOR3 end); // 経路探索のプログラム
	point Vector3ToPoint(VECTOR3 v);			// VECTOR3の座標をpointに変換する

	std::priority_queue<NodeAStar, std::vector<NodeAStar>, std::greater<NodeAStar>> aStarList;
	std::vector<std::vector<int>> map;	// 読み込む通れる場所のデータ
	std::vector<std::vector<int>> way;	// 経路
	std::vector<point> shortestWay;		// 最短経路

	int height;
	int width;
	int distance;

	int mapOffset; // map.size() / 2;
}

void StageSearch::Init()
{
	height = 0;
	width = 0;
	distance = 0;
	mapOffset = 0;
	ReadMap(wayDataName);
}

void StageSearch::Draw()
{
	int color = 0;
	for (int z = 0; z < map.size(); z++)
	{
		for (int x = 0; x < map.size(); x++)
		{
			VECTOR3 topLeft = VECTOR3(z * (float)BOX_SIZE, ADD_DRAW_WAY_HEIGHT, x * (float)BOX_SIZE) - ADD_WAY_INFO_POS;
			VECTOR3 topRight = VECTOR3(z * (float)BOX_SIZE + (float)BOX_SIZE, ADD_DRAW_WAY_HEIGHT, x * (float)BOX_SIZE) - ADD_WAY_INFO_POS;
			VECTOR3 downLeft = VECTOR3(z * (float)BOX_SIZE, ADD_DRAW_WAY_HEIGHT, x * (float)BOX_SIZE + (float)BOX_SIZE) - ADD_WAY_INFO_POS;
			VECTOR3 downRight = VECTOR3(z * (float)BOX_SIZE + (float)BOX_SIZE, ADD_DRAW_WAY_HEIGHT, x * (float)BOX_SIZE + (float)BOX_SIZE) - ADD_WAY_INFO_POS;

			if (map[x][z] == MAP_NUM::EMPTY)
			{
				color = Color::EMPTY;
			}
			else if (map[x][z] == MAP_NUM::WALL)
			{
				color = Color::WALL;
			}
			else if (map[x][z] == MAP_NUM::BRANCH)
			{
				color = Color::BRANCH;
			}
			else
			{
				color = Color::OTHER;
			}
			DrawTriangle3D(topLeft, topRight, downRight, color, TRUE);
			DrawTriangle3D(downRight, downLeft, topLeft, color, TRUE);
		}
	}

	//DrawSearchWay();
}

VECTOR3 StageSearch::GetShortestWayPosition(VECTOR3 currentPos, VECTOR3 goalPos)
{
	SearchData(currentPos, goalPos);

	point current = Vector3ToPoint(currentPos);
	point end = Vector3ToPoint(goalPos);
	
	shortestWay.clear(); // 前回の情報をクリア
	point check = end;
	point shortestNext = check;
	point next;
	int minCost = INIT_DISTANCE;
	// 現在地からゴールにたどり着くまでループ

	while (!(check.x == current.x && check.z == current.z))
	{
		shortestWay.push_back(shortestNext);
		for (int d = 0; d < DIR::MAX_DIR; d++)
		{
			next = { check.x + direction[d].x, check.z + direction[d].z };
			if (next.x < 0 || next.x >= width)
			{
				continue;
			}
			if (next.z < 0 || next.z >= height)
			{
				continue;
			}
			
			if (way[next.z][next.x] < way[check.z][check.x] && way[next.z][next.x] < minCost)
			{
				minCost = way[next.z][next.x];
				shortestNext = next;
			}
		}
		check = shortestNext;
	}
		

	// 次に向かう場所を変えす
	if (shortestWay.empty())
	{
		return currentPos;
	}

	point nextPoint = shortestWay.back();

	float x = (float)(nextPoint.x - mapOffset) * BOX_SIZE;
	float z = (float)(nextPoint.z - mapOffset) * BOX_SIZE;

	return VECTOR3{ x, currentPos.y, z };
}

void StageSearch::ReadMap(std::string fileName)
{
	char name[FILE_DATA_SIZE];
	sprintf_s<FILE_DATA_SIZE>(name, fileName.c_str());
	map.clear();

	// ステージデータの読み込み
	CsvReader* csv = new CsvReader(name);
	for (int line = 0; line < csv->GetLines(); line++)
	{
		std::vector<int> mapLine;
		for (int column = 0; column < csv->GetColumns(line); column++)
		{
			int c = csv->GetInt(line, column);
			mapLine.push_back(c);
		}
		map.push_back(mapLine);
	}
	delete csv;
	mapOffset = (int)map.size() / 2;
}

void StageSearch::DrawSearchWay()
{
	// 開発時の確認のために経路を表示する関数
	int startX = 100;
	int startY = 100;
	int boxWidth = 10;
	point leftTop;
	point rightDown;
	int color = 0;
	int d;

	if (way.empty())
	{
		return;
	}

	for (int y = 0; y < map.size() - 70; y++)
	{
		for (int x = 0; x < map.size() - 70; x++)
		{
			leftTop = { x * boxWidth + startX, y * boxWidth + startY };
			rightDown = {leftTop.x + boxWidth, leftTop.z + boxWidth };

			if (way[y][x] == INIT_DISTANCE)
			{
				color = Color::WHITE;
				d = 0;
			}
			else
			{
				color = GetColor(255, 255, 255 / distance * (distance - way[y][x]));
				d = way[y][x];
			}

			DrawBox(leftTop.x, leftTop.z, rightDown.x, rightDown.z, color, TRUE);
			//DrawFormatString(leftTop.x, leftTop.z, Color::BLACK, "%d", d);
		}
	}
	color = Color::SHORTEST_WAY;
	for (int i = 0; i < (int)shortestWay.size(); i++)
	{
		leftTop = { shortestWay[i].x * boxWidth + startX, shortestWay[i].z * boxWidth + startY };
		rightDown = { leftTop.x + boxWidth, leftTop.z + boxWidth };
		DrawBox(leftTop.x, leftTop.z, rightDown.x, rightDown.z, color, TRUE);
	}
}

int StageSearch::SearchData(VECTOR3 start, VECTOR3 end)
{
	while (!aStarList.empty()) // 探索開始前に空にする　万が一に備える
	{
		aStarList.pop();
	}

	height = (int)map.size();
	width = (int)map[0].size();

	point s = Vector3ToPoint(start);
	point e = Vector3ToPoint(end);

	way.assign(height, std::vector<int>(width, INIT_DISTANCE));
	int h = std::abs(e.x - s.x) + std::abs(e.z - s.z);
	way[s.z][s.x] = 0;
	aStarList.push({ 0 + h, 0, s });

	while (!aStarList.empty())
	{
		NodeAStar current = aStarList.top();
		aStarList.pop();

		if (current.distance > way[current.position.z][current.position.x])
		{
			continue;
		}

		if (current.position.x == e.x && current.position.z == e.z)
		{
			distance = current.distance;
			return current.distance;
		}

		for (int d = 0; d < DIR::MAX_DIR; d++)
		{
			int x = current.position.x + direction[d].x;
			int z = current.position.z + direction[d].z;

			if (x < 0 || x >= width)
			{
				continue;
			}
			if (z < 0 || z >= height)
			{
				continue;
			}
			if (map[z][x] == MAP_NUM::WALL)
			{
				continue;
			}

			// 次の場所の距離 = 現在の距離 + マップの重み + 1 ←プラス1して重みが0でも問題ないようにするため
			int nextDistance = current.distance + map[current.position.z][current.position.x] + 1;
			if (nextDistance < way[z][x])
			{
				way[z][x] = nextDistance;
				int nextTotalCost = std::abs(e.x - x) + std::abs(e.z - z);
				aStarList.push({ nextDistance + nextTotalCost, nextDistance, x, z });
			}
		}
	}
	return -1;
}

point StageSearch::Vector3ToPoint(VECTOR3 v)
{
	point ret;
	ret.x = (int)(v.x / BOX_SIZE + mapOffset);
	ret.z = (int)(v.z / BOX_SIZE + mapOffset);
	return ret;
}