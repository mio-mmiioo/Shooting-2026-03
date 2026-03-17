#pragma once
#include "DxLib.h"
#include <string>

namespace Input
{
	void InitActionMap();							// 入力処理の初期化
	void StateUpdate();								// 状態を毎フレーム更新する
	bool IsKeyDown(const std::string& action);		// 押された瞬間→true
	bool IsKeyKeepDown(const std::string& action);	// 押し続けている→true
	bool IsKeyUp(const std::string& action);		// 離された瞬間

	VECTOR GetMousePosition(); // マウスの座標を入手する
}