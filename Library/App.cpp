#include "App.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Time.h"
#include "../Source/Data.h"
#include "../MyLibrary/Input.h"
#include <DxLib.h>

void AppInit()
{
	ObjectManager::Init();
	SceneManager::Init();
	Time::Init();
	Input::InitActionMap();
	Data::Init();
}

void AppUpdate()
{
	Input::StateUpdate();
	SceneManager::Update();
	ObjectManager::Update();
	//Shadow::Update(); // オブジェクトを描画する前に処理する
}

void AppDraw()
{
	Time::Refresh();
	ObjectManager::Draw();
	//Shadow::Draw(); // オブジェクトを描画後に処理する
	SceneManager::Draw();
}

void AppRelease()
{
	Time::Release();
	SceneManager::Release();
	ObjectManager::Release();
}

bool AppIsExit()
{
	return SceneManager::IsExit();
}