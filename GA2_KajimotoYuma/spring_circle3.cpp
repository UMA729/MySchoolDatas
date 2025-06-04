#pragma once
#include "spring_circle3.h"

CSpring_Circle3::CSpring_Circle3() {
	pos.x = WINDOW_WIDTH / 2;
	pos.y = WINDOW_HEIGHT / 2;
}

int CSpring_Circle3::Action(list<unique_ptr<Base>>&) {
	int x, y;
	GetMousePoint(&x, &y);//マウスの位置取得

	//マウスの位置までの距離
	move_distance.x = x - pos.x;
	move_distance.y = y - pos.y;
	//加速度ベクトル
	Vector acc{ 0,0 };
	acc.x = ACC * move_distance.x;
	acc.y = ACC * move_distance.y;
	return 0;
}

void CSpring_Circle3::Draw() {
	//ばねの描画
	DrawLine(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, pos.x, pos.y, GetColor(255, 255, 255));
	//ばねの終点にボールを描画
	DrawCircle(
		pos.x, pos.y,
		32,
		GetColor(255, 0, 0),
		true
	);
}