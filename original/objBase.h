#pragma once
#include "main.h"
#include "objChar.h"

using namespace std;

//オブジェクトベースクラス
class Base : public Character{
public:
	int pri{ 0 };//描画の順番
	int ID{ -1 };//オブジェクトID
	bool FLAG{ true };//オブジェクト存在フラグ
	bool draw_flag{ true };//オブジェクトの描画フラグ

	virtual int Action(list<unique_ptr<Base>>&) = 0;
	virtual void Draw() = 0;
};

//オブジェクトのソートクラス
class sort
{
public:
	bool operator()(const unique_ptr<Base>& x, const unique_ptr<Base>& y) const
	{
		return x.get()->pri < y.get()->pri;
	}
};

//3D用
class Base3D : public Character3D {
public:
	int ID{ -1 };//オブジェクトID
	bool FLAG{ true };//オブジェクト存在フラグ
	bool draw_flag{ true };//オブジェクトの描画フラグ

	virtual int Action(list<unique_ptr<Base3D>>&) = 0;
	virtual void Draw() = 0;
};
