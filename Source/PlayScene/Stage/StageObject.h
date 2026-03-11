#pragma once
#include "../../../MyLibrary/Object3D.h"

class StageObject : public Object3D
{
public:
	StageObject(const std::string& fileName, const Transform& t, int hp, int score);
	~StageObject();

	void Update() override;

private:
	bool isDestructible_; // true→壊れるオブジェクト
	int score_; // 得点
};
