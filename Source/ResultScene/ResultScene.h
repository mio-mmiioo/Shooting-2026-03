#pragma once
#include "../../Library/SceneBase.h"
#include "../../MyLibrary/ClickArea.h"
#include "../Data.h"

class ResultScene : public SceneBase
{
public:
	ResultScene();
	~ResultScene();
	void Update() override;
	void Draw() override;
private:
	Data::image aiming_;
	Data::image hitAiming_;

	button titleButton_;
	float timer_;
};
