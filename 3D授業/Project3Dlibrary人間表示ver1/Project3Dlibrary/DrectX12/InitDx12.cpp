#include "DX12Library.h"

//LIBの登録
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")


//コンストラクタ
DeviceDX12::DeviceDX12(HWND hWnd)
{
	ID3D12Debug* debug = nullptr;
	UINT flagsDXGI = 0;

	//デバック時 DirectX12のデバッグレイヤーを有効にする
#ifdef _DEBUG
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	if (debug)
	{
		debug->EnableDebugLayer();
		debug->Release();
		debug = nullptr;
	}
	flagsDXGI |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	// DirectX12がサポートする利用可能なハードウェアアダプタ取得
	if (FAILED(CreateDXGIFactory2(flagsDXGI, IID_PPV_ARGS(&m_factory))))
		return ;

	//factoryのハードウェアアダプタがDirectX12に対応しているかを確認
	IDXGIAdapter1* adapter{ nullptr };
	for (UINT i = 0; m_factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			continue;

		// アダプタがDirectX12に対応しているか確認確認のためデバイスを作成
		if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			break;
	}
	m_hardwareAdapter = adapter;

	//デバイスの作成
	if (FAILED(D3D12CreateDevice(m_hardwareAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device))))
		return ;

	//グラフィックボードサポートチェック
	D3D12_FEATURE_DATA_D3D12_OPTIONS options{ 0 };
	if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, (void*)&options, sizeof(options))))
		return ;

	// フルスクリーンのサポートなし
	if (FAILED(m_factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER)))
		return ;


	return ;
}

//デストラクタ
DeviceDX12::~DeviceDX12()
{
	m_factory->Release();
	m_hardwareAdapter->Release();
	m_device.Reset();
}
