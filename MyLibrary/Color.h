#pragma once
#include "DxLib.h"

namespace Color
{
	const int BLACK = GetColor(0, 0, 0);
	const int RED = GetColor(255, 0, 0);
	const int WHITE = GetColor(255, 255, 255);
	const int BLUE = GetColor(0, 0, 255);

	// テキスト
	const int TEXT = WHITE;

	// WayInfo/StageSearchで使用
	const int EMPTY = GetColor(100, 255, 100);
	const int WALL = BLACK;
	const int BRANCH = BLUE;
	const int OTHER = GetColor(100, 100, 100);
	const int SHORTEST_WAY = RED;
}
