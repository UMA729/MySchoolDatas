#include "DX12Library.h"


//コンストラクタ
Command::Command(ComPtr<ID3D12Device> &dev, D3D12_COMMAND_LIST_TYPE type)
{
	// コマンドアロケータ作成
	if (FAILED(dev->CreateCommandAllocator(type,
		IID_PPV_ARGS(m_commandAllocator.GetAddressOf()))))
		return;
	// コマンドリスト作成
	if (FAILED(dev->CreateCommandList(0, type,
		m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList))))
		return;

	// コマンドキューを生成.
	D3D12_COMMAND_QUEUE_DESC descCommandQueue{ };
	ZeroMemory(&descCommandQueue, sizeof(descCommandQueue));
	descCommandQueue.Type = type;
	descCommandQueue.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	if (FAILED(dev->CreateCommandQueue(&descCommandQueue, IID_PPV_ARGS(m_commandQueue.GetAddressOf()))))
		return;

	// コマンドキュー内容がGPU上処理中なのかを知らせるフェンスを作成
	m_fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (FAILED(dev->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.GetAddressOf()))))
		return;

	//コマンド一度締める
	m_commandList->Close();
}

//デストラクタ
Command::~Command()
{
	m_commandList.Reset();
	m_commandAllocator.Reset();
	m_commandQueue.Reset();
	m_fence.Reset();
}