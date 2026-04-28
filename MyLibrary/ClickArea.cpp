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

void ClickArea::DrawArea(area a)
{
	int x1 = a.leftTop.x;
	int y1 = a.leftTop.y;
	int x2 = a.rightDown.x;
	int y2 = a.rightDown.y;

	DrawExtendGraph(x1, y1, x2, y2, a.hImage, TRUE);
}

Button::Button(area normal, area select)
{
	normal_ = normal;
	select_ = select;
	isClickArea_ = false;
	isOnArea_ = false;
}

Button::~Button()
{
}

void Button::Update()
{
	isOnArea_ = false; // 一度情報をリセット

	int x = (int)Input::GetMousePosition().x;
	int y = (int)Input::GetMousePosition().y;

	// マウスがボタン上にある
	if (normal_.leftTop.x < x && x < normal_.rightDown.x)
	{
		if (normal_.leftTop.y < y&& y < normal_.rightDown.y)
		{
			isOnArea_ = true;
		}
	}

	if (Input::IsKeyDown("outBullet"))
	{
		if (isOnArea_ == true)
		{
			isClickArea_ = !isClickArea_;
		}
	}
}

void Button::Draw()
{
	// 選択中
	if (isOnArea_ == true)
	{
		int x1 = select_.leftTop.x;
		int y1 = select_.leftTop.y;
		int x2 = select_.rightDown.x;
		int y2 = select_.rightDown.y;
		DrawExtendGraph(x1, y1, x2, y2, select_.hImage, TRUE);
	}
	// それ以外
	else
	{
		int x1 = normal_.leftTop.x;
		int y1 = normal_.leftTop.y;
		int x2 = normal_.rightDown.x;
		int y2 = normal_.rightDown.y;
		DrawExtendGraph(x1, y1, x2, y2, normal_.hImage, TRUE);
	}
}
