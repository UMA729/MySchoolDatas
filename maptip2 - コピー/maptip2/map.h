#pragma once
#include "objBase.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

class CMap {
public:
	CMap();
	~CMap() {};

	int img;//マップチップ画像
	int map[11][160]{ 0 };//マップデータ

	//カメラ座標
	Point camera{ 0,0 };

	//データ読み込み
	void LoadMap();
	//マップオブジェクト生成
	void Map_ObjCreation(vector<unique_ptr<BaseVector>>&);
	//マップ更新処理
	void Action(vector<unique_ptr<BaseVector>>&);

};