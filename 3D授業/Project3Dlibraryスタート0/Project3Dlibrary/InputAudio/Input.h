#pragma once
//C++STL関連のインクルード
#include <iostream>
#include <tchar.h>
#include <memory>
#include <wrl/client.h>	//COMのスマート化

//XInputをインクルード
#include <XInput.h>


//ネームスペースショートカット
using namespace std;
using namespace Microsoft::WRL;

//インプットのインターフェース
namespace INP
{
	//ゲームパットボタン列挙
	constexpr int	MAX_CONTROLERS{ 4 };   //コントローラー最大数

	//ゲームパットボタン列挙
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

	//インターフェース
	bool  GetVKey(int v_key);					//キーボード
	bool  GetMouButtonL();						//マウスのボタンL
	bool  GetMouButtonR();						//マウスのボタンR
	int   GetMouX();							//マウスの位置X
	int   GetMouY();							//マウスの位置Y
	float GetConVecStickLX(int id);				//アナログL方向キー情報取得Y 戻り値(-1.0～1.0)
	float GetConVecStickLY(int id);				//アナログL方向キー情報取得X 戻り値(-1.0～1.0)
	float GetConVecStickRX(int id);				//アナログR方向キー情報取得Y 戻り値(-1.0～1.0)
	float GetConVecStickRY(int id);				//アナログR方向キー情報取得X 戻り値(-1.0～1.0)
	bool  GetConButtons(int id, int button);	//コントローラのボタン情報取得
	void  SetVibration(int id, int wL, int wR);	//バイブレーション設定
	//初期化
	void Initialize();
	//解放
	void Release();
	//アップデート
	void UpdateXControlerConnected();
	bool WindowProcUpdate(HWND hWnd, UINT* uMsg, LPARAM* lParam, int w, int h);
}