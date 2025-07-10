//circle.cpp
#include "carry_thing.h"
#include "function.h"
#include "goal.h"

//�d�͉����x
constexpr float g = 0.98f;

CCarry_thing::CCarry_thing()
{
	radius = 32;
	pos.x = WINDOW_WIDTH / 2;
	pos.y = WINDOW_HEIGHT / 2;
	ID = PLAYER;
}
CCarry_thing::CCarry_thing(int x, int y)
{
	radius = 32;

	pos.x = radius;
	pos.y = WINDOW_HEIGHT - radius;

	ID = PLAYER;
}

int CCarry_thing::Action(list<unique_ptr<Base>>& base)
{
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
		pos.x,pos.y,			 //���S���W
		radius,					 //���a
		GetColor(255,255,255), //�F
		false					 //�h��Ԃ�(true/false)
	);

}