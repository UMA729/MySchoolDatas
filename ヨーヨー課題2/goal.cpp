//circle.cpp
#include "goal.h"
#include "function.h"
#include "carry_thing.h"

CGoal::CGoal(int x, int y)
{
	radius = 32;
	pos.x = x;
	pos.y = y;
	c_r = 255;
	c_b = 0;
	c_g = 0;
	ID = ENEMY;
}

int CGoal::Action(list<unique_ptr<Base>>& base)
{

	return 0;
}



void CGoal::Draw() {
	DrawCircle(
		pos.x, pos.y,			 //中心座標
		radius,					 //半径
		GetColor(c_r, c_g, c_b), //色
		false					 //塗りつぶし(true/false)
	);
}