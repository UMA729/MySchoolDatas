#pragma once
#include "objBase.h"
#include "main.h"

class CCircle :public Base
{
public:
	CCircle();
	CCircle(int x, int y);
	int CMoveCir(int x, int y);

	 //”¼Œa
	 int radius;

	 int Action(list<unique_ptr<Base>>&);
	 void Draw();

};