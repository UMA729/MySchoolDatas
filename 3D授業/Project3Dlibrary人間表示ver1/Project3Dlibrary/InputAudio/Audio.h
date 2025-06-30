#pragma once
//C++STL関連のインクルード
#include <iostream>
#include <tchar.h>
#include <memory>
#include <wrl/client.h>	//COMのスマート化

//ネームスペースショートカット
using namespace std;
using namespace Microsoft::WRL;


//-------------------------------------------------------
//オーディオ
namespace AUD
{
	//サウンドタイプ定数	
	constexpr int TYPE_EFFECT{ 0 };	//効果音(単発音）
	constexpr int TYPE_BACK_MUSIC{ 1 };	//バックミュージック (ループ)

	//インターフェース										
	float Volume(int id, float t);					//各ボリューム変更
	float VolumeMaster(float t);					//マスターボリューム変更
	void Stop(int id);								//ストップ
	void Start(int id);								//スタート
	void DeleteSudio();								//サウンド情報破棄
	void LoadAudio(int id, const char name[], int type);	//サウンドボイスデータ登録

	//初期化				 
	void Initialize();
	//解放
	void Release();
}
//-------------------------------------------------------