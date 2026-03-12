#pragma once
#include "../Library/GameObject.h"

static VECTOR3 LOOK_HEIGHT = { 0.0f, 180.0f, 0.0f }; // 視点の高さ

enum OBJECT_SORT
{
	OBJ_PLAYER,
	OBJ_CHARA,
	OBJ_OBJECT,
	OBJ_OBJECT_D,
	MAX_OBJ
};

class Transform
{
public:
	VECTOR3 position_;	// 位置
	VECTOR3 rotation_;	// 回転
	VECTOR3 scale_;		// 軸ごとの拡縮

	Transform() {
		position_ = VECTOR3(0, 0, 0);
		rotation_ = VECTOR3(0, 0, 0);
		scale_ = VECTOR3(1, 1, 1);
		rotMatrix_ = MGetIdent();
		localMatrix_ = MGetIdent();
	}

	~Transform() {};

	// 位置・回転・拡縮を使用した行列の計算結果を求める
	const MATRIX& MakeLocalMatrix() {
		MATRIX ms = MGetScale(scale_);
		MATRIX mrx = MGetRotX(rotation_.x);
		MATRIX mry = MGetRotY(rotation_.y);
		MATRIX mrz = MGetRotZ(rotation_.z);
		MATRIX mt = MGetTranslate(position_);
		rotMatrix_ = ms * mrz * mrx * mry;
		localMatrix_ = rotMatrix_ * mt;
		return localMatrix_;
	}

	// 位置・回転・拡縮を使用した行列を返す　※計算をしてない
	const MATRIX& GetLocalMatrix() const { return localMatrix_; }

	// 回転・拡縮を使用した行列を返す　※計算をしてない
	const MATRIX& GetRotationMatrix() const { return rotMatrix_; }

private:
	MATRIX rotMatrix_;
	MATRIX localMatrix_;
};

class Object3D : public GameObject 
{
public:
	Object3D();
	virtual ~Object3D();
	virtual void Update() override;
	virtual void Draw() override;
	Transform GetTransform() { return transform_; } // transformの値を返す
	int GetObjectNumber() { return objectNumber_; } // オブジェクトの種類の番号を返す
	float GetDistanceR() { return distanceR_; } // オブジェクトの当たり判定の半径を返す
	void SetParent(Object3D* parent) { parent_ = parent; }
	bool CollideLine(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit = nullptr) const;
	void SetPosition(VECTOR3 newPosition) { transform_.position_ = newPosition; }
	void SetMove(VECTOR3 toPosition); // 現在地からtoPositionの方へ移動する
	virtual void AddHp(int add) { hp_ += add; } // hpにaddを加算

protected:
	int hModel_; // 本当に描画するモデル
	int hitModel_; // 当たり判定用のモデル
	bool isDraw_; // 描画するかどうか true → 描画する
	Transform transform_;
	Object3D* parent_;

	float rotateSpeed_;	// 回転の速度
	float moveSpeed_;	// 移動速度
	int hp_;			// 体力
	int score_;			// 得点
	int objectNumber_;	// オブジェクトの識別番号

	float time_; // 空中にいる時間
	float gravity_; // 重力
	float velocityY_; // y方向に加わる力
	float distanceR_; // ものが当たる半径

	// 開発時のみ使用
	bool isOnGround_; // 足が地についているか true→ついている
};
