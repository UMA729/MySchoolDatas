#pragma once
#include "objBase.h"
#include "main.h"

class CEnemyD :public Base
{
public:

	CEnemyD();

	int Action(list<unique_ptr<Base>>&);

	void Draw();

	float speed;

	Point tri_pos1;
	Point tri_pos2;
	Point tri_pos3;
};