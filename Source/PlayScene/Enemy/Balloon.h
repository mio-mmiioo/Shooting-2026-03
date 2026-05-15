#pragma once
#include "../../../MyLibrary/Object3D.h"
#include "Enemy.h"

class Balloon : public Object3D
{
public:
	Balloon(Data::ObjectData objectData, Data::CharacterData characterData);
	~Balloon() override;
	void Update() override;
	void Draw() override;

private:
	void DevelopmentInput();	// 開発時のみ使用する処理
	
	int prevHp_;				// 前回の体力

	// 浮遊関連で使用
	float noise_;	// ランダムでずらす値
	bool isArrive_;	// true→目的地に到着した
	VECTOR3 toPosition_; // 次に向かう場所
};