#pragma once
#include "spring_circle3.h"

CSpring_Circle3::CSpring_Circle3() {
	pos.x = WINDOW_WIDTH / 2;
	pos.y = WINDOW_HEIGHT / 2;
}

int CSpring_Circle3::Action(list<unique_ptr<Base>>&) {
	int x, y;
	GetMousePoint(&x, &y);//マウスの位置取得



	////画面外制御(横)
	//if (x < 32 && pos.x < 32)
	//	pos.x = 32;
	//else if (x > WINDOW_WIDTH - 32 && pos.x > WINDOW_WIDTH - 32)
	//	pos.x = WINDOW_WIDTH - 32;
	////画面外制御(縦)
	//if (y < 32 && pos.y < 32)
	//	pos.y = 32;
	//else if (y > WINDOW_HEIGHT - 32 && pos.y > WINDOW_HEIGHT - 32)
	//	pos.y = WINDOW_HEIGHT - 32;


	//マウスの位置までの距離
	move_distance.x = x - pos.x;
	move_distance.y = y - pos.y;
	//加速度ベクトル
	Vector acc{ 0,0 };
	acc.x = ACC * move_distance.x;
	acc.y = ACC * move_distance.y;
	vec.x = DAMP * (acc.x + vec.x);
	vec.y = DAMP * (acc.y + vec.y);


	//座標更新
	pos.x += vec.x;
	pos.y += vec.y;

	return 0;
}

void CSpring_Circle3::Draw() {
	//ばねの描画
	DrawLine(pos.x,pos.y, pos.x, pos.y, GetColor(255, 255, 255));
	//ばねの終点にボールを描画
	DrawCircle(
		pos.x , pos.y,
		32,
		GetColor(255, 0, 0),
		true
	);
}