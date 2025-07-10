//DX Library Copyright (C) 2001-2024 Takumi Yamada.
//Dxlib_lib_4.1 2025 Kcomテンプレート Shinya Wakisaka.
#pragma once
#define _USE_MATH_DEFINES
#include "DxLib.h"
#include <list>
#include <vector>
#include <memory>
#include <time.h>

using namespace std;

//ラジアン変換マクロ
#define RADIAN(_deg) ((float)M_PI*(_deg)/180.0f)

//基本ウィンドウサイズ
constexpr auto WINDOW_WIDTH = 800;	//横幅
constexpr auto WINDOW_HEIGHT = 600;	//縦幅

//グローバル変数
extern int GSCORE;

//識別ID
enum objID {
	PLAYER,
	ENEMY,
	POS,
};

//2D用
 struct Point {
	float x, y;
};
 struct Vector {
	float x, y;
};

//3D用
struct Point3 {
	float x, y, z;
};
struct Vector3 {
	float x, y, z;
};
 struct ObjSize {
	float h, w, d;
};

//カメラ情報クラス(main.h)
//class CCameraInfo {
//private:
//	static Point3 pos;//カメラの位置
//public:
//	static Point3 GetPos() { return pos; }//カメラの位置情報取得
//	static void SetPos(Point3 p) { pos = p; }//カメラの位置情報保存
//};