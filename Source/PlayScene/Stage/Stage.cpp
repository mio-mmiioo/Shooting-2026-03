#include "Stage.h"
#include "../../../MyLibrary/CsvReader.h"

// 各オブジェクト
#include "../Player/Player.h"
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
	Transform t;
	int sortNumber;
	int hp;
	int score;
	char file[STAGE::DATA_SIZE];

	for (int line = 0; line < csv->GetLines(); line++)
	{
		sortNumber = csv->GetInt(line, DATA_NUM::OBJECT);
		t.position_.x = csv->GetFloat(line, DATA_NUM::POSITION_X);
		t.position_.y = csv->GetFloat(line, DATA_NUM::POSITION_Y);
		t.position_.z = csv->GetFloat(line, DATA_NUM::POSITION_Z);
		t.position_ += STAGE::ADD_POSITION;
		hp = csv->GetInt(line, DATA_NUM::HP);

		if (sortNumber == OBJECT_SORT::OBJ_PLAYER)
		{
			// プレイヤーを作成
			new Player(t.position_, hp);
		}
		else
		{
			t.rotation_.x = csv->GetFloat(line, DATA_NUM::ROTATION_X);
			t.rotation_.y = csv->GetFloat(line, DATA_NUM::ROTATION_Y);
			t.rotation_.z = csv->GetFloat(line, DATA_NUM::ROTATION_Z);
			t.scale_.x = csv->GetFloat(line, DATA_NUM::SCALE_X);
			t.scale_.y = csv->GetFloat(line, DATA_NUM::SCALE_Y);
			t.scale_.z = csv->GetFloat(line, DATA_NUM::SCALE_Z);
			score = csv->GetInt(line, DATA_NUM::SCORE);

			if (sortNumber == OBJECT_SORT::OBJ_CHARA)
			{
				// 敵などの生成
			}
			else if (sortNumber == OBJECT_SORT::OBJ_OBJECT)
			{
				sprintf_s<STAGE::DATA_SIZE>(file, "Stage_Obj%03d", csv->GetInt(line, DATA_NUM::NUMBER));
				StageObject* obj = new StageObject(file, t, hp, score);
			}
		}
	}
	delete csv;
}
