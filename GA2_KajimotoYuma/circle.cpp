//circle.cpp
#include "circle.h"
#include "function.h"

#include "enemy.h"

//重力加速度
constexpr auto g = 0.98f;

CCircle::CCircle(int x, int y)
{
	radius = 32;
	//pos.x = x;
	//pos.y = y;

	pos.x = radius;
	pos.y = WINDOW_HEIGHT - radius;

	//vec.x = 4.0f;

	//クリックした方向のベクトルを求める
	vec.x = x - pos.x;
	vec.y = y - pos.y;

	vec = Vector_SetLength(vec, 30.0f);

	ID = PLAYER;
}

int CCircle::Action(list<unique_ptr<Base>>& base)
{
	clear_to_frame++;

	//vec.y += g;		//加速度を加算

	pos.x += vec.x; //重力重力
	pos.y += vec.y; //重力重力

	//当たり判定
	//円判定
	for (auto i = base.begin();i != base.end();i++){
		if ((*i)->ID == ENEMY) {
			int e_vecX = 0;
			int e_vecY = 0;
			float e_radius = ((CEnemy*)(*i).get())->radius;
			float distance = DistanceF(pos.x, pos.y, (*i)->pos.x, (*i)->pos.y);
			if (distance < radius + e_radius)
			{
				(*i)->vec.x = vec.x * 0.3f;
				(*i)->vec.y = vec.y * 0.3f;
				//当たっている
				GSCORE_POW += 1.1f;
				FLAG = false;
				GSCORE += (GCOMBO + 1) * GSCORE_POW;//スコア加算
				GCOMBO++;
				//コンボ数で挙動を変える
				if (GCOMBO == 10)
				{
					e_vecX=((CEnemy*)(*i).get())->now_vecX;
					e_vecY=((CEnemy*)(*i).get())->now_vecY;
					(*i)->vec.x = e_vecX*6;
					(*i)->vec.y = e_vecY*5;
				}
				if (GCOMBO == 30)
				{
					(*i)->vec.x += 0;
				}
			}
		}
	}

	//画面の横で跳ね返る
	if (pos.x<radius || pos.x>WINDOW_WIDTH - radius && FLAG != false)
	{
		FLAG = false;
		GCOMBO = 0;
		GSCORE_POW = 1.0f;
	}

	//画面の下で跳ね返る
	if (pos.y > WINDOW_HEIGHT - radius && FLAG != false)
	{
		FLAG = false;
		GCOMBO = 0;
		GSCORE_POW = 1.0f;
	}
	//画面の外に出たら削除
	//if (pos.y > WINDOW_HEIGHT + radius * 2)FLAG = false;


	return 0;
}

void CCircle::Draw() {
	DrawCircle(
		pos.x, pos.y,			 //中心座標
		radius,					 //半径
		GetColor(255,255,255), //色
		false					 //塗りつぶし(true/false)
	);
}