#pragma once
#include "../../../MyLibrary/Object3D.h"
#include "../../../MyLibrary/Color.h"
#include "Enemy.h"

class Stone : public Object3D
{
public:
	Stone(Data::ObjectData objectData, Data::CharacterData characterData);
	~Stone() override;
	void Update() override;
	void Draw() override;

private:
	void DevelopmentInput(); // 開発時のみ使用する処理
	int prevHp_; // 前回の体力

};
