//circle.cpp
#include "carry_thing.h"
#include "function.h"
#include "goal.h"

//重力加速度
constexpr float g = 0.98f;

CCarry_thing::CCarry_thing()
{
	radius = 60;
	pos.x = WINDOW_WIDTH / 2;
	pos.y = WINDOW_HEIGHT / 2;
	t_start = true;
	ID = THING;
}
CCarry_thing::CCarry_thing(int x, int y)
{
	radius = 60;

	pos.x = radius;
	pos.y = WINDOW_HEIGHT - radius;
	t_start = true;
	ID = THING;
}

int CCarry_thing::Action(list<unique_ptr<Base>>& base)
{
	if (t_start == true)
	{
		
		if (pos.x > WINDOW_WIDTH / 8)
		{
			int x, y;
			GetMousePoint(&x, &y);
			x = WINDOW_WIDTH / 2;
			y = WINDOW_HEIGHT / 2;
			if(radius != 32)
			radius -= 0.01;
			pos.x -= 5;
			x = pos.x;
			SetMousePoint(x, y);
		}
		else
		{
			t_start = false;
		}
	}
	//画面外制御(横)
	if (pos.x < radius)
		pos.x = radius;
	else if ( pos.x > WINDOW_WIDTH - radius)
		pos.x = WINDOW_WIDTH - radius-1;
	//画面外制御(縦)
	if ( pos.y < radius)
		pos.y = radius;
	else if ( pos.y > WINDOW_HEIGHT - radius)
		pos.y = WINDOW_HEIGHT - radius-1;

	for (auto i = base.begin(); i != base.end(); i++)
	{
		if ((*i)->ID == PLAYER)
		{
			spring_pos = (*i)->pos;
		}
	}

	return 0;
}

void CCarry_thing::Draw() {
	DrawCircle(
		pos.x,pos.y,			 //中心座標
		radius,					 //半径
		GetColor(255,255,255), //色
		false					 //塗りつぶし(true/false)
	);

}