//circle.cpp
#include "circle.h"
#include "function.h"

#include "enemy.h"

//�d�͉����x
constexpr auto g = 0.98f;

CCircle::CCircle(int x, int y)
{
	radius = 32;
	//pos.x = x;
	//pos.y = y;

	pos.x = radius;
	pos.y = WINDOW_HEIGHT - radius;

	//vec.x = 4.0f;

	//�N���b�N���������̃x�N�g�������߂�
	vec.x = x - pos.x;
	vec.y = y - pos.y;

	vec = Vector_SetLength(vec, 30.0f);

	ID = PLAYER;
}

int CCircle::Action(list<unique_ptr<Base>>& base)
{
	clear_to_frame++;

	//vec.y += g;		//�����x�����Z

	pos.x += vec.x; //�d�͏d��
	pos.y += vec.y; //�d�͏d��

	//�����蔻��
	//�~����
	for (auto i = base.begin();i != base.end();i++){
		if ((*i)->ID == ENEMY) {
			int e_vecX = 0;
			int e_vecY = 0;
			float e_radius = ((CEnemy*)(*i).get())->radius;
			float distance = DistanceF(pos.x, pos.y, (*i)->pos.x, (*i)->pos.y);
			if (distance < radius + e_radius)
			{
				(*i)->vec.x = vec.x * 0.3f;
				(*i)->vec.y = vec.y * 0.3f;
				//�������Ă���
				GSCORE_POW += 1.1f;
				FLAG = false;
				GSCORE += (GCOMBO + 1) * GSCORE_POW;//�X�R�A���Z
				GCOMBO++;
				//�R���{���ŋ�����ς���
				if (GCOMBO == 10)
				{
					e_vecX=((CEnemy*)(*i).get())->now_vecX;
					e_vecY=((CEnemy*)(*i).get())->now_vecY;
					(*i)->vec.x = e_vecX*6;
					(*i)->vec.y = e_vecY*5;
				}
				if (GCOMBO == 30)
				{
					(*i)->vec.x += 0;
				}
			}
		}
	}

	//��ʂ̉��Œ��˕Ԃ�
	if (pos.x<radius || pos.x>WINDOW_WIDTH - radius && FLAG != false)
	{
		FLAG = false;
		GCOMBO = 0;
		GSCORE_POW = 1.0f;
	}

	//��ʂ̉��Œ��˕Ԃ�
	if (pos.y > WINDOW_HEIGHT - radius && FLAG != false)
	{
		FLAG = false;
		GCOMBO = 0;
		GSCORE_POW = 1.0f;
	}
	//��ʂ̊O�ɏo����폜
	//if (pos.y > WINDOW_HEIGHT + radius * 2)FLAG = false;


	return 0;
}

void CCircle::Draw() {
	DrawCircle(
		pos.x, pos.y,			 //���S���W
		radius,					 //���a
		GetColor(255,255,255), //�F
		false					 //�h��Ԃ�(true/false)
	);
}