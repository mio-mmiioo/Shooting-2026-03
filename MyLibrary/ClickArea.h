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
	bool IsMosueKeepInArea(area a); // 範囲内にマウスカーソルがある
	bool IsMouseInArea(area a); // 範囲内にマウスカーソルが入った瞬間
	bool IsMouseInArea(area a, int mouseX, int mouseY); // 範囲内にマウスカーソルが入った瞬間( マウスの座標指定あり )
	void DrawArea(area a); // 指定した範囲の描画
}

class Button
{
public:
	Button(area normal, area select);
	~Button();
	void Update();
	void Draw();
	bool GetIsClickArea() const { return isClickArea_; } // true → マウスで指定した範囲をクリック
	bool GetIsKeepOnArea() const { return isOnArea_; } // true → マウスで指定した範囲上にいる
	bool GetIsOnArea(); // true → マウスで指定した範囲上にいる

	int GetNormalGraph() const { return normal_.hImage; } // 通常時の画像を取得
	int GetSelectGraph() const { return select_.hImage; } // 選択時の画像を取得

private:
	area normal_;		// 通常時
	area select_;		// 選択中
	bool isClickArea_;	// ボタンが押された
	bool isOnArea_;		// ボタン上にある
};
