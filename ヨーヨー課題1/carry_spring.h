#pragma once
#include "objBase.h"

class CCarry_Spring :public Base {
public:
	CCarry_Spring();

	int Action(list<unique_ptr<Base>>&);
	void Draw();

	//���a
	int radius{ 0 };

	//�}�E�X�̈ʒu�܂ł̃x�N�g��
	Vector move_distance{ 0,0 };
	//�΂˂̒���
	float sprin;
	//�΂˂̌�����
	float DAMP{ 0.9f };
	//�����x
	float ACC{ 0.05f };

	Point thing_pos{ 0,0 };

	bool click;

	bool one_click;
};