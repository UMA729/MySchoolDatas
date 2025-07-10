#pragma once
#include "objBase.h"
#include "main.h"

class CCarry_thing :public Base
{
public:
	CCarry_thing();
	CCarry_thing(int x, int y);

	 //”¼Œa
	 int radius;

	 int Action(list<unique_ptr<Base>>&);
	 void Draw();

	 Point spring_pos{ 0,0 };
};