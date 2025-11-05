#pragma once
#include "objBase.h"

class CBlock :public BaseVector {
public:
	//位置、チップNo、イメージハンドル
	CBlock(Point, int, int);
	~CBlock();

	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	int tipNo{ -1 };
};