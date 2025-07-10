#pragma once
#include "objBase.h"

class CSpring_Circle :public Base {
public:
	CSpring_Circle();

	int Action(list<unique_ptr<Base>>&);
	void Draw();

	//ばねの長さ
	float spring_natural_length{ 200 };
	//ばねの長さ（伸びた長さ:x）
	float spring_length{ 0 };
	//ばねの強さ
	float k{ 0.1f };
	//ばねの反発力
	float F{ 0.0f };
	//ばねの減衰率
	float damp{ 0.9f };
	//オブジェクトの質量
	float mass{ 3.0f };
	//加速度
	float acc{ 0.0f };
};