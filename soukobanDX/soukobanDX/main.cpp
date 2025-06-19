#include "DxLib.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	SetOutApplicationLogValidFlag(false);//���O�o�̓I�t
	ChangeWindowMode(TRUE); //�E�B���h�E���[�h�؂�ւ�
	SetGraphMode(224, 224, 32); //�E�B���h�E�T�C�Y

	if (DxLib_Init() == -1) { //DX���C�u��������������
		return -1;			  //�G���[���N�����璼���ɏI��
	}

	SetDrawScreen(DX_SCREEN_BACK); //�`���𗠉�ʂɕύX
	SetWindowText("soukoban"); //�E�B���h�E�̖��O

	//�ϐ��̐錾---------------

	const int Space_num = 0;	//�󔒔ԍ�
	const int P_num = 1;		//�ǔԍ�
	const int Box_num = 2;		//�v���C���[�ԍ�
	const int Goal_num = 3;		//���ԍ�
	const int Wall_num = 4;		//�S�[���ԍ�

	const int map_num = 7;		//�}�b�v�̔z��

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

	//�t�H���g�T�C�Y�ύX
	SetFontSize(50);

	while (1)
	{
		//����ʂ̃f�[�^��S�č폜
		ClearDrawScreen();

		//����----------------------------------------------------------------
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{

				if (tmp_map[i][j] == Goal_num && map[i][j] == Space_num)
				{
					
					//�ۑ��ϐ�����}�b�v�ւ��ǂ�
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
					//���݂̃S�[����3�ȊO�������Ă����v�Ȃ悤�ɕۑ��ϐ���
					tmp_map[i][j] = Goal_num;
				}
				else if (map[i][j] == Wall_num)
				{
					DrawGraph(i * 32, j * 32, gh[4], true);
				}
			}
		}

		//�ړ��̏���
		for (int i = 0; i < map_num; i++)
		{
			for (int j = 0; j < map_num; j++)
			{
				if (map[i][j] == P_num)
				{
					//���ړ�
					if (CheckHitKey(KEY_INPUT_DOWN))
					{
						if (Kdown == 0)
						{
							Kdown = 1;
							//���̈ړ�
							if (map[i][j + 1] == Box_num && map[i][j + 2] != Wall_num && map[i][j + 2] != Box_num)
							{
								map[i][j + 2] = Box_num;
								map[i][j + 1] = Space_num;
							}
							//�v���C���[�̈ړ�
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
					//���ړ�
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
					//�E�ړ�
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
					//��ړ�
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

		ScreenFlip(); //����ʃf�[�^��\��ʂ֔��f

		//�����[�v�Ăяo���B�G���[�ɂȂ����ꍇbreak����
		if (ProcessMessage() == -1)break;
		//�G�X�P�[�v�L�[����������A�G���[�ɂȂ����ꍇ�Abreak����
		if (CheckHitKey(KEY_INPUT_ESCAPE))break;
	}

	//�摜�폜
	DeleteGraph(gh[0]);

	//WaitKey();	 //�L�[���͑҂�
	DxLib_End(); //DX���C�u�����g�p�̏I������
	return 0;
}