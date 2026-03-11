#include "Stage.h"
#include "../../../MyLibrary/CsvReader.h"
#include "../../Data.h"

// 各オブジェクト
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "StageObject.h"

namespace STAGE
{
	const VECTOR3 ADD_POSITION = { -5000.0f, 0.0f, -5000.0f };
	const int DATA_SIZE = 16;
	const std::string FOLDER = "data/stage/";
}

Stage::Stage(int number)
{
	char fileName[STAGE::DATA_SIZE];
	sprintf_s<STAGE::DATA_SIZE>(fileName, "Stage%02d", number);
	ReadMappingData(fileName);
}

Stage::~Stage()
{
}

void Stage::ReadMappingData(std::string fileName)
{
	CsvReader* csv = new CsvReader(STAGE::FOLDER + fileName + ".csv");
	Data::ObjectData o;
	char file[STAGE::DATA_SIZE];

	for (int line = 0; line < csv->GetLines(); line++)
	{
		o.objectSortNumber = csv->GetInt(line, DATA_NUM::OBJECT);
		o.objectNumber = csv->GetInt(line, DATA_NUM::NUMBER);
		o.t.position_.x = csv->GetFloat(line, DATA_NUM::POSITION_X);
		o.t.position_.y = csv->GetFloat(line, DATA_NUM::POSITION_Y);
		o.t.position_.z = csv->GetFloat(line, DATA_NUM::POSITION_Z);
		o.t.position_ += STAGE::ADD_POSITION;
		o.hp = csv->GetInt(line, DATA_NUM::HP);

		if (o.objectSortNumber == OBJECT_SORT::OBJ_PLAYER)
		{
			// プレイヤーを作成
			new Player(o.t.position_, o.hp);
		}
		else
		{
			o.t.rotation_.x = csv->GetFloat(line, DATA_NUM::ROTATION_X);
			o.t.rotation_.y = csv->GetFloat(line, DATA_NUM::ROTATION_Y);
			o.t.rotation_.z = csv->GetFloat(line, DATA_NUM::ROTATION_Z);
			o.t.scale_.x = csv->GetFloat(line, DATA_NUM::SCALE_X);
			o.t.scale_.y = csv->GetFloat(line, DATA_NUM::SCALE_Y);
			o.t.scale_.z = csv->GetFloat(line, DATA_NUM::SCALE_Z);
			o.score = csv->GetInt(line, DATA_NUM::SCORE);

			if (o.objectSortNumber == OBJECT_SORT::OBJ_CHARA)
			{
				sprintf_s<STAGE::DATA_SIZE>(file, "enemy%03d", csv->GetInt(line, DATA_NUM::NUMBER));
				o.path = file;
				// 敵の生成
				Enemy::CreateEnemy(o);
			}
			else if (o.objectSortNumber == OBJECT_SORT::OBJ_OBJECT)
			{
				sprintf_s<STAGE::DATA_SIZE>(file, "Stage_Obj%03d", csv->GetInt(line, DATA_NUM::NUMBER));
				o.path = file;
				StageObject* obj = new StageObject(o);
			}
		}
	}
	delete csv;
}
