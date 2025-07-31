//enemy.h
#pragma once
#include "objBase.h"
#include "main.h"

class CGoal :public Base
{
public:
	CGoal(int x, int y);

	//”¼Œa
	int radius;

	Vector max_vec{ 0,0 };

	Color c;

	bool shield = false;
	bool damege = false;

	int Action(list<unique_ptr<Base>>&);
	void Draw();
};