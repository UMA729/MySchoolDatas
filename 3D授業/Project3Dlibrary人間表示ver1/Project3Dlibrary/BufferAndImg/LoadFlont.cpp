#include <windows.h>
#include <gdiplus.h>	//GDI+を使用
#include <string>
#include <codecvt>
#include <vector>
#include<array>

#include "../DrectX12/DX12Library.h"
#include "Buffer.h"

//フォントイメージの作成
void FontTex::LoadFontImage(ComPtr<ID3D12Device>& dev, wstring str[15])
{
	//空テクスチャを作成
	//アップロード用
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT  footprint{ 0 };		//配置先サブリソースのフットプリント
	UINT64								m_total_bytes{ 0 };
	ID3D12Resource* resource_up_load{ nullptr };//アップロード用テクスチャリソース
	//アップロードと出力テクスチャ作成
	CreateOutPutTexResourceView(dev, footprint, m_total_bytes);
	CreateUpLoadResource(dev, &resource_up_load, m_total_bytes);

	void* ptr = nullptr;
	resource_up_load->Map(0, nullptr, &ptr);//リソースをマップ

	//アップロード用テクスチャリソースのイメージ部の先頭アドレス取得
	UINT32* p_rgb = (reinterpret_cast<UINT32*>(ptr) + footprint.Offset);
	memset(p_rgb, 0x00, sizeof(UINT32) * TEX_SIZE * TEX_SIZE);


	//１文字ずつ作成
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < str[i].size(); j++)
		{
			//文字列から文字を取り出す
			wstring c = wstring(str[i].substr(j, CHAR_STEP));
			//取り出した文字を指定した位置にイメージ化
			LoadCharImage(c, p_rgb, (j % (FONT_SIZE / 2)) * FONT_SIZE, i * FONT_SIZE);
		}
	}
	CopyImageDataTexture(dev, footprint, resource_up_load);

	resource_up_load->Unmap(0, 0);//リソースのアンマップ
	resource_up_load->Release();
}

//登録文字を元にテクスチャを作成
void FontTex::LoadCharImage(wstring c, UINT32* p, int pos_x, int pos_y)
{
	//文字フォント情報
	BYTE* ptr;
	DWORD size;
	GLYPHMETRICS GM;

	//文字作成用
	UINT code = (UINT)c[0];//文字を文字コードにする
	//文字フォントをビットマップ化した時のサイズを測る
	size = GetGlyphOutline(m_hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &MAT);
	ptr = new BYTE[size];
	//ビットマップ取得（文字フォントのアウトラインをビットマップにして取得）
	GetGlyphOutline(m_hdc, code, GGO_GRAY4_BITMAP, &GM, size, ptr, &MAT);

	//書き込み位置
	int x = pos_x;
	int y = pos_y;

	// フォント情報の書き込み
	int iOfs_x = GM.gmptGlyphOrigin.x;
	int iOfs_y = m_TM.tmAscent - GM.gmptGlyphOrigin.y;
	int iBmp_w = GM.gmBlackBoxX + (4 - (GM.gmBlackBoxX % 4)) % 4;
	int iBmp_h = GM.gmBlackBoxY;
	int Level = 17;

	//リソースに文字を書き込む
	UINT32 Alpha, Color;
	for (unsigned int i = iOfs_y; i < iOfs_y + iBmp_h; i++)
	{
		for (unsigned int j = iOfs_x; j < iOfs_x + GM.gmBlackBoxX; j++)
		{
			Alpha = (255 * ptr[j - iOfs_x + iBmp_w * (i - iOfs_y)]) / (Level - 1);
			Color = 0x00ffffff | (Alpha << 24);
			(reinterpret_cast<UINT32*>(p))[(i + y) * TEX_SIZE + (j + x)] = Color;
		}
	}

	//ビットマップ情報破棄
	delete[] ptr;
}

//--------------------------------------------------------------------------//