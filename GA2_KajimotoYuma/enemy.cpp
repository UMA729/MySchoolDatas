//circle.cpp
#include "enemy.h"
#include "function.h"
#include "circle.h"

CEnemy::CEnemy(int x, int y)
{
	radius = 32;
	pos.x = x;
	pos.y = y;
	vec.x = DEF_X;
	vec.y = DEF_Y;
	ID = ENEMY;
}

int CEnemy::Action(list<unique_ptr<Base>>& base)
{
	pos.x += vec.x;
	pos.y += vec.y;
	//��ʂ̉��Œ��˕Ԃ�
	if (pos.x<radius || pos.x>WINDOW_WIDTH - radius)
	{
		vec.x = -vec.x;
	}
	//��ʂ̉��Œ��˕Ԃ�
	if (pos.y > WINDOW_HEIGHT - radius ||pos.y < radius)
	{
		vec.y = -vec.y;
	}

	for (auto i = base.begin(); i != base.end(); i++)
	{
		if ((*i)->ID == PLAYER)
		{

			int Pobj_frame = ((CCircle*)(*i).get())->clear_to_frame;
			int P_radius = ((CCircle*)(*i).get())->radius;
			//�����l��
			if (Pobj_frame == 60)
			{
				now_vecX = vec.x / abs(vec.x);
				now_vecY = vec.x / abs(vec.y);

				//���݂̈ړ�����
				vec.x = now_vecX * DEF_X;
				vec.y = now_vecY * DEF_Y;
			}
		}
	}
	return 0;
}



void CEnemy::Draw() {
	DrawCircle(
		pos.x, pos.y,			 //���S���W
		radius,					 //���a
		GetColor(255, 0, 0), //�F
		false					 //�h��Ԃ�(true/false)
	);
}