#pragma once
#include "../Library/GameObject.h"

struct area
{
	VECTOR2 leftTop; // 左上の座標
	VECTOR2 rightDown; // 右下の座標
	int hImage; // 画像
};

namespace ClickArea
{
	bool IsMosueInArea(area a);
	void DrawArea(area a);
}

class Button
{
public:
	Button(area normal, area select);
	~Button();
	void Update();
	void Draw();
	bool GetIsClickArea() { return isClickArea_; } // true → マウスで指定した範囲をクリック
	bool GetIsOnArea() { return isOnArea_; } // true → マウスで指定した範囲上にいる

private:
	area normal_;		// 通常時
	area select_;		// 選択中
	bool isClickArea_;	// ボタンが押された
	bool isOnArea_;		// ボタン上にある
};
