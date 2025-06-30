#pragma once
#include <tchar.h>
#include <memory>
#include <wrl/client.h>	//COMのスマート化
#include <string>
#include <locale.h>
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

//-------------------------------------------------------
//構造バッファ
class StructBuffer
{
public:
	StructBuffer() {}
	~StructBuffer()
	{
		m_resource.Reset();
	}
	void CreateResource(ComPtr<ID3D12Device>& dev, size_t size);	//バッファリソース作成
	void UpDate(size_t size, void* data);							//バッファへデータアップロード
	ComPtr<ID3D12Resource>& Getbuf() { return m_resource; }
protected:
	ComPtr<ID3D12Resource> m_resource;							//リソース
};
//-------------------------------------------------------
//-------------------------------------------------------
//描画（テクスチャ）リソースバッファ
class ResourceBuffer
{
public:
	ResourceBuffer() {}
	~ResourceBuffer()
	{
		m_resource.Reset();
		m_descriptorHeap.Reset();
	}
	ComPtr<ID3D12Resource>& GetResource() { return m_resource; }
	D3D12_CPU_DESCRIPTOR_HANDLE& GetHandle() { return m_handle; }
	ComPtr<ID3D12DescriptorHeap>& GetDescriptorHeap() { return m_descriptorHeap; };

protected:
	ComPtr<ID3D12Resource>				m_resource{ nullptr };			//リソース
	ComPtr<ID3D12DescriptorHeap>		m_descriptorHeap{ nullptr };	//デスクリプタヒープ
	D3D12_CPU_DESCRIPTOR_HANDLE			m_handle{ 0 };					//ハンドル（ビュー）
};
//--------------------------------------------------------
//レンダリングテクスチャバッファ--------------------------
class RenderTexture :public ResourceBuffer
{
public:
	RenderTexture(ComPtr<ID3D12Device>& dev, int w, int h);
	~RenderTexture()
	{
		m_taget_descriptorHeap.Reset();
	}
	ComPtr<ID3D12DescriptorHeap>& GetTagetDescriptorHeap() { return m_taget_descriptorHeap; }
	D3D12_CPU_DESCRIPTOR_HANDLE& GetTagetHandle() { return m_taget_handle; }
private:
	//ターゲット用
	ComPtr<ID3D12DescriptorHeap>		m_taget_descriptorHeap{ nullptr };	//デスクリプタヒープ
	D3D12_CPU_DESCRIPTOR_HANDLE			m_taget_handle{ 0 };				//ハンドル
};
//--------------------------------------------------------
//Zバッファ
class ZBuffer :public ResourceBuffer
{
public:
	ZBuffer(ComPtr<ID3D12Device>& dev, int w, int h);
	~ZBuffer() {};
private:
};
//--------------------------------------------------------
//テクスチャバッファ
//イメージロード用GDI+
namespace GDI_OBJ
{
	constexpr int MAX_SIZE{ 512 };//イメージ読み込み最大の大きさ
	void Initialize();
	void Release();
}

