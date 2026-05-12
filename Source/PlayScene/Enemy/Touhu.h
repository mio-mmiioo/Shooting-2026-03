#pragma once
#include "../../../MyLibrary/Object3D.h"
#include "Enemy.h"

class Animator;

class Touhu : public Object3D
{
public:
	Touhu(Data::ObjectData objectData, Data::CharacterData characterData);
	~Touhu() override;
	void Update() override;
	void Draw() override;

private:
	enum TOUHU_STATE {
		WALK,	// 歩く
		STAY,	// とどまる
		ATTACK,	// 攻撃する
		DOWN,	// 倒れた
		MAX_TOUHU_STATE
	};

	enum AnimID {
		A_NEUTRAL = 0,
		A_WALK,
		A_ATTACK1,
		A_DOWN,
		A_MAX
	};
	Animator* animator_;

	void DevelopmentInput();	// 開発時のみ使用する処理
	void WalkUpdate();			// 移動の更新処理
	void StayUpdate();			// とどまるときの更新処理
	void AttackUpdate();		// 攻撃するときの更新処理
	void DownUpdate();			// やられたときの更新処理

	TOUHU_STATE state_;		// 状態
	VECTOR3 goPosition_;	// 次に向かう場所
	bool isArrive_;			// true→目的地に到着した
	float stateTimer_;		// 状態を管理するタイマー

	int prevHp_; // 前回の体力
};