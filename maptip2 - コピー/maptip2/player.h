//player.h
#pragma once
#include "objBase.h"

class CPlayer :public BaseVector {
public:
	CPlayer();

	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//マップ上の位置
	Point m_pos{ 0,0 };

};