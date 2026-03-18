#pragma once
#include "../../../MyLibrary/Object2D.h"

class PlayerHp : public Object2D
{
public:
	PlayerHp(int hp);
	~PlayerHp() override;
	void Update() override;
	void Draw() override;
	int GetHp() { return hp_; }
	void AddHp(int addHp, int* currentHp);

private:
	enum HP_STATE {
		HP_NORMAL,		// 通常
		HP_HEAL,		// 回復している
		HP_DAMAGING,	// 攻撃されている
		HP_DAMAGED,		// 攻撃された
		HP_MAX_STATE
	};

	int addHp_;
	int hp_;
	int maxHp_;

	float timer_;
	float hpRaitio_;
	float timeRaitio_;

	// 画像
	int hBarFrameImage_; // HPゲージの枠
	int hBarGreenImage_; // HPゲージ・緑色
	int hBarRedImage_;	 // HPゲージ・赤色

	HP_STATE state_;
};
