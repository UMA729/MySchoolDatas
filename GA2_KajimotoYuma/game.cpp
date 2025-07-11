//ゲームシーン
#include "DxLib.h"
#include "Scene_Manager.h"
#include "game.h"
#include "function.h"

#include "enemy.h"
#include "enemy2.h"
#include "circle.h"
#include "spring_circle2.h"

int GSCORE{ 0 };

//コンストラクタ
CGame::CGame(CManager* p) :CScene(p){
	//Point pos{ Range_Random_NumberF(32, WINDOW_WIDTH - 32),400};
	//base.emplace_back((unique_ptr<Base>)new CEnemy(pos.x, pos.y));

	base.emplace_back((unique_ptr<Base>)new CSpring_Circle2());
}

//更新処理
int CGame::Update(){

	//クリックした場合
	//{
	//	if (GetMouseInput() & MOUSE_INPUT_LEFT)
	//	{
	//		int x, y;
	//		GetMousePoint(&x, &y);//x,yにクリックした位置が保存される

	//		base.emplace_back((unique_ptr<Base>)new CCircle(x, y));
	//	}
	//}
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
	DrawFormatString(0, 16, GetColor(255, 255, 255), "Score = %d", GSCORE);

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
