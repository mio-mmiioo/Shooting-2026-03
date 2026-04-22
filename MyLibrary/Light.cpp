#include "Light.h"
#include "Object3D.h"

#include "../ImGui/imgui.h"

namespace Light
{
	const VECTOR INIT_LIGHT_POSITION = { 0.0f, -2000.0f, 0.0f };
	const float RANGE = 2000.0f;
	const float ATTEND0 = 0.0f;
	const float ATTEND1 = 0.003f;
	const float ATTEND2 = 0.0f;

	VECTOR3 lightPosition = INIT_LIGHT_POSITION + (LOOK_HEIGHT / 2.0f);
	float range = RANGE;
	float atten0 = ATTEND0;
	float atten1 = ATTEND1;
	float atten2 = ATTEND2;
	VECTOR lightDirection; // 影をつけるのに使用しているライト
	int hPlayerLight; // プレイヤーの位置に配置するライトのハンドル
}

void Light::Init()
{
	lightDirection = { -1, -2, 1 };
	ChangeLightTypeDir(lightDirection);
	hPlayerLight = CreatePointLightHandle(lightPosition, range, atten0, atten1, atten2);
}

void Light::Update()
{
	SetLightPositionHandle(hPlayerLight, lightPosition);
	//SetLightRangeAttenHandle(hPlayerLight, range, atten0, atten1, atten2);
}

void Light::Release()
{
	DeleteLightHandleAll();
}

VECTOR Light::GetLightDirection()
{
	return lightDirection;
}

void Light::SetPosition(VECTOR position)
{
	lightPosition = position + (LOOK_HEIGHT / 2.0f);
}
