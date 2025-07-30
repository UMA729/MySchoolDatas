#include "Enemy_A.h"

CEnemyA::CEnemyA()
{
	radius = 32;
	pos.x = WINDOW_WIDTH - (radius+1);
	pos.y = WINDOW_HEIGHT - (radius+1);
	vec.x = 0;
	vec.y = 0;
	ID = ENEMYA;
}

int CEnemyA::Action(list<unique_ptr<Base>>& base)
{
	return 0;
}

void CEnemyA::Draw()
{
	DrawCircle(pos.x, pos.y, radius, GetColor(255, 255, 255), false);
}