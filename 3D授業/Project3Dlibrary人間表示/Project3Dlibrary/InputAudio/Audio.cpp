#include <XAudio2.h>
#include <vector>
#include <fstream>
#include <algorithm>

#include "Audio.h"

#pragma comment(lib, "XAudio2.lib")

using namespace Microsoft::WRL;
using namespace std;


namespace AUD
{
	//サウンド最大数
	constexpr int SOUND_MAX{ 64 };		//ロード最大数
	constexpr int SOUND_SE_MAX{ 32 };	//SE同時最大数

	//サウンドデータクラス
	class SoundData
	{
		public:
		//サウンド情報(RIFFファイルフォーマットを読み取り用)
		unsigned int   m_iSize;			//サウンドサイズ
		unsigned char* m_pWave;			//wave先頭アドレスポインタ
		unsigned char* m_pData;			//波状情報アドレスポインタ
		//Audioデータ
		IXAudio2SourceVoice** m_ppSourceVoice;		//サウンドボイスインターフェース
		IXAudio2SubmixVoice* m_pSFXSubmixVoice;		//サブミクスインターフェース
		XAUDIO2_BUFFER			m_cSoundBuffer;		//サウンドバッファ
		float					m_fVolume;			//ボリューム情報
		int						m_iType;			//サウンドタイプ
	};

	ComPtr<IXAudio2>			g_pXAudio2;				//XAudio2オブジェクト
	IXAudio2MasteringVoice*	    g_pMasteringVoice;		//マスターボイス
	bool						g_is_error_sound;		//サウンドドライブチェック
	SoundData					g_oSudioData[SOUND_MAX];//サウンドデータ
	float						g_fMasterVolume;		//マスターボリューム

	WORD  GetWord(const unsigned char* pData);
	DWORD GetDword(const unsigned char* pData);
	void  FindChunk(const unsigned char* pData, const char* pChunckName, unsigned* Size, unsigned char** ppData);

	//AUD初期化
	void Initialize()
	{
		unsigned XAudio2CreateFlags = 0;

		XAudio2Create(&g_pXAudio2, XAudio2CreateFlags);

		//マスターボイス作成
		g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);

		//マスターボイスエラーチェック
		g_is_error_sound = false;
		if (g_pMasteringVoice == nullptr)
		{
			MessageBox(0, L"ｻｳﾝﾄﾞﾊｰﾄﾞ系のｻﾎﾟｰﾄに問題があるため、音がなりません", nullptr, MB_OK);
			g_is_error_sound = true;
			return;
		}

		//オーディオデータ作成
		for (int i = 0; i < SOUND_MAX; i++)
		{
			//各メンバ初期化	
			g_oSudioData[i].m_pData = nullptr;
			g_oSudioData[i].m_pWave = nullptr;
			g_oSudioData[i].m_iSize = 0;
			g_oSudioData[i].m_iType = TYPE_EFFECT;

			//サウンドインターフェースメモリ作成
			g_oSudioData[i].m_ppSourceVoice = new IXAudio2SourceVoice * [SOUND_SE_MAX];
			for (int j = 0; j < SOUND_SE_MAX; j++)
			{
				g_oSudioData[i].m_ppSourceVoice[j] = nullptr;
			}

			//ミックスボイス作成
			g_pXAudio2->CreateSubmixVoice(&g_oSudioData[i].m_pSFXSubmixVoice, 1, 44100, 0, 0, 0, 0);
		}

