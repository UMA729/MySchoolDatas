#pragma once

#include "objBase.h"

class CWall :public Base
{
public:
	CWall(int ,int );

	int Action(list<unique_ptr<Base>>&);
	void Draw();

	int wall_max_y;
	int wall_spa;
	int wall_wid;
	int center_x;
	int total_width;
	int count;
};