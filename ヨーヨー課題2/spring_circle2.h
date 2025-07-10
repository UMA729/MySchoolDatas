#pragma once
#include "objBase.h"

class CSpring_Circle2 :public Base {
public:
	CSpring_Circle2();

	int Action(list<unique_ptr<Base>>&);
	void Draw();

	//ばねの長さ
	float spring_natural_length{ 200 };
	//ばねの長さ（伸びた長さ:x）
	Vector spring_length{ 0,0 };
	//ばねの強さ
	float k{ 0.1f };
	//ばねの反発力
	Vector F{ 0.0f,0.0f };
	//ばねの減衰率
	float damp{ 0.9f };
	//オブジェクトの質量
	float mass{ 3.0f };
	//加速度
	Vector acc{ 0.0f,0.0f };
};