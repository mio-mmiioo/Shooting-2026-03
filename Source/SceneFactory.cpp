#include "SceneFactory.h"
#include <windows.h>
#include <assert.h>
#include "BootScene.h"

// 追加したシーン
#include "TitleScene/TitleScene.h"
#include "MenuScene/MenuScene.h"
#include "PlayScene/PlayScene.h"
#include "ResultScene/ResultScene.h"

SceneBase* SceneFactory::CreateFirst()
{
	return new BootScene();
}

SceneBase * SceneFactory::Create(const std::string & name)
{
	if (name == "TITLE")
	{
		return new TitleScene();
	}
	if (name == "MENU")
	{
		return new MenuScene();
	}
	if (name == "PLAY")
	{
		return new PlayScene();
	}
	if (name == "RESULT")
	{
		return new ResultScene();
	}
	MessageBox(NULL, ("次のシーンはありません\n" + name).c_str(), "SceneFactory", MB_ICONERROR | MB_OK);
	assert(false);
	return nullptr;
}
