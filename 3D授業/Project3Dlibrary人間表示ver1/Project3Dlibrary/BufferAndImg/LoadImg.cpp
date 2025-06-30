#include <windows.h>
#include <gdiplus.h>	//GDI+を使用
#include <string>
#include <codecvt>
#include <vector>
#include<array>

#include "../DrectX12/DX12Library.h"
#include "Buffer.h"

#pragma comment(lib, "gdiplus.lib")

//------------------------------イメージ---------------------------------//
namespace GDI_OBJ
{
	//GDI+オブジェクト
	ULONG_PTR g_GDIplusToken{ NULL };

	//GDI+の初期化
	void Initialize()
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		if (Gdiplus::GdiplusStartup(&g_GDIplusToken, &gdiplusStartupInput, NULL) != Gdiplus::Ok)
			return;
	}

	//GDI+の解放
	void Release()
	{
		Gdiplus::GdiplusShutdown(g_GDIplusToken);
	}
}


void Texture::LoadTexImage(ComPtr<ID3D12Device>& dev, float const_img[512][512][4])
{
	//イメージロード
	vector<UINT32>p(512 * 512);
	
	//空テクスチャを作成----------------------------------
	//アップロード用
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT  footprint{ 0 };//配置先サブリソースのフットプリント
	UINT64	  m_total_bytes{ 0 };
	ID3D12Resource* resource_up_load{ nullptr };//アップロード用テクスチャリソース
	//アップロードと出力テクスチャ作成
	CreateOutPutTexResourceView(dev, footprint, m_total_bytes);
	CreateUpLoadResource(dev, &resource_up_load, m_total_bytes);

	for (int y = 0; y < 512; y++)
	{
		for (int x = 0; x < 512; x++)
		{
			UINT64 rgb[4];
			rgb[0] = const_img[y][x][0];
			rgb[1] = const_img[y][x][1];
			rgb[2] = const_img[y][x][2];
			rgb[3] = const_img[y][x][3];
				
			//512×512テクスチャにデータを入れる
			UINT64 pos = y * 512 + x;
			p[pos] = (rgb[0]) | (rgb[1] << 8) | (rgb[2] << 16) | (rgb[3] << 24);
		}
	}

	//アップロード用のテクスチャリソースに画素データを入れる--------------------
	void* ptr = nullptr;
	resource_up_load->Map(0, nullptr, &ptr);

	//アップロード用テクスチャリソースのイメージ部の先頭アドレス取得
	UINT32* p_rgba = (reinterpret_cast<UINT32*>(ptr) + footprint.Offset);

	for (int i = 0; i < 512; i++)
		for (int j = 0; j < 512; j++)
			p_rgba[i * 512 + j] = p[i * 512 + j];
	
	resource_up_load->Unmap(0, 0);

	CopyImageDataTexture(dev, footprint, resource_up_load);
	
	resource_up_load->Release();
}


