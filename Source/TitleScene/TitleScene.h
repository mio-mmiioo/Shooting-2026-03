#pragma once
#include "../../Library/SceneBase.h"
#include "../../MyLibrary/ClickArea.h"
#include "../Data.h"

class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();
	void Update() override;
	void Draw() override;

private:
	Data::image aiming_;
	Data::image startText_;
};
