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
	//�T�E���h�ő吔
	constexpr int SOUND_MAX{ 64 };		//���[�h�ő吔
	constexpr int SOUND_SE_MAX{ 32 };	//SE�����ő吔

	//�T�E���h�f�[�^�N���X
	class SoundData
	{
		public:
		//�T�E���h���(RIFF�t�@�C���t�H�[�}�b�g��ǂݎ��p)
		unsigned int   m_iSize;			//�T�E���h�T�C�Y
		unsigned char* m_pWave;			//wave�擪�A�h���X�|�C���^
		unsigned char* m_pData;			//�g����A�h���X�|�C���^
		//Audio�f�[�^
		IXAudio2SourceVoice** m_ppSourceVoice;		//�T�E���h�{�C�X�C���^�[�t�F�[�X
		IXAudio2SubmixVoice* m_pSFXSubmixVoice;		//�T�u�~�N�X�C���^�[�t�F�[�X
		XAUDIO2_BUFFER			m_cSoundBuffer;		//�T�E���h�o�b�t�@
		float					m_fVolume;			//�{�����[�����
		int						m_iType;			//�T�E���h�^�C�v
	};

	ComPtr<IXAudio2>			g_pXAudio2;				//XAudio2�I�u�W�F�N�g
	IXAudio2MasteringVoice*	    g_pMasteringVoice;		//�}�X�^�[�{�C�X
	bool						g_is_error_sound;		//�T�E���h�h���C�u�`�F�b�N
	SoundData					g_oSudioData[SOUND_MAX];//�T�E���h�f�[�^
	float						g_fMasterVolume;		//�}�X�^�[�{�����[��

	WORD  GetWord(const unsigned char* pData);
	DWORD GetDword(const unsigned char* pData);
	void  FindChunk(const unsigned char* pData, const char* pChunckName, unsigned* Size, unsigned char** ppData);

	//AUD������
	void Initialize()
	{
		unsigned XAudio2CreateFlags = 0;

		XAudio2Create(&g_pXAudio2, XAudio2CreateFlags);

		//�}�X�^�[�{�C�X�쐬
		g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);

		//�}�X�^�[�{�C�X�G���[�`�F�b�N
		g_is_error_sound = false;
		if (g_pMasteringVoice == nullptr)
		{
			MessageBox(0, L"�����ʰ�ތn�̻�߰Ăɖ�肪���邽�߁A�����Ȃ�܂���", nullptr, MB_OK);
			g_is_error_sound = true;
			return;
		}

		//�I�[�f�B�I�f�[�^�쐬
		for (int i = 0; i < SOUND_MAX; i++)
		{
			//�e�����o������	
			g_oSudioData[i].m_pData = nullptr;
			g_oSudioData[i].m_pWave = nullptr;
			g_oSudioData[i].m_iSize = 0;
			g_oSudioData[i].m_iType = TYPE_EFFECT;

			//�T�E���h�C���^�[�t�F�[�X�������쐬
			g_oSudioData[i].m_ppSourceVoice = new IXAudio2SourceVoice * [SOUND_SE_MAX];
			for (int j = 0; j < SOUND_SE_MAX; j++)
			{
				g_oSudioData[i].m_ppSourceVoice[j] = nullptr;
			}

			//�~�b�N�X�{�C�X�쐬
			g_pXAudio2->CreateSubmixVoice(&g_oSudioData[i].m_pSFXSubmixVoice, 1, 44100, 0, 0, 0, 0);
		}

		//�}�X�^�[�{�����[��
		g_pMasteringVoice->SetVolume(0.5f, 0);
		g_fMasterVolume = 0.5f;

	}
	//�e�{�����[���ύX
	float Volume(int id, float t)
	{
		//�}�X�^�[�{�C�X�G���[�`�F�b�N
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

	//�}�X�^�[�{�����[���ύX
	float VolumeMaster(float t)
	{
		//�}�X�^�[�{�C�X�G���[�`�F�b�N
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

	//�X�g�b�v
	void Stop(int id)
	{
		//�}�X�^�[�{�C�X�G���[�`�F�b�N
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

	//�X�^�[�g
	void Start(int id)
	{
		//�}�X�^�[�{�C�X�G���[�`�F�b�N
		if (g_is_error_sound == true)
			return;

		if (id < 0 || id>SOUND_MAX)
			return;

		if (g_oSudioData[id].m_iType == TYPE_BACK_MUSIC)
		{
			//���[�v�T�E���h
			//���[�v�X�^�[�g�̍ہA�j������
			Stop(id);
			g_oSudioData[id].m_ppSourceVoice[0]->SubmitSourceBuffer(&g_oSudioData[id].m_cSoundBuffer);
			g_oSudioData[id].m_ppSourceVoice[0]->Start();
		}
		else
		{
			//�G�t�F�N�g�T�E���h
			for (int i = 0; i < (SOUND_SE_MAX << 1); i++)
			{
				//�T�E���h�z�����̊ԂŎg�p����Ă��Ȃ��ꏊ�m�F����
				int count = i % SOUND_SE_MAX;

				//�T�E���h�̏�Ԃ��擾
				XAUDIO2_VOICE_STATE stats;
				g_oSudioData[id].m_ppSourceVoice[count]->GetState(&stats);

				//�T�E���h�o�b�t�@���󂩂ǂ����`�F�b�N
				if (stats.BuffersQueued == 0)
				{
					//�T�E���h�C���^�[�t�F�[�X�ɃT�E���h��o�^���ăT�E���h��炷
					g_oSudioData[id].m_ppSourceVoice[count]->SubmitSourceBuffer(&g_oSudioData[id].m_cSoundBuffer);
					g_oSudioData[id].m_ppSourceVoice[count]->Start();
					return;
				}
			}
		}
		return;
	}

	//�T�E���h���j��
	void DeleteSudio()
	{
		//�}�X�^�[�{�C�X�G���[�`�F�b�N
		if (g_is_error_sound == true)
		{
			return;
		}

		for (int i = 0; i < SOUND_MAX; i++)
		{
			//�C���^�[�t�F�[�X�j��
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

			//�T�E���h�f�[�^�j��
			if (g_oSudioData[i].m_pWave != nullptr)
			{
				delete[]g_oSudioData[i].m_pWave;
				g_oSudioData[i].m_pWave = nullptr;
			}
		}
	}

	//AUD���
	void Release()
	{
		//�T�E���h�f�[�^�̔j��
		DeleteSudio();

		//�T�E���h�������j��
		for (int i = 0; i < SOUND_MAX; i++)
		{
			//�T�E���h�C���^�[�t�F�[�X�j��
			delete[] g_oSudioData[i].m_ppSourceVoice;

			//�~�b�N�X�T�E���h�j��
			g_oSudioData[i].m_pSFXSubmixVoice->DestroyVoice();
		}

		//XAudio�̔j��
		g_pMasteringVoice->DestroyVoice();
		g_pXAudio2.Reset();
	}

	//�T�E���h�{�C�X�f�[�^�o�^
	void LoadAudio(int id, const char name[], int type)
	{
		//�}�X�^�[�{�C�X�G���[�`�F�b�N
		if (g_is_error_sound == true)
			return;
		//�v�f�I�[�o�[�`�F�b�N
		if (id < 0 || id>SOUND_MAX)
			return;

		//�C���^�[�t�F�[�X�j��
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

			//�T�E���h�f�[�^�j��
			if (g_oSudioData[id].m_pWave != nullptr)
				delete[]g_oSudioData[id].m_pWave;
		}

		//�T�E���h�^�C�v�Z�b�g
		g_oSudioData[id].m_iType = type;

		//wave�t�@�C���̃��������ɓW�J
		ifstream ifs(name, ios::in | ios::binary);
		if (!ifs)	return;

		unsigned int Size = 0;
		//�W�J
		ifs.seekg(0, ios::end);
		Size = static_cast<unsigned>(ifs.tellg());
		ifs.seekg(0, ios::beg);
		g_oSudioData[id].m_pWave = new unsigned char[Size];

		ifs.read(reinterpret_cast<char*>(g_oSudioData[id].m_pWave), Size);
		ifs.close();

		//RIFF�t�@�C�����
		//wave���
		WAVEFORMATEX WaveformatEx = { 0 };
		FindChunk(g_oSudioData[id].m_pWave, "fmt", &g_oSudioData[id].m_iSize, &g_oSudioData[id].m_pData);

		//wave��񕪃A�h���X�����炵�Ĕg��f�[�^�Ɉړ�
		unsigned char* p = g_oSudioData[id].m_pData;
		WaveformatEx.wFormatTag		= GetWord(p);	p += sizeof(WORD);
		WaveformatEx.nChannels		= GetWord(p);	p += sizeof(WORD);
		WaveformatEx.nSamplesPerSec = GetDword(p);	p += sizeof(DWORD);
		WaveformatEx.nAvgBytesPerSec= GetDword(p);	p += sizeof(DWORD);
		WaveformatEx.nBlockAlign	= GetWord(p);	p += sizeof(WORD);
		WaveformatEx.wBitsPerSample = GetWord(p);	p += sizeof(WORD);
		WaveformatEx.cbSize			= GetWord(p);	p += sizeof(WORD);

		//�g�{�̂̃f�[�^
		FindChunk(g_oSudioData[id].m_pData, "data", &g_oSudioData[id].m_iSize, &g_oSudioData[id].m_pData);

		//�Đ��̂��߂̃C���^�[�t�F�[�X����
		for (int i = 0; i < SOUND_SE_MAX; i++)
		{
			//�T�u�~�N�X�{�C�X���Z�b�g
			XAUDIO2_SEND_DESCRIPTOR data;
			data.Flags = 0;
			data.pOutputVoice = g_oSudioData[id].m_pSFXSubmixVoice;
			XAUDIO2_VOICE_SENDS SFXSendList;
			memset(&SFXSendList, 0x00, sizeof(XAUDIO2_VOICE_SENDS));
			SFXSendList.SendCount = 1;
			SFXSendList.pSends = &data;
			//�\�[�X�{�C�X�쐬
			g_pXAudio2->CreateSourceVoice(&g_oSudioData[id].m_ppSourceVoice[i],
				&WaveformatEx, 0U, 2.0F, 0, &SFXSendList, 0);
		}
		//�T�E���h�o�b�t�@���\�[�X�{�C�X�L���[�ɍ쐬
		memset(&g_oSudioData[id].m_cSoundBuffer, 0x00, sizeof(g_oSudioData[id].m_cSoundBuffer));
		g_oSudioData[id].m_cSoundBuffer.AudioBytes = g_oSudioData[id].m_iSize;
		g_oSudioData[id].m_cSoundBuffer.pAudioData = reinterpret_cast<BYTE*>(g_oSudioData[id].m_pData);
		g_oSudioData[id].m_cSoundBuffer.Flags = XAUDIO2_END_OF_STREAM;

		//���[�v�ݒ�
		if (type == TYPE_EFFECT)
			g_oSudioData[id].m_cSoundBuffer.LoopCount = XAUDIO2_NO_LOOP_REGION;
		else
			g_oSudioData[id].m_cSoundBuffer.LoopCount = XAUDIO2_LOOP_INFINITE;

		//�{�����[���ݒ�
		g_oSudioData[id].m_fVolume = 0.5f;
		g_oSudioData[id].m_pSFXSubmixVoice->SetVolume(g_oSudioData[id].m_fVolume, 0);
	}

	//�^�Ƀf�[�^�����킹�ē����
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

	//���Ȃ炸�`�����N�͌�����Ƃ��Ċȗ���
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