//ゲームシーン
#include "DxLib.h"
#include "Scene_Manager.h"
#include "game.h"
#include "Enemy_A.h"
#include "Enemy_D.h"
#include "Player.h"
#include "function.h"

int GSecond{ 0 };
char GChar[10];

//コンストラクタ
CGame::CGame(CManager* p) :CScene(p) {
	base.emplace_back((unique_ptr<Base>)new CPlayer());
	base.emplace_back((unique_ptr<Base>)new CEnemyA());
	base.emplace_back((unique_ptr<Base>)new CEnemyD());
	frame = 0;
}

//更新処理
int CGame::Update() {


	//listオブジェクトの更新処理
	for (auto i = base.begin(); i != base.end(); i++)
		(*i)->Action(base);

	//リストのサイズ保存
	temp_list_Length = (int)base.size();

	//リストからのオブジェクト削除処理
	for (auto i = base.begin(); i != base.end();)
		(*i)->FLAG ? i++ : i = base.erase(i);

	frame++;

	if (frame == 60)
	{
		GSecond++;
		frame = 0;
	}
	return 0;
}

//描画処理
void CGame::Draw()
{
	SetFontSize(80);
	DrawFormatString(WINDOW_WIDTH/2, 64, GetColor(255, 255, 255), "%d", GSecond);
	int str = GetDrawStringWidth(GChar, -1);
	int win = (WINDOW_WIDTH - str) / 2;
	SetFontSize(100);
	DrawFormatString(win, 200, GetColor(255, 255, 255), "%s", GChar);
	//listオブジェクトの描画
	for (auto i = base.begin(); i != base.end(); i++)
		if ((*i)->FLAG) (*i)->Draw();

	//3D軸の描画
	//DrawLine3D(
	//	VGet(0, 0, 0),
	//	VGet(0, 0, 100),
	//	0x0000ff
	//);
	//DrawLine3D(
	//	VGet(0, 0, 0),
	//	VGet(100, 0, 0),
	//	0xff0000
	//);
	//DrawLine3D(
	//	VGet(0, 0, 0),
	//	VGet(0, 100, 0),
	//	0x00ff00
	//);
}

CGame::~CGame()
{

}
