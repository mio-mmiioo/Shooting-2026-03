#include "ClickArea.h"
#include "Input.h"

bool ClickArea::IsMosueKeepInArea(area a)
{
	int x = (int)Input::GetMousePosition().x;
	int y = (int)Input::GetMousePosition().y;

	if (IsMouseInArea(a, x, y) == true)
	{
		return true;
	}
	return false;
}

bool ClickArea::IsMouseInArea(area a)
{
	int prevX = (int)Input::GetPrevMousePosition().x;
	int prevY = (int)Input::GetPrevMousePosition().y;
	int x = (int)Input::GetMousePosition().x;
	int y = (int)Input::GetMousePosition().y;

	if (IsMouseInArea(a, prevX, prevY) == false && IsMouseInArea(a, x, y))
	{
		return true;
	}

	return false;
}

bool ClickArea::IsMouseInArea(area a, int mouseX, int mouseY)
{
	if (a.leftTop.x < mouseX && mouseX < a.rightDown.x)
	{
		if (a.leftTop.y < mouseY && mouseY < a.rightDown.y)
		{
			return true;
		}
	}
	return false;
}

void ClickArea::DrawArea(area a)
{
	int x1 = (int)a.leftTop.x;
	int y1 = (int)a.leftTop.y;
	int x2 = (int)a.rightDown.x;
	int y2 = (int)a.rightDown.y;

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

	// マウスがボタン上にある
	if (ClickArea::IsMosueKeepInArea(normal_) == true)
	{
		isOnArea_ = true;
	}

	if (Input::IsKeyDown("outBullet"))
	{
		if (isOnArea_ == true)
		{
			isClickArea_ = !isClickArea_;
		}
	}
}

void Button::Draw() const
{
	// 選択中
	if (isOnArea_ == true)
	{
		int x1 = (int)select_.leftTop.x;
		int y1 = (int)select_.leftTop.y;
		int x2 = (int)select_.rightDown.x;
		int y2 = (int)select_.rightDown.y;
		DrawExtendGraph(x1, y1, x2, y2, select_.hImage, TRUE);
	}
	// それ以外
	else
	{
		int x1 = (int)normal_.leftTop.x;
		int y1 = (int)normal_.leftTop.y;
		int x2 = (int)normal_.rightDown.x;
		int y2 = (int)normal_.rightDown.y;
		DrawExtendGraph(x1, y1, x2, y2, normal_.hImage, TRUE);
	}
}

bool Button::GetIsOnArea() const
{
	if (ClickArea::IsMouseInArea(normal_) == true)
	{
		return true;
	}
	return false;
}
