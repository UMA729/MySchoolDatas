#pragma once
#include "objBase.h"
#include "main.h"

class CPlayer :public Base
{
public:
	
	CPlayer();
	
	int Action(list<unique_ptr<Base>>&);

	void Draw();

	int HP;
};