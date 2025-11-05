#include "block.h"

CBlock::CBlock(Point p, int _i, int _img) {
	img = _img;

	ImgWidth = 16;
	ImgHeight = 16;

	pos = p;

	CutX = (_i % 51) * ImgWidth;
	CutY = (_i / 51) * ImgHeight;

	pri = 0;
	ID = BLOCK;
}

int CBlock::Action(vector<unique_ptr<BaseVector>>& base) {
	return 0;
}

void CBlock::Draw() {
	DrawRectGraph(
		pos.x, pos.y,
		CutX, CutY,
		ImgWidth, ImgHeight,
		img,
		true
	);
}

CBlock::~CBlock(){}