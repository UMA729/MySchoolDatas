#include "Enemy_D.h"

CEnemyD::CEnemyD()
{
	radius = 32;
	pos.x = WINDOW_WIDTH;
	pos.y = WINDOW_HEIGHT;
	vec.x = 0;
	vec.y = 0;
	speed = 10;
	ID = ENEMYD;
}
int CEnemyD::Action(list<unique_ptr<Base>>& base)
{
	pos.x--;


	return 0;
}

void CEnemyD::Draw()
{
	DrawTriangle(
		pos.x - 64, pos.y - 2,
		pos.x,		pos.y - 2,
		pos.x - 32, pos.y - 66,
		GetColor(255, 255, 255)
		, false);
}