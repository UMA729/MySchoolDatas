//player.cpp
#pragma once
#include "player.h"

CPlayer::CPlayer() {
	img = LoadGraph("image\\player.png");

	//マップ初期位置
	m_pos.x = 92;
	m_pos.y = 160;
	//描画位置
	pos = m_pos;

	pri = 1;
	ID = PLAYER;
}

int CPlayer::Action(vector<unique_ptr<BaseVector>>& base) {


	vec.x = 0.0f; vec.y = 0.0f;

	if (CheckHitKey(KEY_INPUT_LEFT)) vec.x = -4.0f; //左移動
	if (CheckHitKey(KEY_INPUT_RIGHT))vec.x = 4.0f;	//右移動
	if (CheckHitKey(KEY_INPUT_UP))	 vec.y = -4.0f;	//上移動
	if (CheckHitKey(KEY_INPUT_DOWN)) vec.y = 4.0f;	//した移動

	//座標更新
	m_pos.x += vec.x;
	m_pos.y += vec.y;

	return 0;
}

void CPlayer::Draw() {
	DrawCircle(
		pos.x, pos.y, 16,		//描画位置
		GetColor(255,0,0),		//画像切り取り開始位置
		true
	);
}