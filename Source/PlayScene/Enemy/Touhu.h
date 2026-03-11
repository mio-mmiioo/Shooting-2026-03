#pragma once
#include "../../../MyLibrary/Object3D.h"
#include "Enemy.h"

class Touhu : public Object3D
{
public:
	Touhu(Data::ObjectData objectData, Data::EnemyData enemyData);
	~Touhu() override;
	void Update() override;
	void Draw() override;

private:
	void DevelopmentInput(); // ŠJ”­‚Ì‚İg—p‚·‚éˆ—

};