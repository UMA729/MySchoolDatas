#pragma once
#include "spring_circle3.h"

CSpring_Circle3::CSpring_Circle3() {
	pos.x = WINDOW_WIDTH / 2;
	pos.y = WINDOW_HEIGHT / 2;
}

int CSpring_Circle3::Action(list<unique_ptr<Base>>&) {
	int x, y;
	GetMousePoint(&x, &y);//�}�E�X�̈ʒu�擾

	//�}�E�X�̈ʒu�܂ł̋���
	move_distance.x = x - pos.x;
	move_distance.y = y - pos.y;
	//�����x�x�N�g��
	Vector acc{ 0,0 };
	acc.x = ACC * move_distance.x;
	acc.y = ACC * move_distance.y;
	return 0;
}

void CSpring_Circle3::Draw() {
	//�΂˂̕`��
	DrawLine(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, pos.x, pos.y, GetColor(255, 255, 255));
	//�΂˂̏I�_�Ƀ{�[����`��
	DrawCircle(
		pos.x, pos.y,
		32,
		GetColor(255, 0, 0),
		true
	);
}