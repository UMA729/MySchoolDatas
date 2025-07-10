//�΂˂̊�b
#pragma once
#include "spring_circle.h"

CSpring_Circle::CSpring_Circle() {
	pos.x = WINDOW_WIDTH / 2;
	pos.y = spring_natural_length;
}

int CSpring_Circle::Action(list<unique_ptr<Base>>&) {
	int x, y;
	GetMousePoint(&x, &y);//�}�E�X�̈ʒu�擾
	
	//�N���b�N�����ʒu��y���W���΂˂̏I�_�ɂ���
	{
		if (GetMouseInput() & MOUSE_INPUT_LEFT) {
			pos.y = y;
		}
	}

	//�΂��̐L�т��������v�Z
	spring_length = pos.y - spring_natural_length;
	//�΂˂̗͂��v�Z
	F = -k * spring_length;
	acc = F / mass;//�d���ɂ������x���v�Z
	vec.y = damp * (vec.y + acc);//�������{�d���ɂ������x���l��
	//vec.y = damp * (vec.y + F);//���������l�������ꍇ
	//vec.y += F;

	//���W�X�V
	pos.y += vec.y;
	return 0;
}

void CSpring_Circle::Draw() {
	//�΂˂̕`��
	DrawLine(WINDOW_WIDTH / 2, 0, pos.x, pos.y, GetColor(255, 255, 255));
	//�΂˂̏I�_�Ƀ{�[����`��
	DrawCircle(
		pos.x, pos.y,
		32,
		GetColor(255, 0, 0),
		true
	);
}