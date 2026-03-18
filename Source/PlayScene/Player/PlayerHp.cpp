#include "PlayerHp.h"
#include <assert.h>

namespace {
	const int LEFT_TOP_X = 20; // HPƒQ[ƒW‚Ì¶ã‚ÌxÀ•W
	const int LEFT_TOP_Y = 20; // HPƒQ[ƒW‚Ì¶ã‚ÌyÀ•W
	const int IMAGE_WIDTH = 400; // ‰æ‘œ‚Ì‰¡•
	const int IMAGE_HEIGHT = 43; // ‰æ‘œ‚Ìc•
	const float DAMAGING_TIME = 0.4f; // UŒ‚‚³‚ê‚Ä‚¢‚éŽžŠÔ
	const float DAMAGED_TIME = 2.0f; // UŒ‚‚³‚ê‚½Œã‚ÌŽžŠÔ
}

PlayerHp::PlayerHp(int hp)
{
	addHp_ = 0;
	hp_ = hp;
	maxHp_ = hp; // Å‰‚É“n‚³‚ê‚½’l‚ðÅ‘å‚Ì‘Ì—Í‚Æ‚µ‚Ä•Û‘¶‚·‚é

	// ‰æ‘œ‚Ì“Ç‚Ýž‚Ý
	hBarFrameImage_ = LoadGraph("data/image/hpBarFrame.png");
	assert(hBarFrameImage_ > 0);
	hBarGreenImage_ = LoadGraph("data/image/hpBarGreen.png");
	assert(hBarGreenImage_ > 0);
	hBarRedImage_ = LoadGraph("data/image/hpBarRed.png");
	assert(hBarRedImage_ > 0);

	timer_ = 0.0f;
	hpRaitio_ = 0.0f;
	timeRaitio_ = 0.0f;
	state_ = HP_STATE::HP_NORMAL;
}

PlayerHp::~PlayerHp()
{
}

void PlayerHp::Update()
{
	switch (state_)
	{
	case HP_STATE::HP_NORMAL:
		break;
	case HP_STATE::HP_HEAL:
		state_ = HP_STATE::HP_NORMAL;
		break;
	case HP_STATE::HP_DAMAGING:
		timer_ -= Time::DeltaTime();
		if (timer_ <= 0.0f)
		{
			timer_ = DAMAGED_TIME;
			state_ = HP_STATE::HP_DAMAGED;
		}
		break;
	case HP_STATE::HP_DAMAGED:
		timer_ -= Time::DeltaTime();
		if (timer_ <= 0.0f)
		{
			addHp_ = 0;
			hpRaitio_ = 0.0f;
			state_ = HP_STATE::HP_NORMAL;
		}
		break;
	}

}

void PlayerHp::Draw()
{
	float raitio = (float)hp_ / (float)maxHp_;
	DrawGraph(LEFT_TOP_X, LEFT_TOP_Y, hBarFrameImage_, TRUE);

	switch (state_)
	{
	case HP_STATE::HP_NORMAL:
	case HP_STATE::HP_HEAL:
	case HP_STATE::HP_DAMAGING:
		DrawRectGraph(LEFT_TOP_X, LEFT_TOP_Y, 0, 0, (int)(IMAGE_WIDTH * (raitio + hpRaitio_ * 1.0f)), IMAGE_HEIGHT, hBarRedImage_, TRUE);
		break;
	case HP_STATE::HP_DAMAGED:
		timeRaitio_ = timer_ / DAMAGED_TIME;
		DrawRectGraph(LEFT_TOP_X, LEFT_TOP_Y, 0, 0, (int)(IMAGE_WIDTH * (raitio + hpRaitio_ * timeRaitio_)), IMAGE_HEIGHT, hBarRedImage_, TRUE);
	}
	DrawRectGraph(LEFT_TOP_X, LEFT_TOP_Y, 0, 0, (int)(IMAGE_WIDTH * raitio), IMAGE_HEIGHT, hBarGreenImage_, TRUE);
}

void PlayerHp::AddHp(int addHp, int* currentHp)
{
	addHp_ = std::abs(addHp);
	hpRaitio_ = (float)std::abs(addHp_) / (float)maxHp_ + hpRaitio_ * timeRaitio_;
	hp_ += addHp;
	*currentHp += addHp;
	if (hp_ > maxHp_) // Å‘å’l‚æ‚è‘å‚«‚­‚È‚Á‚Ä‚¢‚½ê‡
	{
		hp_ = maxHp_;
		*currentHp += addHp;
	}
	if (addHp > 0)
	{
		state_ = HP_STATE::HP_HEAL;
	}
	else if (addHp < 0)
	{
		state_ = HP_STATE::HP_DAMAGING;
		timer_ = DAMAGING_TIME;
	}
}
