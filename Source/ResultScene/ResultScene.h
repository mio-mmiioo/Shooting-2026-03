#pragma once
#include "../../Library/SceneBase.h"

class ResultScene : public SceneBase
{
public:
	ResultScene();
	~ResultScene();
	void Update() override;
	void Draw() override;
};
