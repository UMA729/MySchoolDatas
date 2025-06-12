#include "DX12Library.h"


//�R���X�g���N�^
Command::Command(ComPtr<ID3D12Device> &dev, D3D12_COMMAND_LIST_TYPE type)
{
	// �R�}���h�A���P�[�^�쐬
	if (FAILED(dev->CreateCommandAllocator(type,
		IID_PPV_ARGS(m_commandAllocator.GetAddressOf()))))
		return;
	// �R�}���h���X�g�쐬
	if (FAILED(dev->CreateCommandList(0, type,
		m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList))))
		return;

	// �R�}���h�L���[�𐶐�.
	D3D12_COMMAND_QUEUE_DESC descCommandQueue{ };
	ZeroMemory(&descCommandQueue, sizeof(descCommandQueue));
	descCommandQueue.Type = type;
	descCommandQueue.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	if (FAILED(dev->CreateCommandQueue(&descCommandQueue, IID_PPV_ARGS(m_commandQueue.GetAddressOf()))))
		return;

	// �R�}���h�L���[���e��GPU�㏈�����Ȃ̂���m�点��t�F���X���쐬
	m_fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (FAILED(dev->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.GetAddressOf()))))
		return;

	//�R�}���h��x���߂�
	m_commandList->Close();
}

//�f�X�g���N�^
Command::~Command()
{
	m_commandList.Reset();
	m_commandAllocator.Reset();
	m_commandQueue.Reset();
	m_fence.Reset();
}