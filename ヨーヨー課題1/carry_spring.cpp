#pragma once
#include "carry_spring.h"
#include "carry_thing.h"
#include"function.h"


CCarry_Spring::CCarry_Spring() {
	radius = 32;
	click = false;
	one_click = false;
}

int CCarry_Spring::Action(list<unique_ptr<Base>>& base) {

	int x, y;
	GetMousePoint(&x, &y);//マウスの位置取得

	for (auto i = base.begin(); i != base.end(); i++)
	{
		float obj_rad = ((CCarry_thing*)(*i).get())->radius;
		if (click == true)
		{
					//マウスの位置までの距離
					move_distance.x = x - (*i)->pos.x;
					move_distance.y = y - (*i)->pos.y;
					//加速度ベクトル
					Vector acc{ 0,0 };
					acc.x = ACC * move_distance.x;
					acc.y = ACC * move_distance.y;

					vec.x = DAMP * (acc.x + vec.x);
					vec.y = DAMP * (acc.y + vec.y);

					//座標更新
					(*i)->pos.x += vec.x;
					(*i)->pos.y += vec.y;

					////画面外制御(横)
					//if (x < radius && pos.x < radius)
					//	pos.x = radius;
					//else if (x > WINDOW_WIDTH - radius && pos.x > WINDOW_WIDTH - radius)
					//	pos.x = WINDOW_WIDTH - radius;
					////画面外制御(縦)
					//if (y < radius && pos.y < radius)
					//	pos.y = radius;
					//else if (y > WINDOW_HEIGHT - radius && pos.y > WINDOW_HEIGHT - radius)
					//	pos.y = WINDOW_HEIGHT - radius;
		}
		if ((*i)->ID == PLAYER)
		{
			if (GetMouseInput() & MOUSE_INPUT_LEFT)
			{
				
				if (one_click == false)
				{
					one_click = true;

					if (click == true)
					{
						click = false;
					}
					else if ((*i)->pos.x - obj_rad / 2 < x && (*i)->pos.y - obj_rad / 2 < y &&
							 (*i)->pos.x + obj_rad / 2 > x && (*i)->pos.y + obj_rad / 2 > y	&&
							 click == false)
					{
						click = true;
					}
				}
			}
			else
			{
				one_click = false;
			}
		}

		thing_pos = (*i)->pos;
		pos.x = x;
		pos.y = y;
	}

	////SetMousePoint(100, 230);

	return 0;
}

void CCarry_Spring::Draw() {
	//ばねの描画
	if(click == true)
	DrawLine(pos.x, pos.y, thing_pos.x, thing_pos.y, GetColor(255, 255, 255));	
}