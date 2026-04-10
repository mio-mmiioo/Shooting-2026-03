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
	void DevelopmentInput(); // 開発時のみ使用する処理

	int maxHp_; // 最大の体力
	int weakPointColor_; // 弱点の色
	int red_; // 弱点の色の赤 
	int green_; // 弱点の色の緑
	int blue_; // 弱点の色の青

};
