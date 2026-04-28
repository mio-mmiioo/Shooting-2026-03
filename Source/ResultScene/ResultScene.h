#pragma once
#include "../../Library/SceneBase.h"
#include "../Data.h"

class Button;

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

	Button* titleButton_;
	float timer_;
};