//テクスチャバッファ
class Texture :public ResourceBuffer
{
public:
	Texture() {};
	~Texture() {};
	void LoadTexImage(ComPtr<ID3D12Device>& dev, string tex_name);
	void LoadTexImage(ComPtr<ID3D12Device>& dev, float const_img[512][512][4]);
protected:
	//出力用テクスチャリソースビュー作成
	bool CreateOutPutTexResourceView(ComPtr<ID3D12Device>& dev, D3D12_PLACED_SUBRESOURCE_FOOTPRINT& footprint, UINT64& total_bytes);
	//アップロードテクスチャリソース作成
	bool CreateUpLoadResource(ComPtr<ID3D12Device>& dev, ID3D12Resource** resource_up_load, UINT64 total_bytes);
	//アップロード情報を出力テクスチャにコピー 
	bool CopyImageDataTexture(ComPtr<ID3D12Device>& dev,D3D12_PLACED_SUBRESOURCE_FOOTPRINT& footprint, ID3D12Resource* resource_up_load);
};
//--------------------------------------------------------
//フォントバッファ
class FontTex :public Texture
{
public:
	FontTex()
	{
		// フォント情報
		HFONT hFont = CreateFont(
			FONT_SIZE,0, 0, 0, 0, FALSE, FALSE, FALSE,
			SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
			FIXED_PITCH | FF_MODERN,
			L"ＭＳ ゴシック"	//使用フォント
		);
		m_hdc = GetDC(NULL);
		m_oldFont = (HFONT)SelectObject(m_hdc, hFont);
		GetTextMetrics(m_hdc, &m_TM);
		m_hFont = hFont;
		return;
	
	};
	~FontTex()
	{
		//フォントオブジェクトの開放
		DeleteObject(m_oldFont);
		DeleteObject(m_hFont);
		ReleaseDC(NULL, m_hdc);
	};
	void LoadFontImage(ComPtr<ID3D12Device>& dev, wstring str[15]);
	
private:
	void LoadCharImage( wstring c, UINT32* p, int pos_x, int pos_y);
	//フォント情報
	HFONT		m_hFont;
	HDC			m_hdc;
	HFONT		m_oldFont;
	TEXTMETRIC	m_TM;
	//フォントテクスチャサイズ
	const int  TEX_SIZE{ 512 };
	const int  FONT_SIZE{ 31 };
	const int  FONT_SHIFT{ 4 };
	const MAT2 MAT{ {0,1},{0,0},{0,0},{0,1} };
	const int  CHAR_STEP{ 1 };
};
//--------------------------------------------------------
//テクスチャ配列化ヘルパークラス
class TextureArray
{
public:
	TextureArray(ComPtr<ID3D12Device>& dev,int texmax)
	{
		if (texmax > SET_TEX_MAX) return;
		//ディスクリプタヒープを作成
		D3D12_DESCRIPTOR_HEAP_DESC	descriptor_heap_desc{};
		ZeroMemory(&descriptor_heap_desc, sizeof(descriptor_heap_desc));
		descriptor_heap_desc.NumDescriptors = texmax;
		descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;		
		if (FAILED(dev->CreateDescriptorHeap(&descriptor_heap_desc,
			IID_PPV_ARGS(m_descriptorHeap.ReleaseAndGetAddressOf()))))
			return;
		//配列の要素のサイズ
		strideHandleBytes = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	};

	~TextureArray()
	{
		m_descriptorHeap.Reset();
	}

	//テクスチャを配列に登録
	void SetTexture(ComPtr<ID3D12Device>& dev, ResourceBuffer* b,int set_id)
	{
		if (set_id >= SET_TEX_MAX) return;
		//リソースビューディスク
		D3D12_SHADER_RESOURCE_VIEW_DESC	shader_resource_view_desc{};
		shader_resource_view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		shader_resource_view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		shader_resource_view_desc.Texture2D.MipLevels = 1;
		shader_resource_view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//リソースをディスクリプターヒープの各要素に設定
		D3D12_CPU_DESCRIPTOR_HANDLE	handle;
		handle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += strideHandleBytes*set_id;
		dev->CreateShaderResourceView(b->GetResource().Get(), &shader_resource_view_desc, handle);
	}

	//SetDescriptorHeaps用
	ComPtr<ID3D12DescriptorHeap>& GetD()
	{
		return m_descriptorHeap;
	}
	//SetGraphicsRootDescriptorTable用
	D3D12_GPU_DESCRIPTOR_HANDLE GetH(int id)
	{
		if (id >= SET_TEX_MAX) id =0;
		D3D12_GPU_DESCRIPTOR_HANDLE	h;
		h=m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
		h.ptr += strideHandleBytes*id;
		return h;
	}

	ComPtr<ID3D12DescriptorHeap> m_descriptorHeap{ nullptr };	//ディスクリプタヒープ
	UINT strideHandleBytes;
	const int SET_TEX_MAX{ 8 };//テクスチャ設定最大数
};