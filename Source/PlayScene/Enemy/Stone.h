#pragma once
#include "../../../MyLibrary/Object3D.h"
#include "Enemy.h"

class Stone : public Object3D
{
public:
	Stone(Data::ObjectData objectData, Data::EnemyData enemyData);
	~Stone() override;
	void Update() override;
	void Draw() override;

private:
	void DevelopmentInput(); // ŠJ”­‚Ì‚İg—p‚·‚éˆ—
	
};
