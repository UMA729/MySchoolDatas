#include "DxLib.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	SetOutApplicationLogValidFlag(false);//ログ出力オフ
	ChangeWindowMode(TRUE); //ウィンドウモード切り替え
	SetGraphMode(224, 224, 32); //ウィンドウサイズ

	if (DxLib_Init() == -1) { //DXライブラリ初期化処理
		return -1;			  //エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK); //描画先を裏画面に変更
	SetWindowText("soukoban"); //ウィンドウの名前

	//変数の宣言---------------

	const int Space_num = 0;	//空白番号
	const int P_num = 1;		//壁番号
	const int Box_num = 2;		//プレイヤー番号
	const int Goal_num = 3;		//箱番号
	const int Wall_num = 4;		//ゴール番号

	const int map_num = 7;		//マップの配列数

	int map[map_num][map_num] =
	{
		{4,4,4,4,4,4,4},
		{4,0,0,0,0,3,4},
		{4,0,1,2,0,0,4},
		{4,0,2,2,2,3,4},
		{4,0,0,2,0,0,4},
		{4,3,0,3,0,3,4},
		{4,4,4,4,4,4,4}
	};
	int tmp_map[map_num][map_num];

	int Kup = 0;
	int Kdown = 0;
	int Kleft = 0;
	int Kright = 0;


	int gh[5];

	gh[0] = LoadGraph("image\\empty.png");
	gh[1] = LoadGraph("image\\player.png");
	gh[2] = LoadGraph("image\\box.png");
	gh[3] = LoadGraph("image\\goll.png");
	gh[4] = LoadGraph("image\\wall.png");

	//フォントサイズ変更
	SetFontSize(50);

	while (1)
	{
		//裏画面のデータを全て削除
		ClearDrawScreen();

		//処理----------------------------------------------------------------
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{

				if (tmp_map[i][j] == Goal_num && map[i][j] == Space_num)
				{
					
					//保存変数からマップへもどす
					map[i][j] = tmp_map[i][j];
				}

				if (map[i][j] == Space_num)
				{
					DrawGraph(i * 32, j * 32, gh[0], true);
				}
				else if (map[i][j] == P_num)
				{
					DrawGraph(i * 32, j * 32, gh[1], true);
				}
				else if (map[i][j] == Box_num)
				{
					DrawGraph(i * 32, j * 32, gh[2], true);
				}
				else if (map[i][j] == Goal_num)
				{
					
					DrawGraph(i * 32, j * 32, gh[3], true);
					//現在のゴールに3以外が入っても大丈夫なように保存変数に
					tmp_map[i][j] = Goal_num;
				}
				else if (map[i][j] == Wall_num)
				{
					DrawGraph(i * 32, j * 32, gh[4], true);
				}
			}
		}

		//移動の処理
		for (int i = 0; i < map_num; i++)
		{
			for (int j = 0; j < map_num; j++)
			{
				if (map[i][j] == P_num)
				{
					//下移動
					if (CheckHitKey(KEY_INPUT_DOWN))
					{
						if (Kdown == 0)
						{
							Kdown = 1;
							//箱の移動
							if (map[i][j + 1] == Box_num && map[i][j + 2] != Wall_num && map[i][j + 2] != Box_num)
							{
								map[i][j + 2] = Box_num;
								map[i][j + 1] = Space_num;
							}
							//プレイヤーの移動
							if (map[i][j + 1] == Space_num || map[i][j + 1] == Goal_num)
							{

								{
									map[i][j + 1] = P_num;
									map[i][j] = Space_num;
									i += 1;
								}
							}
							break;
						}
					}
					else
					{
						Kdown = 0;
					}
					//左移動
					if (CheckHitKey(KEY_INPUT_LEFT))
					{
						if (Kleft == 0)
						{
							Kleft = 1;
							if (map[i - 1][j] == Box_num && map[i - 2][j] != Wall_num && map[i - 2][j] != Box_num)
							{
								map[i - 2][j] = Box_num;
								map[i - 1][j] = Space_num;
							}
							if (map[i - 1][j] == Space_num || map[i - 1][j] == Goal_num)
							{
								map[i - 1][j] = P_num;
								map[i][j] = Space_num;
							}
							break;
						}
					}
					else
					{
						Kleft = 0;
					}
					//右移動
					if (CheckHitKey(KEY_INPUT_RIGHT))
					{
						if (Kright == 0)
						{
							Kright = 1;
							if (map[i + 1][j] == Box_num && map[i + 2][j] != Wall_num && map[i + 2][j] != Box_num)
							{
								map[i + 2][j] = Box_num;
								map[i + 1][j] = Space_num;
							}
							if (map[i + 1][j] == Space_num || map[i + 1][j] == Goal_num)
							{
								map[i + 1][j] = P_num;
								map[i][j] = Space_num;
							}
							break;
						}
					}
					else
					{
						Kright = 0;
					}
					//上移動
					if (CheckHitKey(KEY_INPUT_UP))
					{
						if (Kup == 0)
						{
							Kup = 1;
							if (map[i][j - 1] == Box_num && map[i][j - 2] != Wall_num && map[i][j - 2] != Box_num)
							{
								map[i][j - 2] = Box_num;
								map[i][j - 1] = Space_num;
							}
							if (map[i][j - 1] == Space_num || map[i][j - 1] == Goal_num)
							{
								map[i][j - 1] = P_num;
								map[i][j] = Space_num;
								i -= 1;
							}
							break;
						}
					}
					else
					{
						Kup = 0;
					}

				}
			}
		}
	
		//--------------------------------------------------------------------

		ScreenFlip(); //裏画面データを表画面へ反映

		//毎ループ呼び出す。エラーになった場合breakする
		if (ProcessMessage() == -1)break;
		//エスケープキーを押したり、エラーになった場合、breakする
		if (CheckHitKey(KEY_INPUT_ESCAPE))break;
	}

	//画像削除
	DeleteGraph(gh[0]);

	//WaitKey();	 //キー入力待ち
	DxLib_End(); //DXライブラリ使用の終了処理
	return 0;
}