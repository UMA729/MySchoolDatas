#pragma once
#include "spring_circle2.h"

CSpring_Circle2::CSpring_Circle2() {
	pos.x = WINDOW_WIDTH / 2;
	pos.y = WINDOW_HEIGHT / 2;
}

int CSpring_Circle2::Action(list<unique_ptr<Base>>&) {
	int x, y;
	GetMousePoint(&x, &y);//マウスの位置取得

	//クリックした位置のy座標をばねの終点にする
	{
		if (GetMouseInput() & MOUSE_INPUT_LEFT) {
			pos.x = x;
			pos.y = y;
		}
	}

	//横方向
	spring_length.x = WINDOW_WIDTH / 2 - pos.x;
	F.x = k * spring_length.x;
	acc.x = F.x / mass;
	vec.x = damp * (vec.x + acc.x);

	//縦方向
	spring_length.y = WINDOW_HEIGHT / 2 - pos.y;
	F.y = k * spring_length.y;
	acc.y = F.y / mass;
	vec.y = damp * (vec.y + acc.y);

	//座標更新
	pos.x += vec.x;
	pos.y += vec.y;
	return 0;
}

void CSpring_Circle2::Draw() {
	//ばねの描画
	DrawLine(WINDOW_WIDTH / 2, WINDOW_HEIGHT/2, pos.x, pos.y, GetColor(255, 255, 255));
	//ばねの終点にボールを描画
	DrawCircle(
		pos.x, pos.y,
		32,
		GetColor(255, 0, 0),
		true
	);
}