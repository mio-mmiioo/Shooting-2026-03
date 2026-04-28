#include "Data.h"
#include <assert.h>
#include "../MyLibrary/CsvReader.h"

namespace Data
{
	enum CHARACTER_DATA_NUM {
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
		STATE,			// プレイヤーの状態
		ENEMY_NUM,		// 倒さなきゃいけない敵の数
		TIME,			// 次のフェーズに移るまでの時間
		MAX_P_POSITION_DATA_NUM
	};

	std::vector<PlayerPhase> playerPhase;
	std::map<std::string, CharacterData> characterDataList; // 各キャラクターのデータのリスト
	std::map<std::string, int> images;
	std::map<std::string, int> models;
	std::map<std::string, int> se;

	void InitPlayerPhase(); // プレイヤーの行動を管理するためのデータの読み込み
	void InitCharacterDataList(); // キャラクターのデータの読み込み
	void InitImage(); // 画像データの読み込み
	void InitModel(); // モデルデータの読み込み
	void InitSound(); // 音源データの読み込み
}

void Data::Init()
{
	InitPlayerPhase();
	InitCharacterDataList();
	InitImage();
	InitModel();
	InitSound();
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
	int count = max;
	if (phaseCount <= max)
	{
		count = phaseCount;
	}
	phase->position = playerPhase[count].position;
	phase->distance1 = playerPhase[count].distance1;
	phase->distance2 = playerPhase[count].distance2;
	phase->state = playerPhase[count].state;
	phase->enemyNum = playerPhase[count].enemyNum;
	phase->time = playerPhase[count].time;

	if (phaseCount <= max)
	{
		return 0;
	}

	return -1;
}

void Data::InitPlayerPhase()
{
	CsvReader* csv = new CsvReader("data/playerPhasePosition.csv");
	PlayerPhase current;
	int stateNumber = -1;
	P_STATE s;
	for (int line = 1; line < csv->GetLines(); line++)
	{
		stateNumber = csv->GetInt(line, P_POSITION_DATA_NUM::STATE); 
		switch (stateNumber)
		{
		case P_STATE::MOVE:
			s = P_STATE::MOVE;
			break;
		case P_STATE::STAY:
			s = P_STATE::STAY;
			break;
		default:
			s = P_STATE_MAX;
		}
		current.position.x = csv->GetFloat(line, P_POSITION_DATA_NUM::X);
		current.position.y = csv->GetFloat(line, P_POSITION_DATA_NUM::Y);
		current.position.z = csv->GetFloat(line, P_POSITION_DATA_NUM::Z);
		current.distance1  = csv->GetFloat(line, P_POSITION_DATA_NUM::TURN_START);
		current.distance2  = csv->GetFloat(line, P_POSITION_DATA_NUM::CHANGE_NEXT);
		current.state	   = s;
		current.enemyNum   = csv->GetInt(line, P_POSITION_DATA_NUM::ENEMY_NUM);
		current.time = csv->GetFloat(line, P_POSITION_DATA_NUM::TIME);
		playerPhase.push_back(current);
	}

	delete csv;
}

void Data::InitCharacterDataList()
{
	CsvReader* csv = new CsvReader("data/character.csv");
	CharacterData data;
	for (int line = 1; line < csv->GetLines(); line++)
	{
		std::string name			= csv->GetString(line, CHARACTER_DATA_NUM::NAME);
		data.gravity				= csv->GetFloat(line, CHARACTER_DATA_NUM::GRAVITY);
		data.moveSpeed				= csv->GetFloat(line, CHARACTER_DATA_NUM::MOVE_SPEED);
		data.rotateSpeed			= csv->GetFloat(line, CHARACTER_DATA_NUM::ROTATE_SPEED);
		data.distanceR				= csv->GetFloat(line, CHARACTER_DATA_NUM::DISTANCE_R);
		data.distanceCurrentAndGo	= csv->GetFloat(line, CHARACTER_DATA_NUM::DISTANCE1);
		data.distanceThisAndPlayer	= csv->GetFloat(line, CHARACTER_DATA_NUM::DISTANCE2);
		data.hp						= csv->GetInt(line, CHARACTER_DATA_NUM::HP);
		data.score					= csv->GetInt(line, CHARACTER_DATA_NUM::SCORE);
		characterDataList[name]		= data;
	}

	delete csv;
}

