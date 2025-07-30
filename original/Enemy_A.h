#pragma once
#include "objBase.h"
#include "main.h"

class CEnemyA :public Base
{
public:

	CEnemyA();

	int Action(list<unique_ptr<Base>>&);

	void Draw();

	float speed;
};