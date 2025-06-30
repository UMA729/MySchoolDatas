#pragma once
#include "../DrectX12/DX12Library.h"
#include "../BufferAndImg/Buffer.h"

//モデル：レンダリングポリゴン1枚
class Model_RenderPolygon
{
	public:
		Model_RenderPolygon(ComPtr<ID3D12Device>& dev)
		{
			m_index.reset(new Index(dev));
			m_vertex.reset(new Vertex(dev));
			m_render_tex.reset(new RenderTexture(dev, 800, 720));
			m_z_buffer.reset(new ZBuffer(dev, 800,720));


			//実験
			//m_flont.reset(new FontTex());
			//wstring str[15]
			//{
			//	{L"0123456789 <>?!"},
			//	{L"abcdefghijklmno"},
			//	{L"pqrstuvwxyz&|()"},
			//	{L"あいうえおかきくけこさしすせそ"},
			//	{L"たちつてとなにぬねのはひふへほ"},
			//	{L"まみむめもらりるれろやゆよわん"},
			//	{L"がぎぐげござじずぜぞだぢづでど"},
			//	{L"ばびぶべぼぱぴぷぺぽぁぃぅぉゃ"},
			//	{L"ゅょヴヤ－ユヨワヲンＺ+-/'"},
			//	{L"アイウエオカキクケコサシスセソ"},
			//	{L"タチツテトナニヌネノハヒフヘホ"},
			//	{L"マミムメモラリルレロガギグゲゴ"},
			//	{L"ザジズゼゾダヂヅデドバビブベボ"},
			//	{L"パピプペポＡＢＣＤＥＦＧＨＩＪ"},
			//	{L"ＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹ"},
			//};
			//m_flont->LoadFontImage(dev, str);
		};
		~Model_RenderPolygon()
		{
			m_index.reset();
			m_vertex.reset();
			m_render_tex.reset();
			m_z_buffer.reset();


			//実験
			//m_flont.reset();
		};

		//バーテックスバッファ--------------------------
		class Vertex :public StructBuffer
		{
		public:
			Vertex(ComPtr<ID3D12Device>& dev)
			{
				CreateResource(dev, sizeof(m_Vertices));
				UpDate(sizeof(m_Vertices), m_Vertices);
				m_view.BufferLocation = m_resource->GetGPUVirtualAddress();
				m_view.StrideInBytes = sizeof(Layout);
				m_view.SizeInBytes = sizeof(m_Vertices);
			}

			D3D12_VERTEX_BUFFER_VIEW	m_view;	//バーテックスビュー
		private:
			//頂点レイアウト構造体
			struct Layout
			{
				XMFLOAT3 Pos;		//位置(X,Y,Z)
				XMFLOAT3 Color;		//色(R,G,B)
				XMFLOAT2 Tex;		//テクスチャ位置(U,V)
			};

			//頂点情報
			Layout m_Vertices[4]
			{
				{ XMFLOAT3(-1.0f, -1.0f,  0.0f) , XMFLOAT3(1.0f, 0.0f, 1.0f) , XMFLOAT2(0.0f,1.0f) },
				{ XMFLOAT3( 1.0f, -1.0f,  0.0f) , XMFLOAT3(0.0f, 1.0f, 1.0f) , XMFLOAT2(1.0f,1.0f) },
				{ XMFLOAT3(-1.0f,  1.0f,  0.0f) , XMFLOAT3(1.0f, 1.0f, 0.0f) , XMFLOAT2(0.0f,0.0f) },
				{ XMFLOAT3( 1.0f,  1.0f,  0.0f) , XMFLOAT3(1.0f, 1.0f, 1.0f) , XMFLOAT2(1.0f,0.0f) },
			};
		};
		//インデックスバッファ---------------------------
		class Index :public StructBuffer
		{
		public:
			Index(ComPtr<ID3D12Device>& dev)
			{
				CreateResource(dev, sizeof(m_Indices));
				UpDate(sizeof(m_Indices), m_Indices);
				m_view.BufferLocation = m_resource->GetGPUVirtualAddress();
				m_view.SizeInBytes = sizeof(m_Indices);
				m_view.Format = DXGI_FORMAT_R32_UINT;
			}
			int Size()
			{
				return sizeof(m_Indices) / sizeof(uint32_t);
			}

			D3D12_INDEX_BUFFER_VIEW	m_view;	//インデックスビュー
		private:

			//インデックス
			uint32_t m_Indices[6]
			{
				0,2,1,	1,2,3,
			};
		};
		
		//--------------------------------------------------
		//データ
		//--------------------------------------------------
		unique_ptr<Index>			m_index;
		unique_ptr<Vertex>			m_vertex;
		unique_ptr<RenderTexture>	m_render_tex;
		unique_ptr<ZBuffer>			m_z_buffer;


		//実験
		//unique_ptr<FontTex>	m_flont;
	
};