//enemy.h
#pragma once
#include "objBase.h"
#include "main.h"

class CEnemy2 :public Base
{
public:
	CEnemy2();

	//半径
	int radius;

	int Action(list<unique_ptr<Base>>&);
	void Draw();

	//消えるまでのフレーム時間
	int clear_to_frame{ 0 };
};