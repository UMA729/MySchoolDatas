//circle.cpp
#include "enemy2.h"
#include "function.h"

CEnemy2::CEnemy2(int x, int y)
{
	radius = 32;
	pos.x = x;
	pos.y = y;
	vec.y = 20;
	ID = ENEMY;
}

int CEnemy2::Action(list<unique_ptr<Base>>& base)
{
	if (GCOMBO < 30)
	{
		Gravity = 0.49f;
	}
	
	pos.y += vec.y;
	vec.y -= Gravity;

	//��ʂ̉��Œ��˕Ԃ�
	if (pos.x<radius / 2 || pos.x>WINDOW_WIDTH - radius / 2)
	{
		FLAG = false;
	}

	return 0;
}



void CEnemy2::Draw() {
	DrawCircle(
		pos.x, pos.y,			 //���S���W
		radius,					 //���a
		GetColor(120, 120, 0), //�F
		false					 //�h��Ԃ�(true/false)
	);
}