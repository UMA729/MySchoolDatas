//enemy.h
#pragma once
#include "objBase.h"
#include "main.h"

class CEnemy :public Base
{
public:
	CEnemy(int x, int y);

	//”¼Œa
	int radius;

	Vector max_vec{ 0.0 };

	int Action(list<unique_ptr<Base>>&);
	void Draw();
};