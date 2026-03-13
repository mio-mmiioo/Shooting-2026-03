#include "WayInfo.h"
#include "../../../MyLibrary/CsvReader.h"
#include "../../../MyLibrary/Color.h"

struct point {
	int x;
	int z;
};

struct vertex {
	point position = { 0, 0 };	// 位置情報
	int distance = -1;			// 距離
	int number = -1;			// 識別番号
	bool isDicision = false;	// 決定しているか
	std::vector<vertex> next;	// つながっている頂点リスト
	std::vector<point> posList;	// 最短経路の道情報
};

struct way {
	point startPos = { 0, 0 };
	point endPos = { 0, 0 };
	int cost = 0;
};

namespace WayInfo {
	// 方向
	enum DIR {
		RIGHT,
		DOWN,
		LEFT,
		UP,
		MAX_DIR
	};

	// データから読み込まれるマップの情報
	enum MAP_NUM {
		EMPTY,
		WALL,
		BRANCH,
		OBJECT_SPACE,
		MAX_MAP_NUM
	};

	void InitWayData();								// 道情報をcsvファイルから読み込む
	void InitVertexList();							// 頂点情報リストを初期化
	void SetNext(point check, int vertexNum, int direction); // 次に行ける頂点情報をセットする
	bool CheckVertex(point p);						// true→頂点
	vertex FindStartVertex();						// 頂点リストの最初の頂点を求める
	void SetShortestWay(vertex start);				// 最短経路を求める
	int GetCost(point startPos, point endPos);		// 距離(cost)を求める
	VECTOR3 GetShortestWay(point pos);				// startPosからの最短経路を返す
	bool IsSameVertex(point point1, point point2);	// true→point1とpoint2が同じ頂点
	point VectorToPoint(VECTOR3 position);			// VECTOR3をpointに変換する

	const int MAX_DISTANCE = 5000; // 各頂点のコストの初期化に使用
	const VECTOR3 ADD_WAY_INFO_POS = { 5000.0f, 0.0f, 5000.0f }; // 中心を0にするために加える数
	const float ADD_DRAW_WAY_HEIGHT = 5.0f; // 道情報の描画のy座標
	const VECTOR3 ADD_HALF_BOX_POS = { (float)(BOX_SIZE / 2), 0.0f, (float)(BOX_SIZE / 2) };
	const int FILE_DATA_SIZE = 64; // ファイル名に使用するデータのサイズ
	const float SPHERE_R = 40.0f;
	const int DIV_NUM = 20;

	point dir_[DIR::MAX_DIR]; // 方向
	std::vector<std::vector<int>> wayInfo_; // 通れる道の情報

	point startPos_;						// 経路探索を開始する位置
	point goalPos_;							// 目的地
	std::vector<vertex> vertexList_;		// 頂点情報のリスト
	std::vector<vertex> checkVertexList_;	// 確認する頂点リスト 経路探索を求めるときに使用
	std::vector<way> wayList_;				// 道情報のリスト
}

void WayInfo::Init()
{
	dir_[RIGHT] = { 1, 0 };
	dir_[DOWN] = { 0, 1 };
	dir_[LEFT] = { -1, 0 };
	dir_[UP] = { 0, -1 };

	startPos_ = { -1, -1 };
	InitWayData();		// csvファイルから読み込んだデータをwayInfo_にセットする
	InitVertexList();	// 頂点情報と道情報をセットする
}

