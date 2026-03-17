#pragma once
#include "../Library/GameObject.h"

struct area
{
	VECTOR2 leftTop; // 左上の座標
	VECTOR2 rightDown; // 右下の座標
	int hImage; // 画像
	int color;	// 色
};

struct button
{
	area buttonArea; // ボタンの範囲
	//area clickedArea; // ボタンを押した後に展開される範囲
	bool isClickArea;
};

namespace ClickArea
{
	bool IsMosueInArea(area a);
	void SetClickArea(button* b);
	void DrawArea(area a, int color);
}
