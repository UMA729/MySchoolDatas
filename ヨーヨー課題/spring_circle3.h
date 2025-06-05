#pragma once
#include "objBase.h"

class CSpring_Circle3 :public Base {
public:
	CSpring_Circle3();

	int Action(list<unique_ptr<Base>>&);
	void Draw();

	//マウスの位置までのベクトル
	Vector move_distance{ 0,0 };
	//ばねの長さ
	float sprin;
	//ばねの減衰率
	float DAMP{ 0.9f };
	//加速度
	float ACC{ 0.05f };
};