#include "stdio.h"

int main()
{
	int act_num = 0; //�s������
	int z_count = 0; //0�̐��̃J�E���g

	int map[5][5] = {
	{1,1,1,1,1 },
	{1,2,0,0,1},
	{1,0,0,0,1},
	{1,0,0,0,1},
	{1,1,1,1,1 },
	};

	//�Q�[�����I���܂Ń��[�v������
	while (1)
	{
		z_count = 0;
		//�}�b�v��\��������
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (map[i][j] == 0)
				{
					z_count++;
				}
				//�}�b�v��1������ꏊ�ɕǃe�L�X�g�̏o��
				if (map[i][j] == 1)
				{
					printf("��");
				}
				//�}�b�v��2������ꏊ�ɕǃe�L�X�g�̏o��
				else if (map[i][j] == 2)
				{
					printf("�o");
				}
				else if (map[i][j] == 3)
				{
					printf("�E");
				}
				else
				{
					printf("�@");
				}
			}
			printf("\n");
		}
		//�Q�[���I���̏���
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{

				if (z_count == 0)
				{
					//0�̃J�E���g��0�Ȃ�΃N���A
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
							//0�̃J�E���g��1�ȏ�ł܂�肪0�ȊO�ŃQ�[���I�[�o�[
							printf("game over");
							return 0;
						}
					}
				}
			}
		}
		//������@
		printf("��,2,��4,�E6,��8\n");


		scanf_s("%d", &act_num);

		//�ړ��̏���
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