//イメージ情報の読み込み(画像ファイルを開く：対応画像形式 BMP, JPEG, PNG, GIF, TIFF, WMF, EMF)
void Texture::LoadTexImage(ComPtr<ID3D12Device>& dev, string tex_name)
{
	//読み込みイメージの幅
	UINT64 w{ 0 }, h{ 0 };
	//ファイル名から画像データ取得
	wchar_t wc[128]{ L"" };
	int ret{ 0 };
	//一次イメージバッファ
	vector<vector<array<float, 4>>> img;
	//イメージロード
	vector<UINT32>p(512 * 512);
	
	//空テクスチャを作成----------------------------------
	//アップロード用
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT  footprint{ 0 };//配置先サブリソースのフットプリント
	UINT64	  m_total_bytes{ 0 };
	ID3D12Resource* resource_up_load{ nullptr };//アップロード用テクスチャリソース
	//アップロードと出力テクスチャ作成
	CreateOutPutTexResourceView(dev, footprint, m_total_bytes);
	CreateUpLoadResource(dev, &resource_up_load, m_total_bytes);

	//空テクスチャに画像データをロード--------------------
	//文字列とワイド文字列変換コンバート
	ret = MultiByteToWideChar(CP_ACP, 0,tex_name.c_str(),tex_name.length() + 1,wc, 128);

	//Gdiplusで画像ファイルの画素を取得
	Gdiplus::Bitmap* pBitmap = Gdiplus::Bitmap::FromFile(wc);
	if (pBitmap->GetLastStatus() == Gdiplus::Ok)
	{
		h = pBitmap->GetWidth();
		w = pBitmap->GetHeight();

		//一次イメージバッファ作成
		img.resize(h);
		for (int i = 0; i < h; i++)
			img[i].resize(w);

		//グラフィックをデータから1ピクセルずつ描画していく
		Gdiplus::Color  srcColor;
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				pBitmap->GetPixel(i, j, &srcColor);
				img[i][j][0] = srcColor.GetR();
				img[i][j][1] = srcColor.GetG();
				img[i][j][2] = srcColor.GetB();
				img[i][j][3] = srcColor.GetA();
			}
		}

		//線形補間による拡大縮小
		for (int y = 0; y < 512; y++)
		{
			for (int x = 0; x < 512; x++)
			{
				UINT64 rgb_w[4];
				for (int c = 0; c < 4; c++)
				{
					// 拡大前画像における座標計算
					float original_x = (double)x / (512.0f / w);
					float original_y = (double)y / (512.0f / h);

					// 周囲４点の座標を計算
					int	x0 = (int)original_x;	int x1 = x0 + 1;
					int y0 = (int)original_y;	int y1 = y0 + 1;

					// m1やn1が画像をはみ出た場合の処理
					if (x1 >= 512) x1 = 512 - 1;
					if (y1 >= 512) y1 = 512 - 1;

					// 元々の座標とm0,n0との距離を計算
					int dx = original_x - x0;
					int dy = original_y - y0;

					// 周囲４点の画素の色cの輝度値より線形補間
					rgb_w[c]= img[y1][x1][c] * dy * dx
							+ img[y1][x0][c] * dy * (1.0f - dx)
							+ img[y0][x1][c] * (1.0f - dy) * dx
							+ img[y0][x0][c] * (1.0f - dy) * (1.0f - dx);
				}

				//512×512テクスチャにデータを入れる
				UINT64 pos = x * 512 + y;
				p[pos] = (rgb_w[0]) | (rgb_w[1] << 8) | (rgb_w[2] << 16) | (rgb_w[3] << 24);
			}
		}

		delete pBitmap;
	}
	else
	{
		//テクスチャが無いときは0xFFで埋める
		w = 512;
		h = 512;
		memset(&p[0], 0xFF, sizeof(UINT32) * w * h);
	}
	
	//アップロード用のテクスチャリソースに画素データを入れる--------------------
	void* ptr = nullptr;
	resource_up_load->Map(0, nullptr, &ptr);

	//アップロード用テクスチャリソースのイメージ部の先頭アドレス取得
	UINT32* p_rgba = (reinterpret_cast<UINT32*>(ptr) + footprint.Offset);

	//GID+から獲得したグラフィック情報をアップロード用テクスチャに入れる
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			p_rgba[i * 512 + j] = p[i * w + j];
		}
	}
	resource_up_load->Unmap(0, 0);
	CopyImageDataTexture(dev, footprint, resource_up_load);

	resource_up_load->Release();
}


//出力用テクスチャリソースビューを作成
bool Texture::CreateOutPutTexResourceView(ComPtr<ID3D12Device>& dev,
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT& footprint, UINT64& total_bytes)
{
	//テクスチャのリソースビューを作成
	//ヒーププロパティ
	D3D12_HEAP_PROPERTIES heap_props;
	ZeroMemory(&heap_props, sizeof(heap_props));
	heap_props.Type = D3D12_HEAP_TYPE_DEFAULT;
	heap_props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap_props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heap_props.CreationNodeMask = 1;
	heap_props.VisibleNodeMask = 1;
	//リソースデスク
	D3D12_RESOURCE_DESC   resource_desc;
	ZeroMemory(&resource_desc, sizeof(resource_desc));
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resource_desc.Width = 512;
	resource_desc.Height = 512;
	resource_desc.DepthOrArraySize = 1;
	resource_desc.MipLevels = 1;
	resource_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resource_desc.SampleDesc.Count = 1;
	resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	// リソースを生成.※D3D12_RESOURCE_STATE_COPY_DEST(書き込み用リソース)
	if (FAILED(dev->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &resource_desc,
		D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(m_resource.GetAddressOf()))))
		return false;

	//リソースの設定取得
	dev->GetCopyableFootprints(&resource_desc, 0, 1, 0, &footprint, nullptr, nullptr, &total_bytes);

	//デスクリプタヒープデスク				
	D3D12_DESCRIPTOR_HEAP_DESC	descriptor_heap_desc{};
	descriptor_heap_desc.NumDescriptors = 1;
	descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descriptor_heap_desc.NodeMask = 0;

	// テクスチャー用のデスクリプタヒープを作成		
	if (FAILED(dev->CreateDescriptorHeap(&descriptor_heap_desc, IID_PPV_ARGS(m_descriptorHeap.GetAddressOf()))))
		return false;

	//テクスチャーシェーダーリソースビューデスク
	D3D12_SHADER_RESOURCE_VIEW_DESC	shader_resource_view_desc{};
	shader_resource_view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	shader_resource_view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	shader_resource_view_desc.Texture2D.MipLevels = 1;
	shader_resource_view_desc.Texture2D.MostDetailedMip = 0;
	shader_resource_view_desc.Texture2D.PlaneSlice = 0;
	shader_resource_view_desc.Texture2D.ResourceMinLODClamp = 0.0F;
	shader_resource_view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	//テクスチャーシェーダーリソースビューを作成	
	m_handle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	dev->CreateShaderResourceView(m_resource.Get(), &shader_resource_view_desc, m_handle);

	return true;
}

