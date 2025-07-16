
#include "DxLib.h"
#include "Scene_Manager.h"
#include "game.h"
#include "title.h"
#include "function.h"

CTitle::CTitle(CManager* p) :CScene(p)
{

}

int CTitle::Update()
{
	//ループ再生
	/*if (!CheckSoundMem(sound[0]))
		PlaySoundMem(sound[0], DX_PLAYTYPE_LOOP, true);

	if (CheckHitKey(KEY_INPUT_UP)) {
		PlaySoundMem(sound[1], DX_PLAYTYPE_BACK, true);
	}*/
	//スペースキーでgameシーンに以降
	if (GetMouseInput()& MOUSE_INPUT_LEFT)
	{
		int x, y;
		GetMousePoint(&x, &y);
		if (WINDOW_WIDTH  / 2 - 60 / 2 < x && x < WINDOW_WIDTH  / 2 + 60 / 2 &&
			WINDOW_HEIGHT / 2 - 60 / 2 < y && y < WINDOW_HEIGHT / 2 + 60 / 2)
		{
			manager->Scene_Delete();
			manager->scene = new CGame(manager);
		}
	}


	return 0;
}

void CTitle::Draw()
{
	SetFontSize(50);
	DrawFormatString(WINDOW_WIDTH/2-5.5*60, WINDOW_HEIGHT/4, GetColor(255, 255, 255), "Buttonをクリックでスタート");

	DrawCircle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 60, GetColor(255, 0, 0), true);
	DrawCircle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 61, GetColor(255, 255, 255), false);

	SetFontSize(25);
	DrawFormatString(WINDOW_WIDTH/2-3*12.5, WINDOW_HEIGHT/2.09, GetColor(255, 255, 255), "BUTTON");
}

CTitle::~CTitle()
{
	InitSoundMem();//メモリ内のサウンドデータをすべて削除
	DeleteSoundMem(sound[0]);//指定したサウンドデータをメモリから削除
}