		//マスターボリューム
		g_pMasteringVoice->SetVolume(0.5f, 0);
		g_fMasterVolume = 0.5f;

	}
	//各ボリューム変更
	float Volume(int id, float t)
	{
		//マスターボイスエラーチェック
		if (g_is_error_sound == true)
			return 0.0f;

		if (id < 0 || id>SOUND_MAX)
			return -1.0f;

		g_oSudioData[id].m_fVolume += t;
		if (g_oSudioData[id].m_fVolume < 0.0f) g_oSudioData[id].m_fVolume = 0.0f;
		if (g_oSudioData[id].m_fVolume > 1.0f) g_oSudioData[id].m_fVolume = 1.0f;

		g_oSudioData[id].m_pSFXSubmixVoice->SetVolume(g_oSudioData[id].m_fVolume, 0);
		return g_oSudioData[id].m_fVolume;
	}

	//マスターボリューム変更
	float VolumeMaster(float t)
	{
		//マスターボイスエラーチェック
		if (g_is_error_sound == true)
		{
			return 0.0f;
		}

		g_fMasterVolume += t;
		if (g_fMasterVolume < 0.0f) g_fMasterVolume = 0.0f;
		if (g_fMasterVolume > 1.0f) g_fMasterVolume = 1.0f;
		g_pMasteringVoice->SetVolume(g_fMasterVolume, 0);
		return g_fMasterVolume;
	}

	//ストップ
	void Stop(int id)
	{
		//マスターボイスエラーチェック
		if (g_is_error_sound == true)
			return;

		if (id < 0 || id>SOUND_MAX)
			return;

		for (int i = 0; i < SOUND_SE_MAX; i++)
		{
			g_oSudioData[id].m_ppSourceVoice[i]->Stop();
			g_oSudioData[id].m_ppSourceVoice[i]->FlushSourceBuffers();
		}
	}

	//スタート
	void Start(int id)
	{
		//マスターボイスエラーチェック
		if (g_is_error_sound == true)
			return;

		if (id < 0 || id>SOUND_MAX)
			return;

		if (g_oSudioData[id].m_iType == TYPE_BACK_MUSIC)
		{
			//ループサウンド
			//ループスタートの際、破棄する
			Stop(id);
			g_oSudioData[id].m_ppSourceVoice[0]->SubmitSourceBuffer(&g_oSudioData[id].m_cSoundBuffer);
			g_oSudioData[id].m_ppSourceVoice[0]->Start();
		}
		else
		{
			//エフェクトサウンド
			for (int i = 0; i < (SOUND_SE_MAX << 1); i++)
			{
				//サウンド配列二周の間で使用されていない場所確認する
				int count = i % SOUND_SE_MAX;

				//サウンドの状態を取得
				XAUDIO2_VOICE_STATE stats;
				g_oSudioData[id].m_ppSourceVoice[count]->GetState(&stats);

				//サウンドバッファが空かどうかチェック
				if (stats.BuffersQueued == 0)
				{
					//サウンドインターフェースにサウンドを登録してサウンドを鳴らす
					g_oSudioData[id].m_ppSourceVoice[count]->SubmitSourceBuffer(&g_oSudioData[id].m_cSoundBuffer);
					g_oSudioData[id].m_ppSourceVoice[count]->Start();
					return;
				}
			}
		}
		return;
	}

	//サウンド情報破棄
	void DeleteSudio()
	{
		//マスターボイスエラーチェック
		if (g_is_error_sound == true)
		{
			return;
		}

		for (int i = 0; i < SOUND_MAX; i++)
		{
			//インターフェース破棄
			for (int j = 0; j < SOUND_SE_MAX; j++)
			{
				if (g_oSudioData[i].m_ppSourceVoice[j] != nullptr)
				{
					g_oSudioData[i].m_ppSourceVoice[j]->Stop();
					g_oSudioData[i].m_ppSourceVoice[j]->FlushSourceBuffers();
					g_oSudioData[i].m_ppSourceVoice[j]->DestroyVoice();
					g_oSudioData[i].m_ppSourceVoice[j] = nullptr;
				}
			}

			//サウンドデータ破棄
			if (g_oSudioData[i].m_pWave != nullptr)
			{
				delete[]g_oSudioData[i].m_pWave;
				g_oSudioData[i].m_pWave = nullptr;
			}
		}
	}

	//AUD解放
	void Release()
	{
		//サウンドデータの破棄
		DeleteSudio();

		//サウンドメモリ破棄
		for (int i = 0; i < SOUND_MAX; i++)
		{
			//サウンドインターフェース破棄
			delete[] g_oSudioData[i].m_ppSourceVoice;

			//ミックスサウンド破棄
			g_oSudioData[i].m_pSFXSubmixVoice->DestroyVoice();
		}

		//XAudioの破棄
		g_pMasteringVoice->DestroyVoice();
		g_pXAudio2.Reset();
	}

	//サウンドボイスデータ登録
	void LoadAudio(int id, const char name[], int type)
	{
		//マスターボイスエラーチェック
		if (g_is_error_sound == true)
			return;
		//要素オーバーチェック
		if (id < 0 || id>SOUND_MAX)
			return;

		//インターフェース破棄
		if (g_oSudioData[id].m_ppSourceVoice[0] != nullptr)
		{
			for (int j = 0; j < SOUND_SE_MAX; j++)
			{
				if (g_oSudioData[id].m_ppSourceVoice[j] != nullptr)
				{
					g_oSudioData[id].m_ppSourceVoice[j]->Stop();
					g_oSudioData[id].m_ppSourceVoice[j]->FlushSourceBuffers();
					g_oSudioData[id].m_ppSourceVoice[j]->DestroyVoice();
					g_oSudioData[id].m_ppSourceVoice[j] = nullptr;
				}
			}

			//サウンドデータ破棄
			if (g_oSudioData[id].m_pWave != nullptr)
				delete[]g_oSudioData[id].m_pWave;
		}

		//サウンドタイプセット
		g_oSudioData[id].m_iType = type;

		//waveファイルのメモリ内に展開
		ifstream ifs(name, ios::in | ios::binary);
		if (!ifs)	return;

		unsigned int Size = 0;
		//展開
		ifs.seekg(0, ios::end);
		Size = static_cast<unsigned>(ifs.tellg());
		ifs.seekg(0, ios::beg);
		g_oSudioData[id].m_pWave = new unsigned char[Size];

		ifs.read(reinterpret_cast<char*>(g_oSudioData[id].m_pWave), Size);
		ifs.close();

		//RIFFファイル解析
		//wave情報
		WAVEFORMATEX WaveformatEx = { 0 };
		FindChunk(g_oSudioData[id].m_pWave, "fmt", &g_oSudioData[id].m_iSize, &g_oSudioData[id].m_pData);

		//wave情報分アドレスをずらして波状データに移動
		unsigned char* p = g_oSudioData[id].m_pData;
		WaveformatEx.wFormatTag		= GetWord(p);	p += sizeof(WORD);
		WaveformatEx.nChannels		= GetWord(p);	p += sizeof(WORD);
		WaveformatEx.nSamplesPerSec = GetDword(p);	p += sizeof(DWORD);
		WaveformatEx.nAvgBytesPerSec= GetDword(p);	p += sizeof(DWORD);
		WaveformatEx.nBlockAlign	= GetWord(p);	p += sizeof(WORD);
		WaveformatEx.wBitsPerSample = GetWord(p);	p += sizeof(WORD);
		WaveformatEx.cbSize			= GetWord(p);	p += sizeof(WORD);

		//波本体のデータ
		FindChunk(g_oSudioData[id].m_pData, "data", &g_oSudioData[id].m_iSize, &g_oSudioData[id].m_pData);

		//再生のためのインターフェース生成
		for (int i = 0; i < SOUND_SE_MAX; i++)
		{
			//サブミクスボイスをセット
			XAUDIO2_SEND_DESCRIPTOR data;
			data.Flags = 0;
			data.pOutputVoice = g_oSudioData[id].m_pSFXSubmixVoice;
			XAUDIO2_VOICE_SENDS SFXSendList;
			memset(&SFXSendList, 0x00, sizeof(XAUDIO2_VOICE_SENDS));
			SFXSendList.SendCount = 1;
			SFXSendList.pSends = &data;
			//ソースボイス作成
			g_pXAudio2->CreateSourceVoice(&g_oSudioData[id].m_ppSourceVoice[i],
				&WaveformatEx, 0U, 2.0F, 0, &SFXSendList, 0);
		}
		//サウンドバッファをソースボイスキューに作成
		memset(&g_oSudioData[id].m_cSoundBuffer, 0x00, sizeof(g_oSudioData[id].m_cSoundBuffer));
		g_oSudioData[id].m_cSoundBuffer.AudioBytes = g_oSudioData[id].m_iSize;
		g_oSudioData[id].m_cSoundBuffer.pAudioData = reinterpret_cast<BYTE*>(g_oSudioData[id].m_pData);
		g_oSudioData[id].m_cSoundBuffer.Flags = XAUDIO2_END_OF_STREAM;

		//ループ設定
		if (type == TYPE_EFFECT)
			g_oSudioData[id].m_cSoundBuffer.LoopCount = XAUDIO2_NO_LOOP_REGION;
		else
			g_oSudioData[id].m_cSoundBuffer.LoopCount = XAUDIO2_LOOP_INFINITE;

		//ボリューム設定
		g_oSudioData[id].m_fVolume = 0.5f;
		g_oSudioData[id].m_pSFXSubmixVoice->SetVolume(g_oSudioData[id].m_fVolume, 0);
	}

	//型にデータを合わせて入れる
	WORD GetWord(const unsigned char* pData)
	{
		WORD Value = pData[0] | pData[1] << 8;
		return Value;
	}
	DWORD GetDword(const unsigned char* pData)
	{
		DWORD Value = pData[0] | pData[1] << 8 | pData[2] << 16 | pData[3] << 24;
		return Value;
	}

	//かならずチャンクは見つかるとして簡略化
	void FindChunk(const unsigned char* pData, const char* pChunckName, unsigned* Size, unsigned char** ppData)
	{
		const unsigned CHUNKNAME_LENGTH = std::strlen(pChunckName);
		while (true)
		{
			bool IsFind = true;
			for (unsigned i = 0; i < CHUNKNAME_LENGTH; ++i)
			{
				if (pData[i] != pChunckName[i])
				{
					IsFind = false;
					break;
				}
			}
			if (IsFind)
			{
				*Size = pData[4 + 0] | pData[4 + 1] << 8 | pData[4 + 2] << 16 | pData[4 + 3] << 24;
				*ppData = const_cast<unsigned char*>(pData + 8);
				return;
			}

			pData++;
		}
		return;
	}
}