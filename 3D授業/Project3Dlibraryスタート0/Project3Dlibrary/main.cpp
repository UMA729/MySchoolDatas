#define WIN32_LEAN_AND_MEAN //windows.h必要最低限のヘッダのみのインクルードにする
//C++STL関連のインクルード
#include <iostream>
#include <tchar.h>
#include <memory>


//インクルード
#include <windows.h>
#include "InputAudio/Input.h"
#include "InputAudio/Audio.h"
#include "DrectX12/DX12Library.h"
#include "ShaderGraphicPipeline/GraphicPipeLine.h"
#include "BufferAndImg/Buffer.h"

//モデル
#include "ModelStructs/RenPolygonModel.h"	//レンダリングテクスチャ描画ポリゴン
#include "ModelStructs/3DcubeModel.h"		//キューブモデル

//レンダリングアクションクラス
#include "RenderingAction/RenderAction.h"

//スレッドクラス
#include "ThreadOutput/ThreadOutput.h"

//ネームスペースショートカット
using namespace std;


//定数
constexpr auto WINDOW_CLASS{ _T("DirectX12") };
constexpr auto WINDOW_TITLE{ WINDOW_CLASS };
constexpr auto WINDOW_STYLE{ WS_OVERLAPPEDWINDOW };
constexpr auto WINDOW_WIDTH{ 800 };
constexpr auto WINDOW_HEIGHT{ 720 };
constexpr auto WINDOW_LOOP{ 1 };


//DirectX12構築
unique_ptr <DX12>			DevX12;
//コマンドクラス
unique_ptr<COMMAND>			Com;
//レンダリングバッファ
unique_ptr<RENDER>			Render;

//パイプライン
unique_ptr<DrawPipeLine>		DrawPipe;
unique_ptr<Cube3dDrawPipeLine>	CubePipe;

//モデル
unique_ptr<Model_RenderPolygon> RenderPolygon;	//レンダリングテクスチャ描画ポリゴン
unique_ptr<Model_3dCube> Cuble3D;				//キューブモデル


//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

//メイン関数
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, TCHAR* lpszCmdLine, int nCmdShow)
{
	// ウィンドウクラスを登録
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

	// ウィンドウ作成
	HWND hWnd = CreateWindow(
		WINDOW_CLASS, WINDOW_TITLE, WINDOW_STYLE,
		10, 10,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL, NULL, hInstance, NULL);

	//ウィンドウ表示
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	//DirectX12構築
	DevX12.reset(new DX12(hWnd));
	//コマンドクラス(スワップチェイン用)
	Com.reset(new COMMAND(DevX12->GetDev(), D3D12_COMMAND_LIST_TYPE_DIRECT));
	
	//レンダリングバッファ
	Render.reset(new RENDER(DevX12->GetDev(),Com->GetQueue(), DevX12->GetFac(),hWnd, WINDOW_WIDTH, WINDOW_HEIGHT));
	//出力スレッド
	unique_ptr<ThrendOutput> output(new ThrendOutput());
	//インプット&オーディオ
	INP::Initialize();
	AUD::Initialize();
	//GDI+
	GDI_OBJ::Initialize();

	
	//パイプライン
	DrawPipe.reset(new DrawPipeLine(DevX12->GetDev()));				//プライマリドロー用パイプライン
	CubePipe.reset(new Cube3dDrawPipeLine(DevX12->GetDev()));		//キューブモデルパイプライン

	//モデル構造
	RenderPolygon.reset(new Model_RenderPolygon(DevX12->GetDev())); //モデル＿レンダリングテクスチャ描画ポリゴン
	Cuble3D.reset(new Model_3dCube(DevX12->GetDev()));				//モデル＿立方体

	//レンダリングアクション
	unique_ptr<RenderAction_Output> ren_act_out;					//レンダリングアクション：アウトプット
	ren_act_out.reset(new RenderAction_Output(DevX12->GetDev(), move(Com)));
	unique_ptr<RenderAction_3D_Cube> ren_act_cube;					//レンダリングアクション：3Dキューブモデル
	ren_act_cube.reset( new RenderAction_3D_Cube(DevX12->GetDev()));
	ren_act_cube->Init(0, 0, 0);
	

	
	//ゲームループ
	atomic<bool> game_loop{ true };
	unique_ptr<thread> thread_game(new thread([&]() 
	{
			int time_add=0;
			int time_count=0;

			while (game_loop.load(memory_order_acquire))
			{
				auto start = std::chrono::steady_clock::now();//１フレーム計算開始時間
	
				//----レンダリングテクスチャに描画----------------
				ren_act_cube->Action();
				output->StartRun(ren_act_cube->GetCom());
				
				//--------------------------------------------------
				//レンダリングテクスチャ情報をバックバッファに描画
				ren_act_out->Action();
				output->StartRun(ren_act_out->GetCom());
				//--------------------------------------------------
				
				while (1)
				{
					auto end = std::chrono::steady_clock::now();//１フレーム計算終了時間

					//１フレーム計算
					auto dur = end - start;
					auto micro = std::chrono::duration_cast<std::chrono::microseconds> (dur).count();
					//１フレーム制御（16sm）
					int stop = (int)micro - 16666;

					if (stop >= 0)
					{
						time_add += (int)micro;
						time_count++;
						break;
					}


				}


				//平均フレームレート計算
				if (time_count > 500)
				{
					wstring s = to_wstring(time_add / time_count / 1000);
					s += L",\n";
					OutputDebugString(s.c_str());
					time_count = 0;
					time_add = 0;
				}

				//アウトプットレンダリング終了チェック
				while (1)
				{
					if (output->CheckRun() == false)
						break;
				}
				//バックからプライマリ
				ren_act_out->ActionPresent();

			}
	}));
	

	// ウィンドウメッセージループ
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

	//解放
	DrawPipe.reset();
	Render.reset();
	DevX12.reset();
	
	GDI_OBJ::Release();
	AUD::Release();
	INP::Release();
	return 0;
}

// ウィンドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	//インプットにマウス情報取得させる
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