void Data::InitImage()
{
	images["titleBackground"]	= LoadGraph("data/image/titleBackground.png");
	images["aiming"]		= LoadGraph("data/image/pointer1.png");
	images["hitAiming"]		= LoadGraph("data/image/pointer2.png");
	images["reload"]		= LoadGraph("data/image/reload.png");
	images["startText"]		= LoadGraph("data/image/startText.png");
}

void Data::InitModel()
{
	// 「○○_c」のデータは当たり判定用のポリゴン数が少ないモデル

	// キャラクター
	models["player"]	 = MV1LoadModel("data/model/player.mv1"); // プレイヤー
	models["player_c"]	 = MV1LoadModel("data/model/player_c.mv1");
	models["enemy000"]	 = MV1LoadModel("data/model/enemy000.mv1"); // 石
	models["enemy000_c"] = MV1LoadModel("data/model/enemy000_c.mv1");
	models["enemy001"]	 = MV1LoadModel("data/model/enemy001.mv1"); // 豆腐
	models["enemy001_c"] = MV1LoadModel("data/model/enemy001_c.mv1");

	// 建物
	models["Stage_Obj002"]	 = MV1LoadModel("data/model/Stage_Obj002.mv1");
	models["Stage_Obj002_c"] = MV1LoadModel("data/model/Stage_Obj002_c.mv1");
	models["Stage_Obj003"]	 = MV1LoadModel("data/model/Stage_Obj003.mv1");
	models["Stage_Obj003_c"] = MV1LoadModel("data/model/Stage_Obj003_c.mv1");
	models["Stage_Obj004"]	 = MV1LoadModel("data/model/Stage_Obj004.mv1");
	models["Stage_Obj004_c"] = MV1LoadModel("data/model/Stage_Obj004_c.mv1");
	models["Stage_Obj005"]	 = MV1LoadModel("data/model/Stage_Obj005.mv1");
	models["Stage_Obj005_c"] = MV1LoadModel("data/model/Stage_Obj005_c.mv1");
	models["Stage_Obj006"]	 = MV1LoadModel("data/model/Stage_Obj006.mv1");
	models["Stage_Obj006_c"] = MV1LoadModel("data/model/Stage_Obj006_c.mv1");
	models["Stage_Obj007"]	 = MV1LoadModel("data/model/Stage_Obj007.mv1");
	models["Stage_Obj007_c"] = MV1LoadModel("data/model/Stage_Obj007_c.mv1");
	models["Stage_Obj008"]	 = MV1LoadModel("data/model/Stage_Obj008.mv1");
	models["Stage_Obj008_c"] = MV1LoadModel("data/model/Stage_Obj008_c.mv1");
	models["Stage_Obj009"]	 = MV1LoadModel("data/model/Stage_Obj009.mv1");
	models["Stage_Obj009_c"] = MV1LoadModel("data/model/Stage_Obj009_c.mv1");
	models["Stage_Obj010"]	 = MV1LoadModel("data/model/Stage_Obj010.mv1");
	models["Stage_Obj010_c"] = MV1LoadModel("data/model/Stage_Obj010_c.mv1");
	models["Stage_Obj011"]	 = MV1LoadModel("data/model/Stage_Obj011.mv1");
	models["Stage_Obj011_c"] = MV1LoadModel("data/model/Stage_Obj011_c.mv1");
}

void Data::InitSound()
{
	se["outBullet"]		= LoadSoundMem("data/sound/outBullet1.mp3");
	se["reload"]		= LoadSoundMem("data/sound/reload.mp3");
	se["attackEnemy"]	= LoadSoundMem("data/sound/attackEnemy.mp3");
	se["breakEnemy"]	= LoadSoundMem("data/sound/breakEnemy.mp3");
}
