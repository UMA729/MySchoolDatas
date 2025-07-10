//circle.cpp
#include "circle.h"
#include "function.h"

#include "enemy.h"

//�d�͉����x
constexpr float g = 0.98f;

CCircle::CCircle()
{
	radius = 32;
	pos.x = radius;
	pos.y = WINDOW_HEIGHT - radius;
}
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

	vec.y += g;		//�����x�����Z

	pos.x += vec.x; //�d�͏d��
	pos.y += vec.y; //�d�͏d��

	//�����蔻��
	//�~����
	for (auto i = base.begin(); i != base.end(); i++) {
		if ((*i)->ID == ENEMY) {
			float e_radius = ((CEnemy*)(*i).get())->radius;

			float distance = DistanceF(pos.x, pos.y, (*i)->pos.x, (*i)->pos.y);
			if (distance < radius + e_radius)
			{
				FLAG = false;
				GSCORE++;
			}
		}

		//��ʂ̉�,���ŏ����l��
		if (pos.x<radius || pos.x>WINDOW_WIDTH - radius ||
			pos.y > WINDOW_HEIGHT - radius &&
			FLAG != false)
			//��ʂ̊O�ɏo����폜
			//if (pos.y > WINDOW_HEIGHT + radius * 2)FLAG = false;


			return 0;
	}
}

void CCircle::Draw() {
	DrawCircle(
		pos.x, pos.y,			 //���S���W
		radius,					 //���a
		GetColor(255,255,255), //�F
		false					 //�h��Ԃ�(true/false)
	);

}