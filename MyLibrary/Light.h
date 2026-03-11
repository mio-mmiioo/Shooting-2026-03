#pragma once
#include "DxLib.h"
#include <vector>

namespace Light
{
	void Init();
	void Update();
	VECTOR GetLightDirection();
	void SetPosition(VECTOR position);
}
