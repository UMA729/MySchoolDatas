#pragma once
#include "objBase.h"

class CCarry_Spring :public Base {
public:
	CCarry_Spring();

	int Action(list<unique_ptr<Base>>&);
	void Draw();

	//半径
	int radius{ 0 };

	//マウスの位置までのベクトル
	Vector move_distance{ 0,0 };
	//ばねの長さ
	float sprin;
	//ばねの減衰率
	float DAMP{ 0.9f };
	//加速度
	float ACC{ 0.05f };

	Point thing_pos{ 0,0 };

	bool click;

	bool one_click;
};