//ゲームシーン
#include "DxLib.h"
#include "Scene_Manager.h"
#include "game.h"
#include "function.h"

#include "enemy.h"
#include "enemy2.h"
#include "circle.h"

int GCOMBO{ 0 };
int GGAMETIME{ 0 };
int g_time;
float GSCORE{ 1.0f };
float GSCORE_POW{ 1.0f };
bool g_click_now = false;
bool alr_c = false;

//コンストラクタ
CGame::CGame(CManager* p) :CScene(p){

}

//更新処理
int CGame::Update(){

	if (GGAMETIME % 3 == 0 && GCOMBO < 30 )
	{
		if (alr_c == false)
		{
			alr_c = true;
			Point pos1{ Range_Random_NumberF(32, WINDOW_WIDTH / 2 -32),WINDOW_HEIGHT-32 };
			base.emplace_back((unique_ptr<Base>)new CEnemy(pos1.x, pos1.y));
			Point pos2{ Range_Random_NumberF(WINDOW_WIDTH - 32,WINDOW_WIDTH  + 32),WINDOW_HEIGHT - 32 };
			base.emplace_back((unique_ptr<Base>)new CEnemy2(pos2.x, pos2.y));
		}
	}
	else
	{
		alr_c = false;
	}
	g_time++;
	if (g_time == 60)
	{
		GGAMETIME++;
		g_time = 0;
	}
	//クリックした場合
	{
		if ((GetMouseInput() & MOUSE_INPUT_LEFT))
		{
			if (g_click_now == false)
			{
				g_click_now = true;
				int x, y;
				GetMousePoint(&x, &y);//x,yにクリックした位置が保存される

				base.emplace_back((unique_ptr<Base>)new CCircle(x, y));
			}
		}
		else
		{
			g_click_now = false;
		}
	}
	//listオブジェクトの更新処理
	for (auto i = base.begin(); i != base.end(); i++)
		(*i)->Action(base);

	//リストのサイズ保存
	temp_list_Length = (int)base.size();

	//リストからのオブジェクト削除処理
	for (auto i = base.begin(); i != base.end();)
		(*i)->FLAG ? i++ : i = base.erase(i);

	return 0;
}

//描画処理
void CGame::Draw()
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "List_Size = %d", base.size());
	DrawFormatString(0, 16, GetColor(255, 255, 255), "Score = %.3f", GSCORE);
	DrawFormatString(0, 32, GetColor(255, 255, 255), "COMBO = %d", GCOMBO);
	DrawFormatString(400, 0, GetColor(255, 255, 255), "Time = %d", GGAMETIME);

	DrawCircle(400, WINDOW_HEIGHT - 32, 32, GetColor(255, 255, 255), false);

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
