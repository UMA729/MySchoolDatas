#pragma once

#include "objBase.h"

class CWall :Base
{
public:
	CWall(int x,int y);

	int Action(list<unique_ptr<Base>>&);
	void Draw();
};