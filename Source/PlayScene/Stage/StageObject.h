#pragma once
#include "../../../MyLibrary/Object3D.h"
#include "../../Data.h"

class StageObject : public Object3D
{
public:
	StageObject(Data::ObjectData objectData);
	~StageObject();

	void Update() override;

private:
	bool isDestructible_; // true→壊れるオブジェクト
	int score_; // 得点
};
