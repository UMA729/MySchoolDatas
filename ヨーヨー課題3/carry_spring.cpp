#pragma once
#include "carry_spring.h"
#include "carry_thing.h"
#include "Scene_Manager.h"
#include "title.h"
#include"function.h"


CCarry_Spring::CCarry_Spring() {
	radius = 60;
	click = false;
	one_click = false;
	g_start = false;
}

int CCarry_Spring::Action(list<unique_ptr<Base>>& base) {

	if (g_start == false)
	{
		for (auto i = base.begin(); i != base.end(); i++)
		{

			bool start = ((CCarry_thing*)(*i).get())->t_start;
			if (start == false)
			{
				g_start = true;
				click = true;
			}
		}
	}

	int x, y;
	GetMousePoint(&x, &y);//マウスの位置取得

	if (g_start == true)
	{
		{
			for (auto i = base.begin(); i != base.end(); i++)
			{
				float obj_rad = ((CCarry_thing*)(*i).get())->radius;
				//if (click == true)
				//{
					//マウスの位置までの距離
					move_distance.x = x - (*i)->pos.x;
					move_distance.y = y - (*i)->pos.y;
					//加速度ベクトル
					Vector acc{ 0,0 };
					acc.x = ACC * move_distance.x;
					acc.y = ACC * move_distance.y;

					vec.x = DAMP * (acc.x + vec.x);
					vec.y = DAMP * (acc.y + vec.y);

					DrawFormatString(0, 70, GetColor(255, 255, 255), "%f\n%f", vec.x, vec.y);

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
				//}
				if ((*i)->ID == PLAYER)
				{
					if (GetMouseInput() & MOUSE_INPUT_LEFT)
					{
						int x, y;

						GetMousePoint(&x, &y);

						float dx = (*i)->pos.x - x;
						float dy = (*i)->pos.y - y;

						float dr = dx * dx + dy * dy;

						float dl = obj_rad * obj_rad;

						if (one_click == false)
						{
							one_click = true;

							if (click == true)
							{
								click = false;
							}
							else if (dr < dl && click == false)
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
		}
	}

	////SetMousePoint(100, 230);

	return 0;
}


void CCarry_Spring::Draw() {
	//ばねの描画
	if(click == true)
	DrawLine(pos.x, pos.y, thing_pos.x, thing_pos.y, GetColor(255, 255, 255));	
}