void WayInfo::WayDraw()
{
	int color = 0;
	for (int z = 0; z < wayInfo_.size(); z++)
	{
		for (int x = 0; x < wayInfo_.size(); x++)
		{
			VECTOR3 topLeft = VECTOR3(z * (float)BOX_SIZE, ADD_DRAW_WAY_HEIGHT, x * (float)BOX_SIZE) - ADD_WAY_INFO_POS;
			VECTOR3 topRight = VECTOR3(z * (float)BOX_SIZE + (float)BOX_SIZE, ADD_DRAW_WAY_HEIGHT, x * (float)BOX_SIZE) - ADD_WAY_INFO_POS;
			VECTOR3 downLeft = VECTOR3(z * (float)BOX_SIZE, ADD_DRAW_WAY_HEIGHT, x * (float)BOX_SIZE + (float)BOX_SIZE) - ADD_WAY_INFO_POS;
			VECTOR3 downRight = VECTOR3(z * (float)BOX_SIZE + (float)BOX_SIZE, ADD_DRAW_WAY_HEIGHT, x * (float)BOX_SIZE + (float)BOX_SIZE) - ADD_WAY_INFO_POS;

			if (wayInfo_[x][z] == MAP_NUM::EMPTY)
			{
				color = Color::EMPTY;
			}
			else if (wayInfo_[x][z] == MAP_NUM::WALL)
			{
				color = Color::WALL;
			}
			else if (wayInfo_[x][z] == MAP_NUM::BRANCH)
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
}

void WayInfo::DrawVertex()
{
	DrawSphere3D(ADD_WAY_INFO_POS * -1.0f, SPHERE_R, DIV_NUM, Color::WHITE, Color::WHITE, TRUE);
	VECTOR3 pos;
	int color = 0;
	for (vertex& v : vertexList_)
	{
		pos = VECTOR3((float)(v.position.x * BOX_SIZE), 0.0f, (float)(v.position.z * BOX_SIZE));
		if (v.number == 0)
		{
			color = Color::WHITE;
		}
		else
		{
			color = Color::BLACK;
		}
		DrawSphere3D(pos - ADD_WAY_INFO_POS + ADD_HALF_BOX_POS, SPHERE_R, DIV_NUM, color, color, TRUE);
	}
}

VECTOR3 WayInfo::SetVertexPosition(VECTOR3 position, int num)
{
	VECTOR3 ret = position;
	ret.x = (float)(vertexList_[num].position.x * BOX_SIZE);
	ret.z = (float)(vertexList_[num].position.z * BOX_SIZE);

	return ret - ADD_WAY_INFO_POS + ADD_HALF_BOX_POS;
}

int WayInfo::CheckVertexNum(VECTOR3 position)
{
	point check = VectorToPoint(position);
	for (vertex& v : vertexList_)
	{
		if (IsSameVertex(check, v.position) == true)
		{
			return v.number;
		}
	}
	return -1;
}

bool WayInfo::IsVertexPosition(VECTOR3 position)
{
	point check = VectorToPoint(position);
	for (vertex& v : vertexList_)
	{
		if (IsSameVertex(check, v.position) == true)
		{
			return true;
		}
	}
	return false;
}

VECTOR3 WayInfo::GetShortestWayPosition(VECTOR3 currentPos, VECTOR3 goalPos)
{
	// 道情報の初期化
	for (vertex& v : vertexList_)
	{
		v.distance = MAX_DISTANCE;
		v.isDicision = false;
		v.posList.clear();
	}

	// スタートの位置を代入
	startPos_ = VectorToPoint(currentPos);
	vertex start = FindStartVertex(); // 最初の位置を distance = 0 にする
	SetShortestWay(start);

	// whileでcheckVertexListが存在する間くり返す
	while (!checkVertexList_.empty())
	{
		SetShortestWay(checkVertexList_.front());
		checkVertexList_.erase(checkVertexList_.begin());
	}

	// goalPosに一番近い頂点をvertexListから探す
	goalPos_ = VectorToPoint(goalPos);
	VECTOR3 ret = GetShortestWay(goalPos_);

	return ret;
}

void WayInfo::InitWayData()
{
	char fileName[FILE_DATA_SIZE];
	sprintf_s<FILE_DATA_SIZE>(fileName, "data/stage/wayInfo/wayInfo%02d.csv", 1);
	wayInfo_.clear();

	// ステージデータの読み込み
	CsvReader* csv = new CsvReader(fileName);
	for (int line = 0; line < csv->GetLines(); line++)
	{
		std::vector<int> mapLine;
		for (int column = 0; column < csv->GetColumns(line); column++)
		{
			int c = csv->GetInt(line, column);
			mapLine.push_back(c);
		}
		wayInfo_.push_back(mapLine);
	}
	delete csv;
}

void WayInfo::InitVertexList()
{
	// 分岐点を頂点の番号に変更 頂点リストに頂点を追加
	for (int z = 0; z < wayInfo_.size(); z++)
	{
		for (int x = 0; x < wayInfo_.size(); x++)
		{
			if (CheckVertex(point{ x, z }) == true) // 分岐点の場合
			{
				wayInfo_[z][x] = MAP_NUM::BRANCH;
				vertex v = { point{x, z}, MAX_DISTANCE, (int)vertexList_.size(), false, std::vector<vertex>() };
				vertexList_.push_back(v);
			}
		}
	}

	// nextをセット
	for (vertex& v : vertexList_)
	{
		for (int direction = 0; direction < DIR::MAX_DIR; direction++)
		{
			int x = v.position.x + dir_[direction].x;
			int z = v.position.z + dir_[direction].z;
			point check = { x, z };
			SetNext(check, v.number, direction);
		}
	}
}

void WayInfo::SetNext(point check, int vertexNum, int direction)
{
	int distance = 1;
	if (wayInfo_[check.z][check.x] != MAP_NUM::WALL && wayInfo_[check.z][check.x] != MAP_NUM::OBJECT_SPACE)
	{
		// 距離(cost)を求める
		while (wayInfo_[check.z][check.x] != MAP_NUM::BRANCH)
		{
			check.x = check.x + dir_[direction].x;
			check.z = check.z + dir_[direction].z;
			distance += 1;
		}

		// 現在の頂点から行ける頂点をセットし、道情報もセット
		for (vertex& v : vertexList_)
		{
			if (IsSameVertex(check, v.position) == true)
			{
				vertexList_[vertexNum].next.push_back(v);
				wayList_.push_back(way{ vertexList_[vertexNum].position, v.position, distance });
				break;
			}
		}
	}
}

bool WayInfo::CheckVertex(point p)
{
	bool ret = false;
	if (p.x == 0 || p.z == 0 || p.x == vertexList_.size() - 1 || p.z == vertexList_.size() - 1)
	{
		return ret;
	}

	if (wayInfo_[p.z][p.x] != MAP_NUM::EMPTY)
	{
		return ret;
	}

	int counter = 0;
	bool checkDir[DIR::MAX_DIR];

	for (int i = 0; i < DIR::MAX_DIR; i++)
	{
		int checkX = p.x + (int)dir_[i].x;
		int checkZ = p.z + (int)dir_[i].z;
		if (wayInfo_[checkZ][checkX] == MAP_NUM::EMPTY || wayInfo_[checkZ][checkX] == MAP_NUM::BRANCH)
		{
			checkDir[i] = true;
			counter += 1;
		}
		else
		{
			checkDir[i] = false;
		}
	}

	if (counter > 2)
	{
		ret = true;
	}

	// 直角に曲がる
	for (int i = 0; i < DIR::MAX_DIR; i++)
	{
		if (i != DIR::MAX_DIR - 1)
		{
			if (checkDir[i] == checkDir[i + 1])
			{
				ret = true;
			}
		}
		else
		{
			if (checkDir[i] == checkDir[0])
			{
				ret = true;
			}
		}
	}
	return ret;
}

vertex WayInfo::FindStartVertex()
{
	for (vertex& v : vertexList_)
	{
		if (IsSameVertex(startPos_, v.position) == true)
		{
			v.distance = 0;
			v.isDicision = true;
			return v;
		}
	}
	return vertex();
}

void WayInfo::SetShortestWay(vertex start)
{
	// 確認中の頂点を決定済みにする
	vertex& startV = vertexList_[start.number];
	for (vertex& v : vertexList_)
	{
		if (IsSameVertex(v.position, start.position) == true)
		{
			v.isDicision = true;
			v.posList.push_back(start.position);
		}
	}

	// 次の場所に距離(cost)を入れる
	for (vertex& checkV : startV.next)
	{
		for (vertex& v : vertexList_)
		{
			int checkDistance = startV.distance + GetCost(startV.position, checkV.position);
			if (v.distance > checkDistance && v.isDicision == false && IsSameVertex(v.position, checkV.position) == true)
			{
				v.distance = checkDistance;
				v.posList.resize(startV.posList.size());
				v.posList.assign(startV.posList.begin(), startV.posList.end());
			}
		}
	}

	// 現時点で最も近い場所を探す
	{
		std::vector<vertex> sortMinDistance;
		for (int i = 0; i < startV.next.size(); i++)
		{
			if (vertexList_[startV.next[i].number].isDicision == false)
			{
				sortMinDistance.push_back(startV.next[i]);
			}
			// サイズが2以上ならソートする
			for (int j = (int)sortMinDistance.size() - 2; j >= 0; j--)
			{
				if (sortMinDistance[j].distance > sortMinDistance[j + 1].distance)
				{
					std::swap(sortMinDistance[j], sortMinDistance[j + 1]);
				}
			}
		}

		for (int i = 0; i < sortMinDistance.size(); i++)
		{
			checkVertexList_.push_back(sortMinDistance[i]);
			for (int j = (int)checkVertexList_.size() - 2; j >= 0; j--)
			{
				if (checkVertexList_[j].distance > checkVertexList_[j + 1].distance)
				{
					std::swap(checkVertexList_[j].distance, checkVertexList_[j + 1].distance);
				}
			}
		}
	}
}

int WayInfo::GetCost(point startPos, point endPos)
{
	for (way& w : wayList_)
	{
		if (IsSameVertex(w.startPos, startPos) == true)
		{
			if (IsSameVertex(w.endPos, endPos) == true)
			{
				return w.cost;
			}
		}
	}
	return MAX_DISTANCE;
}

VECTOR3 WayInfo::GetShortestWay(point pos)
{
	std::vector<VECTOR3> ret;
	// 最終的な経路を探す
	for (vertex& v : vertexList_)
	{
		if (IsSameVertex(v.position, pos) == true)
		{
			int checkNum = (int)v.posList.size() - 1;
			if (checkNum > 1)
			{
				while (IsSameVertex(v.posList[checkNum], v.posList[checkNum - 1]) == true)
				{
					v.posList.pop_back();
					checkNum -= 1;
				}
			}

			for (int j = 0; j < v.posList.size(); j++)
			{
				VECTOR3 pos = { (float)(v.posList[j].x * BOX_SIZE), 0.0f, (float)(v.posList[j].z * BOX_SIZE) };
				pos -= ADD_WAY_INFO_POS - ADD_HALF_BOX_POS;
				ret.push_back(pos);
			}
			break;
		}
	}

	if (ret.size() > 1)
	{
		return ret[1];
	}
	else
	{
		return ret[0];
	}
}

bool WayInfo::IsSameVertex(point point1, point point2)
{
	if (point1.x == point2.x && point1.z == point2.z)
	{
		return true;
	}
	return false;
}

point WayInfo::VectorToPoint(VECTOR3 position)
{
	point ret;
	ret.x = position.x / BOX_SIZE + wayInfo_.size() / 2;
	ret.z = position.z / BOX_SIZE + wayInfo_.size() / 2;
	return ret;
}
