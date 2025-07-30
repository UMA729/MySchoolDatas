#include "Player.h"

CPlayer::CPlayer()
{
	vec.x = 0;
	vec.y = 0;
	pos.x = WINDOW_WIDTH / 2;
	pos.y = WINDOW_HEIGHT / 0.5f;
	HP = 3;
	ID = PLAYER;
}

int CPlayer::Action(list<unique_ptr<Base>>& base)
{
	return 0;
}

void CPlayer::Draw()
{
	DrawCircle(pos.x, pos.y, radius, GetColor(255, 255, 255), false);
}