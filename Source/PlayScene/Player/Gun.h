#pragma once
#include "../../../MyLibrary/Object3D.h"

namespace GUN
{
	enum TYPE {
		HAND,		// ハンドガン
		MACHINE,	// マシンガン
		MAX_TYPE,
	};
}

class Gun : public Object3D
{
	struct gun {
		GUN::TYPE type;				// 銃の種類
		int attackPower;				// 攻撃力
		int remainingAll;			// 残弾数
		int maxSetting;				// 銃に装填できる最大数
		int remainingSetting;		// 銃に装填されている残弾数
		int hImageGauge;			// 銃弾の装填数のゲージ
		int hImageGaugeRemaining;	// 残弾数を示すゲージ
		float reloadTimer;			// リロード時間を管理するためのタイマー
		float reloadTime;			// リロードにかかる時間
		float coolDownTimer;		// 連射を防ぐためのタイマー
		float coolDownTime;			// 次に撃てるようになるまでの時間
	};

public:
	Gun();
	~Gun() override;
	void Update() override;
	void Draw() override;

	void DrawRemainingSetting();
	bool OutBullet();
	void OutBulletEffect();
	bool ReloadBullet(); // true → リロード処理ができた
	void AddBurrelt(int addNumber);
	void SetGunType(GUN::TYPE type);

	int GetAttack();
	float GetReloadTimer(); // 残りのリロード時間
	float GetReloadTime();	// 銃のリロードにかかる時間

private:
	GUN::TYPE gunType_;

	struct gun current_;
	struct gun hand_;
	struct gun machine_;

	int hImageEffectOutBullet_; // 発砲後に画面を光らせるための画像
};

