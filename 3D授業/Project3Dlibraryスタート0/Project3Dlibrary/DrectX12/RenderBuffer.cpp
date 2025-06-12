#include "DX12Library.h"

//コンストラクタ
RenderBuffer::RenderBuffer(ComPtr<ID3D12Device>& dev, ComPtr<ID3D12CommandQueue>& queue,IDXGIFactory4& fac, HWND hWnd, int w, int h)
{
	//プライマリバッファとセカンダリバッファ作成（スワップチェイン）--------------------------------------
	// スワップチェインステータス
	DXGI_SWAP_CHAIN_DESC1	swapChainDesc{};
	swapChainDesc.BufferCount = FRAME_COUNT;
	swapChainDesc.Width  = w;
	swapChainDesc.Height = h;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	// スワップチェイン作成
	ComPtr<IDXGISwapChain1>	swapChain;
	if (FAILED(fac.CreateSwapChainForHwnd(queue.Get(), hWnd,&swapChainDesc, nullptr, nullptr, &swapChain)))
		return;
	// メンバのスワップチェインに渡す
	if (FAILED(swapChain.As(&m_swapChain)))
		return;

	// スワップチェインのバックバッファのアドレス位置
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
	// デスクリプタヒープにプライマリバッファとバックバッファ用のデスクリプタ作成
	D3D12_DESCRIPTOR_HEAP_DESC descHeap;
	ZeroMemory(&descHeap, sizeof(descHeap));
	descHeap.NumDescriptors = FRAME_COUNT;
	descHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	if (FAILED(dev->CreateDescriptorHeap(&descHeap, IID_PPV_ARGS(m_descriptorHeapRTV.GetAddressOf()))))
		return;

	//デスクリプタヒープの要素番号のサイズ
	UINT strideHandleBytes = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//スワップチェインにレンダーターゲットビューの作成.
	for (UINT i = 0; i < FRAME_COUNT; i++)
	{
		//スワップチェインの各バッファを取得
		if (FAILED(m_swapChain->GetBuffer(i, IID_PPV_ARGS(m_renderTargets[i].GetAddressOf()))))
			return;
		//デスクリプタヒープをCPU上で扱うアドレスを各ハンドルに渡す
		//（この段階ではリソースアクセスできない）
		m_handleRTV[i] = m_descriptorHeapRTV->GetCPUDescriptorHandleForHeapStart();
		m_handleRTV[i].ptr += i * strideHandleBytes;
		//ハンドルがデスクリプタを指し、プライマリとバックバッファにアクセスできるようにする
		//（ビューを作成することでアクセス可能になる）
		dev->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, m_handleRTV[i]);
	}

	//Zバッファ作成-------------------------------------------------------------------------------------
	m_z_buffer.reset(new ZBuffer(dev, w, h));
}

//デストラクタ
RenderBuffer::~RenderBuffer()
{
	m_swapChain.Reset();
	for (auto t : m_renderTargets)
		t.Reset();
	m_descriptorHeapRTV.Reset();
	
	m_z_buffer.reset();
}