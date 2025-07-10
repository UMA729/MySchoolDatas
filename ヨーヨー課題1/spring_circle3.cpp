#pragma once
#include "spring_circle3.h"

CSpring_Circle3::CSpring_Circle3() {
	pos.x = WINDOW_WIDTH / 2;
	pos.y = WINDOW_HEIGHT / 2;
}

int CSpring_Circle3::Action(list<unique_ptr<Base>>&) {
	int x, y;
	GetMousePoint(&x, &y);//�}�E�X�̈ʒu�擾



	////��ʊO����(��)
	//if (x < 32 && pos.x < 32)
	//	pos.x = 32;
	//else if (x > WINDOW_WIDTH - 32 && pos.x > WINDOW_WIDTH - 32)
	//	pos.x = WINDOW_WIDTH - 32;
	////��ʊO����(�c)
	//if (y < 32 && pos.y < 32)
	//	pos.y = 32;
	//else if (y > WINDOW_HEIGHT - 32 && pos.y > WINDOW_HEIGHT - 32)
	//	pos.y = WINDOW_HEIGHT - 32;


	//�}�E�X�̈ʒu�܂ł̋���
	move_distance.x = x - pos.x;
	move_distance.y = y - pos.y;
	//�����x�x�N�g��
	Vector acc{ 0,0 };
	acc.x = ACC * move_distance.x;
	acc.y = ACC * move_distance.y;
	vec.x = DAMP * (acc.x + vec.x);
	vec.y = DAMP * (acc.y + vec.y);


	//���W�X�V
	pos.x += vec.x;
	pos.y += vec.y;

	return 0;
}

void CSpring_Circle3::Draw() {
	//�΂˂̕`��
	DrawLine(pos.x,pos.y, pos.x, pos.y, GetColor(255, 255, 255));
	//�΂˂̏I�_�Ƀ{�[����`��
	DrawCircle(
		pos.x , pos.y,
		32,
		GetColor(255, 0, 0),
		true
	);
}