//enemy.h
#pragma once
#include "objBase.h"
#include "main.h"

class CEnemy2 :public Base
{
public:
	CEnemy2();

	//���a
	int radius;

	int Action(list<unique_ptr<Base>>&);
	void Draw();

	//������܂ł̃t���[������
	int clear_to_frame{ 0 };
};