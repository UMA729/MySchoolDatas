#pragma once
#include "spring_circle2.h"

CSpring_Circle2::CSpring_Circle2() {
	pos.x = WINDOW_WIDTH / 2;
	pos.y = WINDOW_HEIGHT / 2;
}

int CSpring_Circle2::Action(list<unique_ptr<Base>>&) {
	int x, y;
	GetMousePoint(&x, &y);//�}�E�X�̈ʒu�擾

	//�N���b�N�����ʒu��y���W���΂˂̏I�_�ɂ���
	{
		if (GetMouseInput() & MOUSE_INPUT_LEFT) {
			pos.x = x;
			pos.y = y;
		}
	}

	//������
	spring_length.x = WINDOW_WIDTH / 2 - pos.x;
	F.x = k * spring_length.x;
	acc.x = F.x / mass;
	vec.x = damp * (vec.x + acc.x);

	//�c����
	spring_length.y = WINDOW_HEIGHT / 2 - pos.y;
	F.y = k * spring_length.y;
	acc.y = F.y / mass;
	vec.y = damp * (vec.y + acc.y);

	//���W�X�V
	pos.x += vec.x;
	pos.y += vec.y;
	return 0;
}

void CSpring_Circle2::Draw() {
	//�΂˂̕`��
	DrawLine(WINDOW_WIDTH / 2, WINDOW_HEIGHT/2, pos.x, pos.y, GetColor(255, 255, 255));
	//�΂˂̏I�_�Ƀ{�[����`��
	DrawCircle(
		pos.x, pos.y,
		32,
		GetColor(255, 0, 0),
		true
	);
}