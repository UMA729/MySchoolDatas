//ゲームシーンヘッダ
#pragma once
#include "objBase.h"
#include "CCamera.h"

class CGame :public CScene
{
private:
public:
	//オブジェクトリスト
	//list<unique_ptr<Base3D>> base3D;//3Dオブジェクト
	list<unique_ptr<Base>> base;//2Dオブジェクト

	//オブジェクト個数保存用
	int temp_list_Length{ 0 };

	//コンストラクタ
	CGame(CManager*);

	//デストラクタ
	~CGame();

	int Update();//更新処理
	void Draw();//描画処理

	//CCamera* camera;//カメラオブジェクト
};