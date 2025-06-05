//enemy.h
#pragma once
#include "objBase.h"
#include "main.h"

class CEnemy2 :public Base
{
public:
	CEnemy2();

	//”¼Œa
	int radius;

	int Action(list<unique_ptr<Base>>&);
	void Draw();

	//Á‚¦‚é‚Ü‚Å‚ÌƒtƒŒ[ƒ€ŠÔ
	int clear_to_frame{ 0 };
};