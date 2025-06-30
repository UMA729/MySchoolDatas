#define WIN32_LEAN_AND_MEAN //windows.h�K�v�Œ���̃w�b�_�݂̂̃C���N���[�h�ɂ���
//C++STL�֘A�̃C���N���[�h
#include <iostream>
#include <tchar.h>
#include <memory>


//�C���N���[�h
#include <windows.h>
#include "InputAudio/Input.h"
#include "InputAudio/Audio.h"
#include "DrectX12/DX12Library.h"
#include "ShaderGraphicPipeline/GraphicPipeLine.h"
#include "BufferAndImg/Buffer.h"

//���f��
#include "ModelStructs/RenPolygonModel.h"	//�����_�����O�e�N�X�`���`��|���S��
#include "ModelStructs/3DcubeModel.h"		//�L���[�u���f��

//�����_�����O�A�N�V�����N���X
#include "RenderingAction/RenderAction.h"

//�X���b�h�N���X
#include "ThreadOutput/ThreadOutput.h"

//�l�[���X�y�[�X�V���[�g�J�b�g
using namespace std;


//�萔
constexpr auto WINDOW_CLASS{ _T("DirectX12") };
constexpr auto WINDOW_TITLE{ WINDOW_CLASS };
constexpr auto WINDOW_STYLE{ WS_OVERLAPPEDWINDOW };
constexpr auto WINDOW_WIDTH{ 800 };
constexpr auto WINDOW_HEIGHT{ 720 };
constexpr auto WINDOW_LOOP{ 1 };


//DirectX12�\�z
unique_ptr <DX12>			DevX12;
//�R�}���h�N���X
unique_ptr<COMMAND>			Com;
//�����_�����O�o�b�t�@
unique_ptr<RENDER>			Render;

//�p�C�v���C��
unique_ptr<DrawPipeLine>		DrawPipe;
unique_ptr<Cube3dDrawPipeLine>	CubePipe;

//���f��
unique_ptr<Model_RenderPolygon> RenderPolygon;	//�����_�����O�e�N�X�`���`��|���S��
unique_ptr<Model_3dCube> Cuble3D;				//�L���[�u���f��


//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

//���C���֐�
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, TCHAR* lpszCmdLine, int nCmdShow)
{
	// �E�B���h�E�N���X��o�^
	WNDCLASSEX	wndclass{};
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndclass.lpfnWndProc = WindowProc;
	wndclass.hInstance = hInstance;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = WINDOW_CLASS;
	RegisterClassEx(&wndclass);

	RECT windowRect{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&windowRect, WINDOW_STYLE, FALSE);

	// �E�B���h�E�쐬
	HWND hWnd = CreateWindow(
		WINDOW_CLASS, WINDOW_TITLE, WINDOW_STYLE,
		10, 10,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL, NULL, hInstance, NULL);

	//�E�B���h�E�\��
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	//DirectX12�\�z
	DevX12.reset(new DX12(hWnd));
	//�R�}���h�N���X(�X���b�v�`�F�C���p)
	Com.reset(new COMMAND(DevX12->GetDev(), D3D12_COMMAND_LIST_TYPE_DIRECT));
	
	//�����_�����O�o�b�t�@
	Render.reset(new RENDER(DevX12->GetDev(),Com->GetQueue(), DevX12->GetFac(),hWnd, WINDOW_WIDTH, WINDOW_HEIGHT));
	//�o�̓X���b�h
	unique_ptr<ThrendOutput> output(new ThrendOutput());
	//�C���v�b�g&�I�[�f�B�I
	INP::Initialize();
	AUD::Initialize();
	//GDI+
	GDI_OBJ::Initialize();

	
	//�p�C�v���C��
	DrawPipe.reset(new DrawPipeLine(DevX12->GetDev()));				//�v���C�}���h���[�p�p�C�v���C��
	CubePipe.reset(new Cube3dDrawPipeLine(DevX12->GetDev()));		//�L���[�u���f���p�C�v���C��

	//���f���\��
	RenderPolygon.reset(new Model_RenderPolygon(DevX12->GetDev())); //���f���Q�����_�����O�e�N�X�`���`��|���S��
	Cuble3D.reset(new Model_3dCube(DevX12->GetDev()));				//���f���Q������

	//�����_�����O�A�N�V����
	unique_ptr<RenderAction_Output> ren_act_out;					//�����_�����O�A�N�V�����F�A�E�g�v�b�g
	ren_act_out.reset(new RenderAction_Output(DevX12->GetDev(), move(Com)));
	unique_ptr<RenderAction_3D_Cube> ren_act_cube;					//�����_�����O�A�N�V�����F3D�L���[�u���f��
	ren_act_cube.reset( new RenderAction_3D_Cube(DevX12->GetDev()));
	ren_act_cube->Init(0, 0, 0);
	

	
	//�Q�[�����[�v
	atomic<bool> game_loop{ true };
	unique_ptr<thread> thread_game(new thread([&]() 
	{
			int time_add=0;
			int time_count=0;

			while (game_loop.load(memory_order_acquire))
			{
				auto start = std::chrono::steady_clock::now();//�P�t���[���v�Z�J�n����
	
				//----�����_�����O�e�N�X�`���ɕ`��----------------
				ren_act_cube->Action();
				output->StartRun(ren_act_cube->GetCom());
				
				//--------------------------------------------------
				//�����_�����O�e�N�X�`�������o�b�N�o�b�t�@�ɕ`��
				ren_act_out->Action();
				output->StartRun(ren_act_out->GetCom());
				//--------------------------------------------------
				
				while (1)
				{
					auto end = std::chrono::steady_clock::now();//�P�t���[���v�Z�I������

					//�P�t���[���v�Z
					auto dur = end - start;
					auto micro = std::chrono::duration_cast<std::chrono::microseconds> (dur).count();
					//�P�t���[������i16sm�j
					int stop = (int)micro - 16666;

					if (stop >= 0)
					{
						time_add += (int)micro;
						time_count++;
						break;
					}


				}


				//���σt���[�����[�g�v�Z
				if (time_count > 500)
				{
					wstring s = to_wstring(time_add / time_count / 1000);
					s += L",\n";
					OutputDebugString(s.c_str());
					time_count = 0;
					time_add = 0;
				}

				//�A�E�g�v�b�g�����_�����O�I���`�F�b�N
				while (1)
				{
					if (output->CheckRun() == false)
						break;
				}
				//�o�b�N����v���C�}��
				ren_act_out->ActionPresent();

			}
	}));
	

	// �E�B���h�E���b�Z�[�W���[�v
	MSG	msg;
	while (WINDOW_LOOP)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	game_loop.store(false, memory_order_release);
	thread_game->join();
	thread_game.reset();
	
	ren_act_out.reset();
	output.reset();

	Cuble3D.reset();
	RenderPolygon.reset();

	//���
	DrawPipe.reset();
	Render.reset();
	DevX12.reset();
	
	GDI_OBJ::Release();
	AUD::Release();
	INP::Release();
	return 0;
}

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	//�C���v�b�g�Ƀ}�E�X���擾������
	INP::WindowProcUpdate(hWnd, &nMsg, &lParam, WINDOW_WIDTH, WINDOW_HEIGHT);

	switch (nMsg)
	{
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, nMsg, wParam, lParam);
	}
	return 0;
}