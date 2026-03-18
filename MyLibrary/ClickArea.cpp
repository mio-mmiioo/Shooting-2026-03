#include "ClickArea.h"
#include "Input.h"

bool ClickArea::IsMosueInArea(area a)
{
	int x = (int)Input::GetMousePosition().x;
	int y = (int)Input::GetMousePosition().y;

	if (a.leftTop.x < x && x < a.rightDown.x)
	{
		if (a.leftTop.y < y && y < a.rightDown.y)
		{
			return true;
		}
	}
	return false;
}

void ClickArea::SetClickArea(button* b)
{
	if (b == nullptr)
	{
		return;
	}
	if (IsMosueInArea(b->buttonArea) == true)
	{
		b->isClickArea = !(b->isClickArea);
	}
}

void ClickArea::DrawArea(area a, int color)
{
	int x1 = a.leftTop.x;
	int y1 = a.leftTop.y;
	int x2 = a.rightDown.x;
	int y2 = a.rightDown.y;
	if (a.hImage == -1)
	{
		DrawBox(x1, y1, x2, y2, color, TRUE);
	}
	else
	{
		DrawExtendGraph(x1, y1, x2, y2, a.hImage, TRUE);
	}
}
