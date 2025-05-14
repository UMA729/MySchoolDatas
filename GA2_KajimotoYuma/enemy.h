//enemy.h
#pragma once
#include "objBase.h"
#include "main.h"

#define DEF_X 4
#define DEF_Y 3
class CEnemy :public Base
{
public:
	CEnemy(int x, int y);

	//”¼Œa
	int radius;
	//Œ»İ‚ÌˆÚ“®•ûŒü
	int now_vecX;
	int now_vecY;


	int Action(list<unique_ptr<Base>>&);
	void Draw();
};