#include "stdio.h"

int main()
{
	int act_num = 0; //行動方向
	int z_count = 0; //0の数のカウント

	int map[5][5] = {
	{1,1,1,1,1 },
	{1,2,0,0,1},
	{1,0,0,0,1},
	{1,0,0,0,1},
	{1,1,1,1,1 },
	};

	//ゲームが終わるまでループさせる
	while (1)
	{
		z_count = 0;
		//マップを表示させる
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (map[i][j] == 0)
				{
					z_count++;
				}
				//マップに1がある場所に壁テキストの出力
				if (map[i][j] == 1)
				{
					printf("壁");
				}
				//マップに2がある場所に壁テキストの出力
				else if (map[i][j] == 2)
				{
					printf("Ｐ");
				}
				else if (map[i][j] == 3)
				{
					printf("・");
				}
				else
				{
					printf("　");
				}
			}
			printf("\n");
		}
		//ゲーム終了の処理
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{

				if (z_count == 0)
				{
					//0のカウントが0ならばクリア
					printf("game clear");
					return 0;
				}
				else if (z_count > 0)
				{
					if (map[i][j] == 2)
					{
						if (map[i + 1][j] != 0 &&
							map[i][j - 1] != 0 &&
							map[i][j + 1] != 0 &&
							map[i - 1][j] != 0)
						{
							//0のカウントが1以上でまわりが0以外でゲームオーバー
							printf("game over");
							return 0;
						}
					}
				}
			}
		}
		//操作方法
		printf("下,2,左4,右6,上8\n");


		scanf_s("%d", &act_num);

		//移動の処理
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (map[i][j] == 2)
				{
					if (act_num == 2 && map[i + 1][j] == 0)
					{
						map[i + 1][j] = 2;
						map[i][j] = 3;
						i += 1;
						break;
					}
					else if (act_num == 4 && map[i][j - 1] == 0)
					{
						map[i][j - 1] = 2;
						map[i][j] = 3;
						break;
					}
					else if (act_num == 6 && map[i][j + 1] == 0)
					{
						map[i][j + 1] = 2;
						map[i][j] = 3;
						break;
					}
					else if (act_num == 8 && map[i - 1][j] == 0)
					{
						map[i - 1][j] = 2;
						map[i][j] = 3;
						i -= 1;
						break;
					}
				}
			}
		}
	}
}