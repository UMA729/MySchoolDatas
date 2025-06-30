#define WIN32_LEAN_AND_MEAN //windows.h�K�v�Œ���̃w�b�_�݂̂̃C���N���[�h�ɂ���
#pragma comment(lib,"xinput.lib ")

#include <Windows.h>
#include <XInput.h>
#include <wrl/client.h>	//COM�̃X�}�[�g��



//C++STL�֘A�̃C���N���[�h
#include <iostream>
#include <tchar.h>
#include <memory>
#include <vector>

#include "Input.h"

constexpr auto MAX_CONTROLERS{ 4 };
constexpr auto THRESHOLD{ 32767.0f }; //�W���C�X�e�B�b�N�̂������l

namespace INP
{
	//XBOX360��p�R���g���[���[�X�e�[�^�X
	struct CONTROLER_STATE
	{
		XINPUT_STATE	m_state;		//�R���g���[���[�X�e�[�^�X
		bool			m_bConnected;	//�ڑ����
	};

	//XBOX360��p�R���g���[���[�p�b�g
	CONTROLER_STATE GAME_PAD[MAX_CONTROLERS];

	int  g_Pos_x{ 0 }, g_Pos_y{0};		//�}�E�X�ʒu
	bool g_ButtonL{ false };			//�}�E�X�N���b�N�{�^��L
	bool g_ButtonR{ false };			//�}�E�X�N���b�N�{�^��R


	//�R���g���[���̐ڑ���Ԃ̊m�F
	void UpdateXControlerConnected()
	{
		DWORD dwResult;
		for (DWORD i = 0; i < MAX_CONTROLERS; i++)
		{
			dwResult = XInputGetState(i, &GAME_PAD[i].m_state);
			if (dwResult == ERROR_SUCCESS)
				GAME_PAD[i].m_bConnected = true;
			else
				GAME_PAD[i].m_bConnected = false;
		}


	}

	//�j��
	void Release()
	{
		Initialize();
	}

	//������
	void Initialize()
	{

		g_Pos_x = 0;
		g_Pos_y = 0;
		g_ButtonL = false;
		g_ButtonR = false;
	}

	//�E�B���h�E�v���V�[�W���o�^�p�֐�
	bool WindowProcUpdate(HWND hWnd, UINT* uMsg, LPARAM* lParam, int w, int h)
	{
		if (*uMsg == WM_MOUSEMOVE)
		{
			RECT rect;
			GetClientRect(hWnd, &rect);

			POINT point = { LOWORD(*lParam), HIWORD((*lParam)) };

			g_Pos_x =static_cast<int>( w * (point.x / (float)rect.right));   // �J�[�\���� x ���W
			g_Pos_y =static_cast<int>( h * (point.y / (float)rect.bottom));   // �J�[�\���� y ���W
			return true;
		}
		else if (*uMsg == WM_LBUTTONDOWN)
		{
			g_ButtonL = true;
		}
		else if (*uMsg == WM_LBUTTONUP)
		{
			g_ButtonL = false;
		}
		else if (*uMsg == WM_RBUTTONDOWN)
		{
			g_ButtonR = true;
		}
		else if (*uMsg == WM_RBUTTONUP)
		{
			g_ButtonR = false;
		}
		return false;
	}

	bool GetVKey(int v_key)
	{
		if (GetAsyncKeyState(v_key) & 0x8000)
			return true;
		else
			return false;
		return false;
	}
	bool GetMouButtonL()
	{
		return g_ButtonL;
	}
	bool GetMouButtonR()
	{
		return g_ButtonR;
	}

	int GetMouX()
	{
		return g_Pos_x;
	}

	int GetMouY()
	{
		return g_Pos_y;
	}

	//�A�i���O�����L�[���擾
	float GetConVecStickLX(int id)
	{
		if (GAME_PAD[id].m_bConnected == true)
		{
			float w = ((float)GAME_PAD[id].m_state.Gamepad.sThumbLX) / THRESHOLD;
			if (w < -1.0f) w = -1.0f;
			if (w > +1.0f) w = +1.0f;
			return w;
		}
		return 0;
	}

	float GetConVecStickLY(int id)
	{
		if (GAME_PAD[id].m_bConnected == true)
		{
			float w = ((float)GAME_PAD[id].m_state.Gamepad.sThumbLY) / THRESHOLD;
			if (w < -1.0f) w = -1.0f;
			if (w > +1.0f) w = +1.0f;
			return w;
		}
		return 0;
	}

	float GetConVecStickRX(int id)
	{
		if (GAME_PAD[id].m_bConnected == true)
		{
			float w = ((float)GAME_PAD[id].m_state.Gamepad.sThumbRX) / THRESHOLD;
			if (w < -1.0f) w = -1.0f;
			if (w > +1.0f) w = +1.0f;
			return w;
		}
		return 0;
	}

	float GetConVecStickRY(int id)
	{
		if (GAME_PAD[id].m_bConnected == true)
		{
			float w = ((float)GAME_PAD[id].m_state.Gamepad.sThumbRY) / THRESHOLD;
			if (w < -1.0f) w = -1.0f;
			if (w > +1.0f) w = +1.0f;
			return w;
		}
		return 0;
	}


	//�R���g���[���̃{�^�����擾
	bool GetConButtons(int id, int button)
	{
		if (GAME_PAD[id].m_bConnected == true)
		{
			if (GAME_PAD[id].m_state.Gamepad.wButtons & button)
			{
				return true;
			}
		}

		return false;
	}

	//�o�C�u���[�V�����ݒ�
	void SetVibration(int id, int wL, int wR)
	{
		if (GAME_PAD[id].m_bConnected == true)
		{
			XINPUT_VIBRATION vibration;
			ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
			vibration.wLeftMotorSpeed = wL; // use any value between 0-65535 here
			vibration.wRightMotorSpeed = wR; // use any value between 0-65535 here
			XInputSetState(id, &vibration);
		}
	}
}