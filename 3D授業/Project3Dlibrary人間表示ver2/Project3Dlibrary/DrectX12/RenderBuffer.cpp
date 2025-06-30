#include "DX12Library.h"

//�R���X�g���N�^
RenderBuffer::RenderBuffer(ComPtr<ID3D12Device>& dev, ComPtr<ID3D12CommandQueue>& queue,IDXGIFactory4& fac, HWND hWnd, int w, int h)
{
	//�v���C�}���o�b�t�@�ƃZ�J���_���o�b�t�@�쐬�i�X���b�v�`�F�C���j--------------------------------------
	// �X���b�v�`�F�C���X�e�[�^�X
	DXGI_SWAP_CHAIN_DESC1	swapChainDesc{};
	swapChainDesc.BufferCount = FRAME_COUNT;
	swapChainDesc.Width  = w;
	swapChainDesc.Height = h;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	// �X���b�v�`�F�C���쐬
	ComPtr<IDXGISwapChain1>	swapChain;
	if (FAILED(fac.CreateSwapChainForHwnd(queue.Get(), hWnd,&swapChainDesc, nullptr, nullptr, &swapChain)))
		return;
	// �����o�̃X���b�v�`�F�C���ɓn��
	if (FAILED(swapChain.As(&m_swapChain)))
		return;

	// �X���b�v�`�F�C���̃o�b�N�o�b�t�@�̃A�h���X�ʒu
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
	// �f�X�N���v�^�q�[�v�Ƀv���C�}���o�b�t�@�ƃo�b�N�o�b�t�@�p�̃f�X�N���v�^�쐬
	D3D12_DESCRIPTOR_HEAP_DESC descHeap;
	ZeroMemory(&descHeap, sizeof(descHeap));
	descHeap.NumDescriptors = FRAME_COUNT;
	descHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	if (FAILED(dev->CreateDescriptorHeap(&descHeap, IID_PPV_ARGS(m_descriptorHeapRTV.GetAddressOf()))))
		return;

	//�f�X�N���v�^�q�[�v�̗v�f�ԍ��̃T�C�Y
	UINT strideHandleBytes = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//�X���b�v�`�F�C���Ƀ����_�[�^�[�Q�b�g�r���[�̍쐬.
	for (UINT i = 0; i < FRAME_COUNT; i++)
	{
		//�X���b�v�`�F�C���̊e�o�b�t�@���擾
		if (FAILED(m_swapChain->GetBuffer(i, IID_PPV_ARGS(m_renderTargets[i].GetAddressOf()))))
			return;
		//�f�X�N���v�^�q�[�v��CPU��ň����A�h���X���e�n���h���ɓn��
		//�i���̒i�K�ł̓��\�[�X�A�N�Z�X�ł��Ȃ��j
		m_handleRTV[i] = m_descriptorHeapRTV->GetCPUDescriptorHandleForHeapStart();
		m_handleRTV[i].ptr += i * strideHandleBytes;
		//�n���h�����f�X�N���v�^���w���A�v���C�}���ƃo�b�N�o�b�t�@�ɃA�N�Z�X�ł���悤�ɂ���
		//�i�r���[���쐬���邱�ƂŃA�N�Z�X�\�ɂȂ�j
		dev->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, m_handleRTV[i]);
	}

	//Z�o�b�t�@�쐬-------------------------------------------------------------------------------------
	m_z_buffer.reset(new ZBuffer(dev, w, h));
}

//�f�X�g���N�^
RenderBuffer::~RenderBuffer()
{
	m_swapChain.Reset();
	for (auto t : m_renderTargets)
		t.Reset();
	m_descriptorHeapRTV.Reset();
	
	m_z_buffer.reset();
}