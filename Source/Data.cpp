#include "Data.h"
#include <assert.h>
#include "../MyLibrary/CsvReader.h"

namespace Data
{
	const int OBJ_MIN_NUMBER = 2; // 建物オブジェクトの最小の番号
	const int OBJ_MAX_NUMBER = 11; // 建物オブジェクトの最大の番号
	const int MODEL_NUMBER_DATA_SIZE = 8; // 番号を文字として扱うためのサイズ

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
	std::map<std::string, int> movies;

	void InitPlayerPhase(); // プレイヤーの行動を管理するためのデータの読み込み
	void InitCharacterDataList(); // キャラクターのデータの読み込み
	void InitImage(); // 画像データの読み込み
	void InitModel(); // モデルデータの読み込み
	void InitSound(); // 音源データの読み込み
	void InitMovie(); // 動画データの読み込み
}

void Data::Init()
{
	InitPlayerPhase();
	InitCharacterDataList();
	InitImage();
	InitModel();
	InitSound();
	InitMovie();
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
	std::string path = "data/image/";
	images["titleBackground"]	= LoadGraph((path + "titleBackground.png").c_str());
	images["menuBackground"]	= LoadGraph((path + "menuBackground.png").c_str());
	images["resultBackground"]	= LoadGraph((path + "resultBackground.png").c_str());
	images["aiming"]			= LoadGraph((path + "pointer1.png").c_str());
	images["hitAiming"]			= LoadGraph((path + "pointer2.png").c_str());
	images["reload"]			= LoadGraph((path + "reload.png").c_str());
	images["startText"]			= LoadGraph((path + "startText.png").c_str());
}

void Data::InitModel()
{
	std::string path = "data/model/";

	// 「○○_c」のデータは当たり判定用のポリゴン数が少ないモデル

	// キャラクター 敵が増える場合には、建物と同じように書く
	models["player"]	 = MV1LoadModel((path + "player.mv1").c_str()); // プレイヤー
	models["player_c"]	 = MV1LoadModel((path + "player.mv1_c").c_str());
	models["enemy000"]	 = MV1LoadModel((path + "enemy000.mv1").c_str()); // 石
	models["enemy000_c"] = MV1LoadModel((path + "enemy000_c.mv1").c_str());
	models["enemy001"]	 = MV1LoadModel((path + "tamesi/enemy001.mv1").c_str()); // 豆腐
	models["enemy001_c"] = MV1LoadModel((path + "tamesi/enemy001_c.mv1").c_str());
	models["enemy002"]   = MV1LoadModel((path + "enemy000.mv1").c_str()); // 風船 modelはまだ
	models["enemy002_c"] = MV1LoadModel((path + "enemy000_c.mv1").c_str());

	// 建物
	std::string name = "Stage_Obj";
	char number[MODEL_NUMBER_DATA_SIZE];
	for (int i = OBJ_MIN_NUMBER; i <= OBJ_MAX_NUMBER; i++)
	{
		sprintf_s<MODEL_NUMBER_DATA_SIZE >(number, "%03d", i);
		models[(name + number)] = MV1LoadModel((path + name + number + ".mv1").c_str()); // プレイヤーに見せるモデル
		models[(name + number + "_c")] = MV1LoadModel((path + name + number + "_c.mv1").c_str()); // 当たり判定に使用するモデル
	}
}

void Data::InitSound()
{
	std::string path = "data/sound/";
	se["outBullet"]			= LoadSoundMem((path + "outBullet1.mp3").c_str()); // 発砲
	se["reload"]			= LoadSoundMem((path + "reload.mp3").c_str()); // リロード
	se["attackEnemy"]		= LoadSoundMem((path + "attackEnemy.mp3").c_str()); // 敵が撃たれている
	se["breakEnemy"]		= LoadSoundMem((path + "breakEnemy.mp3").c_str()); // 敵がやられた
	se["canNotOutBullet"]	= LoadSoundMem((path + "canNotOutBullet.mp3").c_str()); // 銃弾が出ない

	se["cancel"] = LoadSoundMem((path + "cancel.mp3").c_str()); // キャンセル
	se["decide"] = LoadSoundMem((path + "decide.mp3").c_str()); // 決定
	se["select"] = LoadSoundMem((path + "select.mp3").c_str()); // 選択
}

void Data::InitMovie()
{
	std::string path = "data/movie/";
	movies["tutorial"]	= LoadGraph((path + "tutorial.ogv").c_str());
	movies["course1"]	= LoadGraph((path + "course1.ogv").c_str());
	movies["course2"]	= LoadGraph((path + "course2.ogv").c_str());
	movies["course3"]	= LoadGraph((path + "course3.ogv").c_str());
}