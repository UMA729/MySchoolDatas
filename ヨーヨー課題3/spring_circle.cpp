//ばねの基礎
#pragma once
#include "spring_circle.h"

CSpring_Circle::CSpring_Circle() {
	pos.x = WINDOW_WIDTH / 2;
	pos.y = spring_natural_length;
}

int CSpring_Circle::Action(list<unique_ptr<Base>>&) {
	int x, y;
	GetMousePoint(&x, &y);//マウスの位置取得
	
	//クリックした位置のy座標をばねの終点にする
	{
		if (GetMouseInput() & MOUSE_INPUT_LEFT) {
			pos.y = y;
		}
	}

	//ばえの伸びた長さを計算
	spring_length = pos.y - spring_natural_length;
	//ばねの力を計算
	F = -k * spring_length;
	acc = F / mass;//重さによる加速度を計算
	vec.y = damp * (vec.y + acc);//減衰率＋重さによる加速度を考慮
	//vec.y = damp * (vec.y + F);//減衰率を考慮した場合
	//vec.y += F;

	//座標更新
	pos.y += vec.y;
	return 0;
}

void CSpring_Circle::Draw() {
	//ばねの描画
	DrawLine(WINDOW_WIDTH / 2, 0, pos.x, pos.y, GetColor(255, 255, 255));
	//ばねの終点にボールを描画
	DrawCircle(
		pos.x, pos.y,
		32,
		GetColor(255, 0, 0),
		true
	);
}