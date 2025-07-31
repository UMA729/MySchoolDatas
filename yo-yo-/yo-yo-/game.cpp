//ゲームシーン
#include "DxLib.h"
#include "Scene_Manager.h"
#include "game.h"
#include "title.h"
#include "function.h"

#include "goal.h"
#include "wall.h"
#include "carry_thing.h"
#include "spring_circle.h"
#include "spring_circle2.h"
#include "carry_spring.h"

int GSCORE{ 10000 };
int CCount{ 0 };
bool WallFlag = false;
bool start = true;
void Reset()
{
	GSCORE = 10000;
	CCount = 0;
	WallFlag = false;

}
//コンストラクタ
CGame::CGame(CManager* p) :CScene(p){
	//Point pos{ Range_Random_NumberF(32, WINDOW_WIDTH - 32),400};
	//base.emplace_back((unique_ptr<Base>)new CEnemy(pos.x, pos.y));

	//base.emplace_back((unique_ptr<Base>)new CSpring_Circle());

	base.emplace_back((unique_ptr<Base>)new CCarry_Spring());
	base.emplace_back((unique_ptr<Base>)new CCarry_thing());
}

//更新処理
int CGame::Update(){

	if (GSCORE <= 0)
	{
		ClearDrawScreen();
		const char *str[3]{
			{"ゲームオーバー"},
			{"Rキーでリスタート"},
			{"ESCでゲーム終了"}
		};
		
		int count = sizeof(str) / sizeof(str[0]);
		int wid = GetDrawStringWidth(str[0], (int)strlen(str[0]));
		int disx = WINDOW_WIDTH/2 - wid / 2;
		int disy = WINDOW_HEIGHT / 2 - (count / 2)*40;

		DrawString(disx,disy,str[0],GetColor(255,0,0));
		DrawString(WINDOW_WIDTH/8,WINDOW_HEIGHT/1.5,str[1],GetColor(255,255,255),true);
		DrawString(WINDOW_WIDTH/1.5,WINDOW_HEIGHT/1.5,str[2],GetColor(255,255,255),true);
		if (CheckHitKey(KEY_INPUT_R))
		{
			ClearDrawScreen();
			Reset();
			//シーン管理クラス
			manager->scene = new CGame(manager);
		}
		if(CheckHitKey(KEY_INPUT_ESCAPE))
			DxLib_End();
	}
	
	for (auto j = base.begin(); j != base.end(); j++)
	{
		
		if ((*j)->ID == THING)
		{
			if ((*j)->pos.x <= WINDOW_WIDTH / 8)
			{
				for (int i = 0; i <= CCount; i++)
				{
					if (WallFlag == false)
						base.emplace_back((unique_ptr<Base>)new CWall(Range_Random_NumberF(20, WINDOW_HEIGHT - 100), i));
					if (i == CCount)
						WallFlag = true;
				}
			}
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
	DrawFormatString(3/WINDOW_WIDTH, 16, GetColor(255, 255, 255), "Score = %d", GSCORE);

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
