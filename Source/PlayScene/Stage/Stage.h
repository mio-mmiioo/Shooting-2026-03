#pragma once
#include "../../../MyLibrary/Object3D.h"

class Stage : public Object3D
{
public:
	Stage(int number = 0);
	~Stage();

private:
	enum DATA_NUM {
		OBJECT,
		NUMBER,
		POSITION_X,
		POSITION_Y,
		POSITION_Z,
		ROTATION_X,
		ROTATION_Y,
		ROTATION_Z,
		SCALE_X,
		SCALE_Y,
		SCALE_Z,
		HP,
		SCORE,
		MAX_DATA
	};

	void ReadMappingData(std::string fileName);
};
