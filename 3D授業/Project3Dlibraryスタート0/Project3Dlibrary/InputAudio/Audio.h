#pragma once
//C++STL�֘A�̃C���N���[�h
#include <iostream>
#include <tchar.h>
#include <memory>
#include <wrl/client.h>	//COM�̃X�}�[�g��

//�l�[���X�y�[�X�V���[�g�J�b�g
using namespace std;
using namespace Microsoft::WRL;


//-------------------------------------------------------
//�I�[�f�B�I
namespace AUD
{
	//�T�E���h�^�C�v�萔	
	constexpr int TYPE_EFFECT{ 0 };	//���ʉ�(�P�����j
	constexpr int TYPE_BACK_MUSIC{ 1 };	//�o�b�N�~���[�W�b�N (���[�v)

	//�C���^�[�t�F�[�X										
	float Volume(int id, float t);					//�e�{�����[���ύX
	float VolumeMaster(float t);					//�}�X�^�[�{�����[���ύX
	void Stop(int id);								//�X�g�b�v
	void Start(int id);								//�X�^�[�g
	void DeleteSudio();								//�T�E���h���j��
	void LoadAudio(int id, const char name[], int type);	//�T�E���h�{�C�X�f�[�^�o�^

	//������				 
	void Initialize();
	//���
	void Release();
}
//-------------------------------------------------------