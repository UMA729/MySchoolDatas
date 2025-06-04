#pragma once
#include "objBase.h"

class CSpring_Circle :public Base {
public:
	CSpring_Circle();

	int Action(list<unique_ptr<Base>>&);
	void Draw();

	//�΂˂̒���
	float spring_natural_length{ 200 };
	//�΂˂̒����i�L�т�����:x�j
	float spring_length{ 0 };
	//�΂˂̋���
	float k{ 0.1f };
	//�΂˂̔�����
	float F{ 0.0f };
	//�΂˂̌�����
	float damp{ 0.9f };
	//�I�u�W�F�N�g�̎���
	float mass{ 3.0f };
	//�����x
	float acc{ 0.0f };
};