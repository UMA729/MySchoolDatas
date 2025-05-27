//circle.cpp
#include "enemy.h"
#include "function.h"
#include "circle.h"

CEnemy::CEnemy(int x, int y)
{
	radius = 32;
	pos.x = x;
	pos.y = y;
	vec.y = 20;
	ID = ENEMY;
}

int CEnemy::Action(list<unique_ptr<Base>>& base)
{

	if (GCOMBO < 30)
	{
		Gravity = 0.49f;
	}
	pos.y -= vec.y;

	vec.y -= Gravity;


	//‰æ–Ê‚Ì‰¡‚Å’µ‚Ë•Ô‚é
	if (pos.y < radius / 2 || pos.y > WINDOW_HEIGHT - radius / 2)
	{
		FLAG = false;
	}

	return 0;
}



void CEnemy::Draw() {
	DrawCircle(
		pos.x, pos.y,			 //’†SÀ•W
		radius,					 //”¼Œa
		GetColor(255, 0, 0), //F
		false					 //“h‚è‚Â‚Ô‚µ(true/false)
	);
}