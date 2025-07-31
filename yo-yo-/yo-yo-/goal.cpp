//circle.cpp
#include "goal.h"
#include "function.h"
#include "carry_thing.h"

CGoal::CGoal(int x, int y)
{
	radius = 32;
	pos.x = WINDOW_WIDTH-100;
	pos.y = WINDOW_HEIGHT/2;
	c.r = 255;
	c.g = 0;
	c.b = 0;
	ID = ENEMY;
}

int CGoal::Action(list<unique_ptr<Base>>& base)
{


	return 0;
}



void CGoal::Draw() {
	DrawCircle(
		pos.x, pos.y,			 //íÜêSç¿ïW
		radius,					 //îºåa
		GetColor(c.r, c.g, c.b), //êF
		false					 //ìhÇËÇ¬Ç‘Çµ(true/false)
	);
}