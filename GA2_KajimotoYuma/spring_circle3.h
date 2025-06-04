#pragma once
#include "objBase.h"

class CSpring_Circle3 :public Base {
public:
	CSpring_Circle3();

	int Action(list<unique_ptr<Base>>&);
	void Draw();

	//マウスの位置までのベクトル
	Vector move_distance{ 0,0 };
	//ばねの減衰率
	float damp{ 0.9f };
	//加速度
	float acc{ 0.0f };
};