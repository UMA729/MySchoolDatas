#pragma once
#include "objBase.h"

class CSpring_Circle3 :public Base {
public:
	CSpring_Circle3();

	int Action(list<unique_ptr<Base>>&);
	void Draw();

	//�}�E�X�̈ʒu�܂ł̃x�N�g��
	Vector move_distance{ 0,0 };
	//�΂˂̒���
	float sprin;
	//�΂˂̌�����
	float DAMP{ 0.9f };
	//�����x
	float ACC{ 0.05f };
};