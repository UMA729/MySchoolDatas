//circle.cpp
#include "enemy2.h"
#include "function.h"

CEnemy2::CEnemy2(int x, int y)
{
	radius = 32;
	pos.x = x;
	pos.y = y;
	ID = ENEMY;
}

int CEnemy2::Action(list<unique_ptr<Base>>& base)
{
	return 0;
}



void CEnemy2::Draw() {
	DrawCircle(
		pos.x, pos.y,			 //’†SÀ•W
		radius,					 //”¼Œa
		GetColor(255, 0, 0), //F
		false					 //“h‚è‚Â‚Ô‚µ(true/false)
	);
}