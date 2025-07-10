#pragma once
#include "objBase.h"

class CSpring_Circle2 :public Base {
public:
	CSpring_Circle2();

	int Action(list<unique_ptr<Base>>&);
	void Draw();

	//�΂˂̒���
	float spring_natural_length{ 200 };
	//�΂˂̒����i�L�т�����:x�j
	Vector spring_length{ 0,0 };
	//�΂˂̋���
	float k{ 0.1f };
	//�΂˂̔�����
	Vector F{ 0.0f,0.0f };
	//�΂˂̌�����
	float damp{ 0.9f };
	//�I�u�W�F�N�g�̎���
	float mass{ 3.0f };
	//�����x
	Vector acc{ 0.0f,0.0f };
};