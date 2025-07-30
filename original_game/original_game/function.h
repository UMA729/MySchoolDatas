#pragma once
#include "objBase.h"

//最大最小
//(Point3 A, Point3 B) A → B 間の最大最小
//最大値最小値
struct MINMAX {
	Vector3 min{ 0,0,0 };
	Vector3 max{ 0,0,0 };

	//Point3 to Point3
	MINMAX(Point3 a, Point3 b) {
		min.x = a.x;
		min.y = a.y;
		min.z = a.z;

		max.x = a.x + b.x;
		max.y = a.y + b.y;
		max.z = a.z + b.z;
	}
	//Vector3 to Vector3
	MINMAX(Vector3 a, Vector3 b) {
		min.x = a.x;
		min.y = a.y;
		min.z = a.z;

		max.x = a.x + b.x;
		max.y = a.y + b.y;
		max.z = a.z + b.z;
	}
	//Point3 to Vector3
	MINMAX(Point3 a, Vector3 b) {
		min.x = a.x;
		min.y = a.y;
		min.z = a.z;

		max.x = a.x + b.x;
		max.y = a.y + b.y;
		max.z = a.z + b.z;
	}
};
//座標 + ベクトル
Point Add_Point_Vector(Point, Vector);
//座標 - 座標
Vector Sub_Point_Point(Point, Point);
//ベクトル - ベクトル
Vector Sub_Vector_Vector(Vector, Vector);
//ベクトル + ベクトル
Vector Add_Vector_Vector(Vector, Vector);
//ベクトル × スカラー
Vector Mul_Vector_Scaler(Vector, float);
//ベクトルの長さ
float Vector_Length(Vector);
//ベクトルの正規化
Vector Vector_Normalize(Vector);
//ベクトルの長さを設定
Vector Vector_SetLength(Vector, float);

//座標 + ベクトル
Point3 Add_Point_Vector(Point3, Vector3);
//座標 - 座標
Vector3 Sub_Point_Point(Point3, Point3);
//ベクトル - ベクトル
Vector3 Sub_Vector_Vector(Vector3, Vector3);
//ベクトル + ベクトル
Vector3 Add_Vector_Vector(Vector3, Vector3);
//ベクトル × スカラー
Vector3 Mul_Vector_Scaler(Vector3, float);
//ベクトルの長さ
float Vector_Length(Vector3);
//ベクトルの正規化
Vector3 Vector_Normalize(Vector3);
//ベクトルの長さを設定
Vector3 Vector_SetLength(Vector3, float);
//内積(3D)
float Vector3_Dot(Vector3, Vector3);
//外積(3D)
Vector3 Vector3_Cross(Vector3, Vector3);
//ベクトルに対して指定した角度に回転(x軸回転)
Vector3 Vector_Rotation_X(Vector3, float);
//指定した角度に回転(x軸回転)
Vector3 Vector_Rotation_X(float);
//ベクトルに対して指定した角度に回転(y軸回転)
Vector3 Vector_Rotation_Y(Vector3, float);
//指定した角度に回転(y軸回転)
Vector3 Vector_Rotation_Y(float);
//ベクトルに対して指定した角度に回転(z軸回転)
Vector3 Vector_Rotation_Z(Vector3, float);
//指定した角度に回転(z軸回転)
Vector3 Vector_Rotation_Z(float);

//矩形判定(x1,y1,x2,y2,w,h)
int HitCheck_box(float, float, float, float, int, int);
//矩形判定２(x1,y1,x2,y2,w1,h1,w2,h2)
int HitCheck_box(float, float, float, float, int, int, int, int);
//矩形判定3(オブジェクト,オブジェクト)
//画像サイズ(ImgWidth,ImgHeight)で判定
int HitCheck_box(Base*, Base*);

//AABB判定(オブジェクト,オブジェクト)
bool HitCheck_AABB(Base3D*, Base3D*);
//AABB判定
bool HitCheck_AABB(MINMAX, MINMAX);
//球判定(位置A,半径A,位置B,半径B)
bool HitCheck_Sphere(Point3, float, Point3, float);

//円と矩形判定処理(円オブジェクト,矩形オブジェクト,円の半径)
void HitCheck_Box_Circle(Base*, Base*, float);

//指定範囲の乱数を取得(min,max)
int Range_Random_Number(int, int);
float Range_Random_NumberF(float, float);

//二点間距離
float DistanceF(float, float, float, float);
float DistanceF(Point3, Point3);

//全てのキーが、離されるまで待機
void WAIT_RELEASE_ALL_KEY();

//全てのキーが、離されるまで待機（タイプ指定 DX_CHECKINPUT_KEY : DX_CHECKINPUT_PAD : DX_CHECKINPUT_MOUS)
void WAIT_RELEASE_ALL_KEY(int);

//数値の描画（画像ハンドル, 一桁目の位置x, 一桁目の位置y, 値, 文字サイズw, 文字サイズh)
void DrawNum(int, int, int, int, int, int);

//リストから指定オブジェクトの取得
Base* Get_obj(list<unique_ptr<Base>>&, int);
Base3D* Get_obj(list<unique_ptr<Base3D>>&, int);

//指定したオブジェクトの座標取得
Point Get_Point(list<unique_ptr<Base>>&, int);

//指定したオブジェクト以外をリストから削除
void Del_SpecifiedObj(list<unique_ptr<Base>>&, int);

//指定オブジェクトをリストから削除(FLAGをfalseにする)
void Del_ListObj(list<unique_ptr<Base>>&, int);

//指定オブジェクトをリストから削除範囲(FLAGをfalseにする)
void Del_ListObj(list<unique_ptr<Base>>&, int, int);