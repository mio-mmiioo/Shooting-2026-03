#include "Gun.h"
#include <assert.h>
#include "../../../MyLibrary/Color.h"

namespace GUN
{
	int gaugePosX = 80;
	int gaugePosY = 140;

	const float START_PERCENT = 35.0f;
	const float END_PERCENT = 115.0f;
}

Gun::Gun()
{
	gunType_ = GUN::TYPE::HAND;

	hImageEffectOutBullet_ = LoadGraph("data/image/light.png");
	assert(hImageEffectOutBullet_ > 0);

	current_ = { GUN::TYPE::MAX_TYPE, 0, 0, 0, 0, -1, -1, 0.0f, 0.0f, 0.0f };

	hand_ = {
		GUN::TYPE::HAND,
		5,
		5,
		16,
		16,
		LoadGraph("data/image/bulletUi01.png"),
		LoadGraph("data/image/bulletUi02.png"),
		0.0f,
		3.0f,
		0.0f,
		0.7f
	};

	machine_ = {
		GUN::TYPE::MACHINE,
		2,
		200,
		50,
		50,
		LoadGraph("data/image/bulletUi01.png"),
		LoadGraph("data/image/bulletUi02.png"),
		0.0f,
		2.0f,
		0.0f,
		0.06f
	};

	SetDrawOrder(-100);
}

Gun::~Gun()
{
	if (hImageEffectOutBullet_ > 0)
	{
		DeleteGraph(hImageEffectOutBullet_);
		hImageEffectOutBullet_ = -1;
	}
}

void Gun::Update()
{
	// タイマーを進める
	if (current_.reloadTimer > 0)
	{
		current_.reloadTimer -= Time::DeltaTime();
	}
	if (current_.coolDownTimer > 0)
	{
		current_.coolDownTimer -= Time::DeltaTime();
	}
}

void Gun::Draw()
{
	DrawRemainingSetting();
}

void Gun::DrawRemainingSetting()
{
	float p = 100.0f / (current_.maxSetting * 1.25f);
	float remaining = GUN::END_PERCENT - (current_.remainingSetting * p);

	DrawCircleGauge(GUN::gaugePosX, GUN::gaugePosY, GUN::END_PERCENT, current_.hImageGauge, GUN::START_PERCENT, 1.0f, 0, 0);
	DrawCircleGauge(GUN::gaugePosX, GUN::gaugePosY, GUN::END_PERCENT, current_.hImageGaugeRemaining, remaining, 1.0f, 0, 0);

	// 残弾数の表示
	if (current_.remainingAll <= 0 && current_.remainingSetting == 0)
	{
		DrawFormatString(GUN::gaugePosX, GUN::gaugePosY, Color::RED, "EMPTY");
	}
	else
	{
		DrawFormatString(GUN::gaugePosX, GUN::gaugePosY, Color::BLACK, "%04d", current_.remainingAll);
	}
}

bool Gun::OutBullet()
{
	if (current_.coolDownTimer > 0 || current_.reloadTimer > 0) // クールダウン中orリロード中
	{
		return false;
	}
	if (current_.remainingSetting <= 0) // 残弾数が0以下
	{
		return false;
	}

	// 銃を撃つ処理
	current_.remainingSetting -= 1;
	current_.coolDownTimer = current_.coolDownTime;
	OutBulletEffect();
	return true;
}

void Gun::OutBulletEffect()
{
	DrawGraph(-10, -10, hImageEffectOutBullet_, TRUE); // 画像の端が黒いため、位置を少しずらしている
}

bool Gun::ReloadBullet()
{
	if (current_.reloadTimer <= 0) // リロード中じゃない
	{
		if (!((current_.remainingAll == current_.remainingSetting) || (current_.remainingAll <= 0))) // リロードできない銃弾数条件じゃない
		{
			int canSetNum = current_.maxSetting - current_.remainingSetting; // 装填可能数 0になってからリロードとは限らない
			if (canSetNum >= current_.remainingAll) // 装填可能数 >= 残弾数
			{
				current_.remainingSetting += current_.remainingAll;
				current_.remainingAll = 0;
			}
			else
			{
				current_.remainingSetting += canSetNum;
				current_.remainingAll -= canSetNum;
			}
			current_.reloadTimer = current_.reloadTimer; // リロードの時間をセット
		}
		return true; // リロード処理ができたため、true
	}
	return false; // リロード処理中なため、不可
}

void Gun::AddBurrelt(int addNumber)
{
	current_.remainingAll += addNumber;
}

void Gun::SetGunType(GUN::TYPE type)
{
	// currentを元のtructに代入
	switch (current_.type)
	{
	case GUN::TYPE::HAND:
		hand_ = current_;
		break;
	case GUN::TYPE::MACHINE:
		machine_ = current_;
		break;
	}

	// currentに新しくセットされる種類の情報を代入
	switch (type)
	{
	case GUN::TYPE::HAND:
		current_ = hand_;
		break;
	case GUN::TYPE::MACHINE:
		current_ = machine_;
		break;
	}
}

int Gun::GetAttack()
{
	return current_.attackPower;
}

float Gun::GetReloadTimer()
{
	return current_.reloadTimer;
}

float Gun::GetReloadTime()
{
	return current_.reloadTime;
}
