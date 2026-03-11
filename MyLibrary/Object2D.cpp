#include "Object2D.h"

Object2D::Object2D()
{
}

Object2D::Object2D(VECTOR2 position, int hImage)
{
	position_ = position;
	hImage_ = hImage;
}

Object2D::~Object2D()
{
	if (hImage_ > 0)
	{
		hImage_ = -1;
	}
}

void Object2D::Update()
{
}

void Object2D::Draw()
{
}
