#pragma once
#include <tchar.h>
#include <memory>
#include <wrl/client.h>	//COMのスマート化
#include <string>
//WindowsとDirectXのインクルード
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>



//ネームスペースショートカット
using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;

#include "../BufferAndImg/Buffer.h"

//------------------------------------------------------
//デバイスDirectX12作成クラス
class DeviceDX12
{
public:
	DeviceDX12(HWND hWnd);
	~DeviceDX12();

	ComPtr<ID3D12Device>& GetDev() { return m_device; }
	IDXGIFactory4& GetFac() { return *m_factory; }

private:
	ComPtr<ID3D12Device>m_device{ nullptr };			//デバイス
	IDXGIFactory4*		m_factory{ nullptr };			//ファクトリー
	IDXGIAdapter1*		m_hardwareAdapter{ nullptr };	//ハードウェアアダプター
};

using  DX12=DeviceDX12;
//------------------------------------------------------

//-------------------------------------------------------
//描画コマンドクラス
class Command
{
public:
	Command(ComPtr<ID3D12Device>& dev, D3D12_COMMAND_LIST_TYPE type);
	~Command();

	ComPtr<ID3D12GraphicsCommandList>&	GetList()		{ return m_commandList; }
	ComPtr<ID3D12CommandAllocator>&		GetAlr()		{ return m_commandAllocator; }
	ComPtr<ID3D12CommandQueue>&			GetQueue()		{ return m_commandQueue; }
	ComPtr<ID3D12Fence>&				GetFence()		{ return m_fence; }
	HANDLE&								GetFenceEvent() { return m_fenceEvent; }

private:
	ComPtr<ID3D12GraphicsCommandList>	m_commandList		{ nullptr };	//コマンドリスト
	ComPtr<ID3D12CommandAllocator>		m_commandAllocator	{ nullptr };	//コマンドアローケータ
	ComPtr<ID3D12CommandQueue>			m_commandQueue		{ nullptr };	//コマンドキュー

	ComPtr<ID3D12Fence>				m_fence{ nullptr };						//フェンス
	HANDLE							m_fenceEvent{ 0 };						//フェンスイベント
};

using  COMMAND = Command;
//-------------------------------------------------------
//------------------------------------------------------
//定数
constexpr int FRAME_COUNT{ 2 };
//レンダリングビュー作成クラス
class RenderBuffer
{
public:
	RenderBuffer(ComPtr<ID3D12Device>& dev, ComPtr<ID3D12CommandQueue>& queue,
		IDXGIFactory4& fac,HWND hWnd,int w, int h);
	~RenderBuffer();

	ComPtr<IDXGISwapChain3>& GetSwapChain()				{ return m_swapChain; }
	ComPtr<ID3D12Resource>& GetRenderTaget(int id)		{ return m_renderTargets[id]; }
	D3D12_CPU_DESCRIPTOR_HANDLE& GetRenderView(int id)  { return m_handleRTV[id]; }
	D3D12_CPU_DESCRIPTOR_HANDLE& GetDepthView()			{ return m_z_buffer->GetHandle(); }
	UINT& GetFrameIndex()								{ return m_frameIndex; }

private:
	//スワップチェイン
	ComPtr<IDXGISwapChain3>		m_swapChain{ nullptr };				//スワップチェイン
	UINT						m_frameIndex{ 0 };					//フレームインデックス
	//レンダリングリソースとビュー
	ComPtr<ID3D12Resource>		m_renderTargets[FRAME_COUNT]{ nullptr };//リソース			（GPU上のリソースバッファ）
	ComPtr<ID3D12DescriptorHeap>m_descriptorHeapRTV{ nullptr };			//ディスプリタヒープ（GPU上のリソースアクセス）
	D3D12_CPU_DESCRIPTOR_HANDLE	m_handleRTV[FRAME_COUNT]{0, 0};			//ハンドル（ビュー）（CPUからデスクリプタにアクセス）
	//Zバッファリソースとビュー
	unique_ptr<ZBuffer>				m_z_buffer;

	// デスクリプタとは、GPUメモリ上に存在する様々なデータやバッファの種類や位置、大きさを示す構造体なモノ
	// デスクリプタが持つポインタでGPU上のバッファデータをアドレス格納してる
	// デスクリプタヒープとは、その構造体のGPU上の配列である。
};
using RENDER = RenderBuffer;
//-------------------------------------------------------

