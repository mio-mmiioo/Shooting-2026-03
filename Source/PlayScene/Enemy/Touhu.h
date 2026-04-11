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
	enum TOUHU_STATE {
		WALK,	// 歩く
		STAY,	// とどまる
		ATTACK,	// 攻撃する
		MAX_TOUHU_STATE
	};

	void DevelopmentInput();	// 開発時のみ使用する処理
	void WalkUpdate();			// 移動の更新処理
	void StayUpdate();			// とどまるときの更新処理
	void AttackUpdate();		// 攻撃するときの更新処理

	TOUHU_STATE state_;		// 状態
	VECTOR3 goPosition_;	// 次に向かう場所
	bool isArrive_;			// true→目的地に到着した
	float stateTimer_;		// 状態を管理するタイマー

	int prevHp_; // 前回の体力
};