//アップロードテクスチャリソース作成
bool Texture::CreateUpLoadResource(ComPtr<ID3D12Device>& dev, ID3D12Resource** resource_up_load, UINT64 total_bytes)
{
	//イメージ情報MAP用リソース作成
	//ヒーププロパティ
	D3D12_HEAP_PROPERTIES heap_props;
	ZeroMemory(&heap_props, sizeof(heap_props));
	heap_props.Type = D3D12_HEAP_TYPE_UPLOAD;
	heap_props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap_props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heap_props.CreationNodeMask = 1;
	heap_props.VisibleNodeMask = 1;

	//リソースデスク
	D3D12_RESOURCE_DESC desc_resurce;
	ZeroMemory(&desc_resurce, sizeof(desc_resurce));
	desc_resurce.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc_resurce.Width = total_bytes;
	desc_resurce.Height = 1;
	desc_resurce.DepthOrArraySize = 1;
	desc_resurce.MipLevels = 1;
	desc_resurce.Format = DXGI_FORMAT_UNKNOWN;
	desc_resurce.SampleDesc.Count = 1;
	desc_resurce.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//イメージ情報のMAP用リソース作成※D3D12_RESOURCE_STATE_GENERIC_READ(読み取り用リソース)
	dev->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE,
		&desc_resurce, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(resource_up_load));
	return true;
}

//イメージをテクスチャに転送
bool Texture::CopyImageDataTexture(ComPtr<ID3D12Device>& dev,D3D12_PLACED_SUBRESOURCE_FOOTPRINT& footprint, ID3D12Resource* resource_up_load)
{
	//テクスチャコピー用コマンド作成
	unique_ptr<COMMAND> copy_com;
	copy_com.reset(new COMMAND(dev, D3D12_COMMAND_LIST_TYPE_COPY));

	//テクスチャコピーコマンド情報破棄
	if (FAILED(copy_com->GetAlr()->Reset()))
		return false;
	if (FAILED(copy_com->GetList()->Reset(copy_com->GetAlr().Get(), nullptr)))
		return false;

	//テクスチャ領域をコピーする
	//転送元（MAP用リソース）
	D3D12_TEXTURE_COPY_LOCATION  src;
	ZeroMemory(&src, sizeof(src));
	src.pResource = resource_up_load;
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint = footprint;
	//転送先(テクスチャリソース)
	D3D12_TEXTURE_COPY_LOCATION  dest;
	ZeroMemory(&dest, sizeof(dest));
	dest.pResource = m_resource.Get();
	dest.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dest.SubresourceIndex = 0;//テクスチャが配列の場合要素数を入れる
	//コマンド：テクスチャ領域をコピー
	copy_com->GetList()->CopyTextureRegion(&dest, 0, 0, 0, &src, nullptr);

	//コマンド実行
	copy_com->GetList()->Close();
	ID3D12CommandList* lists[] = { copy_com->GetList().Get() };
	copy_com->GetQueue()->ExecuteCommandLists(1, lists);

	//フェンスのシグナルを初期の0に戻す
	copy_com->GetFence()->Signal(0);

	//キューの状態「コマンド実行中」「コマンド完了」をフェンスが確保
	//キューのシグナルが１なら「コマンド完了」設定、　0は「コマンド実行中」にフェンス値が変動する
	if (FAILED(copy_com->GetQueue()->Signal(copy_com->GetFence().Get(), 1)))
		return false;

	// 現在のフェンスが0値ならコマンド実行中のため、待機させる
	if (copy_com->GetFence()->GetCompletedValue() < 1)
	{
		//イベントに「コマンド完了」が来たら知らせる指示
		if (FAILED(copy_com->GetFence()->SetEventOnCompletion(1, copy_com->GetFenceEvent())))
			return false;
		//イベント「コマンド完了」が来るまで待機
		WaitForSingleObject(copy_com->GetFenceEvent(), INFINITE);
	}

	//テクスチャコピーコマンド情報破棄
	if (FAILED(copy_com->GetAlr()->Reset()))
		return false;
	if (FAILED(copy_com->GetList()->Reset(copy_com->GetAlr().Get(), nullptr)))
		return false;

	copy_com.reset();
	return true;
}