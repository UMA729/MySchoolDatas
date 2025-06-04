//circle.cpp
#include "circle.h"
#include "function.h"

#include "enemy.h"

//重力加速度
constexpr float g = 0.98f;

CCircle::CCircle()
{
	radius = 32;
	pos.x = radius;
	pos.y = WINDOW_HEIGHT - radius;
}
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

	vec.y += g;		//加速度を加算

	pos.x += vec.x; //重力重力
	pos.y += vec.y; //重力重力

	//当たり判定
	//円判定
	for (auto i = base.begin(); i != base.end(); i++) {
		if ((*i)->ID == ENEMY) {
			float e_radius = ((CEnemy*)(*i).get())->radius;

			float distance = DistanceF(pos.x, pos.y, (*i)->pos.x, (*i)->pos.y);
			if (distance < radius + e_radius)
			{
				FLAG = false;
				GSCORE++;
			}
		}

		//画面の横,下で初期値に
		if (pos.x<radius || pos.x>WINDOW_WIDTH - radius ||
			pos.y > WINDOW_HEIGHT - radius &&
			FLAG != false)
			//画面の外に出たら削除
			//if (pos.y > WINDOW_HEIGHT + radius * 2)FLAG = false;


			return 0;
	}
}

void CCircle::Draw() {
	DrawCircle(
		pos.x, pos.y,			 //中心座標
		radius,					 //半径
		GetColor(255,255,255), //色
		false					 //塗りつぶし(true/false)
	);

}