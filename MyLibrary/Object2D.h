#pragma once
#include "../Library/GameObject.h"

class Object2D : public GameObject
{
public:
	Object2D();
	Object2D(VECTOR2 position, int hImage);
	virtual ~Object2D();
	virtual void Update() override;
	virtual void Draw() override;
	VECTOR2 GetPosition() { return position_; }
private:
	int hImage_;
	VECTOR2 position_;
};