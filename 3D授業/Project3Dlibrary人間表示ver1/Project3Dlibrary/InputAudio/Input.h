#pragma once
//C++STL�֘A�̃C���N���[�h
#include <iostream>
#include <tchar.h>
#include <memory>
#include <wrl/client.h>	//COM�̃X�}�[�g��

//XInput���C���N���[�h
#include <XInput.h>


//�l�[���X�y�[�X�V���[�g�J�b�g
using namespace std;
using namespace Microsoft::WRL;

//�C���v�b�g�̃C���^�[�t�F�[�X
namespace INP
{
	//�Q�[���p�b�g�{�^����
	constexpr int	MAX_CONTROLERS{ 4 };   //�R���g���[���[�ő吔

	//�Q�[���p�b�g�{�^����
	enum class GAMEPAD_DPAD
	{
		UP = 0x0001,
		DOWN = 0x0002,
		LEFT = 0x0004,
		RIGHT = 0x0008,
		START = 0x0010,
		BACK = 0x0020,
		LEFT_THUMB = 0x0040,
		RIGHT_THUMB = 0x0080,
		LEFT_SHOULDER = 0x0100,
		RIGHT_SHOULDER = 0x0200,
		A = 0x1000,
		B = 0x2000,
		X = 0x4000,
		Y = 0x8000,
	};

	//�C���^�[�t�F�[�X
	bool  GetVKey(int v_key);					//�L�[�{�[�h
	bool  GetMouButtonL();						//�}�E�X�̃{�^��L
	bool  GetMouButtonR();						//�}�E�X�̃{�^��R
	int   GetMouX();							//�}�E�X�̈ʒuX
	int   GetMouY();							//�}�E�X�̈ʒuY
	float GetConVecStickLX(int id);				//�A�i���OL�����L�[���擾Y �߂�l(-1.0�`1.0)
	float GetConVecStickLY(int id);				//�A�i���OL�����L�[���擾X �߂�l(-1.0�`1.0)
	float GetConVecStickRX(int id);				//�A�i���OR�����L�[���擾Y �߂�l(-1.0�`1.0)
	float GetConVecStickRY(int id);				//�A�i���OR�����L�[���擾X �߂�l(-1.0�`1.0)
	bool  GetConButtons(int id, int button);	//�R���g���[���̃{�^�����擾
	void  SetVibration(int id, int wL, int wR);	//�o�C�u���[�V�����ݒ�
	//������
	void Initialize();
	//���
	void Release();
	//�A�b�v�f�[�g
	void UpdateXControlerConnected();
	bool WindowProcUpdate(HWND hWnd, UINT* uMsg, LPARAM* lParam, int w, int h);
}