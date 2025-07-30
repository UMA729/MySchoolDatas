#include "Player.h"

CPlayer::CPlayer()
{
	radius = 32;
	vec.x = 0;
	vec.y = 0;
	pos.x = WINDOW_WIDTH/3;
	pos.y = WINDOW_HEIGHT - radius;
	HP = 3;
	c = 0;
	AKey = false;
	ID = PLAYER;
}

int CPlayer::Action(list<unique_ptr<Base>>& base)
{

	if (HP == 0)
	{
		FLAG = false;
	}


	for (auto i = base.begin(); i != base.end(); i++)
	{

		if (CheckHitKey(KEY_INPUT_A))
		{
			if (AKey == false)
			{
				AKey = true;
				GChar[c] = 'A';
				c++;
			}
		}
		else
		{
			AKey = false;
		}
		if (CheckHitKey(KEY_INPUT_D))
		{
			if (DKey == false)
			{
				DKey = true;
				GChar[c] = 'D';
				c++;
			}
		}
		else
		{
			DKey = false;
		}

		if (pos.x - (*i)->pos.x > pos.x + 30)
		{
			FLAG = false;
		}
	}


	return 0;
}

void CPlayer::Draw()
{
	DrawCircle(pos.x, pos.y, radius, GetColor(255, 255, 255), false);
}