//circle.cpp
#include "enemy2.h"
#include "function.h"

CEnemy2::CEnemy2()
{
	radius = 32;
	pos.x = 32;
	pos.y = WINDOW_HEIGHT / 2 - 64;
	vec.x = 5;


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
		GetColor(120, 120, 0), //F
		false					 //“h‚è‚Â‚Ô‚µ(true/false)
	);
}