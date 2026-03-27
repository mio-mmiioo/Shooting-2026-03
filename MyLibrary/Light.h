#pragma once
#include "DxLib.h"
#include <vector>

namespace Light
{
	void Init();
	void Update();
	void Release();
	VECTOR GetLightDirection();
	void SetPosition(VECTOR position);
}
