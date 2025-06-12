#pragma once
#include "../DrectX12/DX12Library.h"
#include "../BufferAndImg/Buffer.h"
#include "../LoadFile/ObjLoad.h"

//モデル 3Dキューブ
class Model_3dCube
{
public:
	Model_3dCube(ComPtr<ID3D12Device>& dev)
	{
		m_obj_file.reset(new OBJ_FILE::ObjFile());

		m_index.reset(new Index(dev,m_obj_file));
		m_vertex.reset(new Vertex(dev,m_obj_file));
		
		//テクスチャ
		m_tex.reset(new Texture());
		string img_name{ "3DModelResource/Color.png" };//「_/..」ファイル名に記号を入れるのはダメ
													   //ファイル名に日本語ダメ　ローマ字ダメ
		m_tex->LoadTexImage(dev, img_name);
	};
	~Model_3dCube()
	{
		m_index.reset();
		m_vertex.reset();
	
	};

	//バーテックスバッファ--------------------------
	class Vertex :public StructBuffer
	{
	public:
		Vertex(ComPtr<ID3D12Device>& dev, unique_ptr<OBJ_FILE::ObjFile>& obj_file)
		{
			Layout v;
			int id_v, id_n ,id_u;

			for (int count = 0;count < obj_file->m_Face.size();count++)
			{
				int order[3]{ 0,1,2 };
				for (int i = 0;i < 3;i++)
				{
					//作成する頂点のID情報を取得
					id_v = obj_file->m_Face[count].v[order[i]].pos - 1;//objは「1」スタートなので
					id_n = obj_file->m_Face[count].v[order[i]].nol - 1;//objは「1」スタートなので
					id_u = obj_file->m_Face[count].v[order[i]].uv - 1;//objは「1」スタートなので
				
					//獲得したIDから頂点位置を作成
					v.Pos.x = obj_file->m_Pos[id_v].x;
					v.Pos.y = obj_file->m_Pos[id_v].y;
					v.Pos.z = obj_file->m_Pos[id_v].z;

					v.Color.x = obj_file->m_Nol[id_n].x;
					v.Color.y = obj_file->m_Nol[id_n].y;
					v.Color.z = obj_file->m_Nol[id_n].z;
					
					v.Uv.x = obj_file->m_Uv[id_u].u;
					v.Uv.y = obj_file->m_Uv[id_u].v;
				
					m_v.push_back(v);
				}

				//四角形ポリゴンの場合は、さらに三角形を追加
				if (obj_file->m_Face[count].v.size() == 4)
				{
					int order[3]{ 3,2,0 };
					for (int i = 0;i < 3;i++)
					{
						//作成する頂点のID情報を取得
						id_v = obj_file->m_Face[count].v[order[i]].pos - 1;//objは「1」スタートなので
						id_n = obj_file->m_Face[count].v[order[i]].nol - 1;//objは「1」スタートなので
						id_u = obj_file->m_Face[count].v[order[i]].uv - 1;//objは「1」スタートなので

						//獲得したIDから頂点位置を作成
						v.Pos.x = obj_file->m_Pos[id_v].x;
						v.Pos.y = obj_file->m_Pos[id_v].y;
						v.Pos.z = obj_file->m_Pos[id_v].z;

						v.Color.x = obj_file->m_Nol[id_n].x;
						v.Color.y = obj_file->m_Nol[id_n].y;
						v.Color.z = obj_file->m_Nol[id_n].z;

						v.Uv.x = obj_file->m_Uv[id_u].u;
						v.Uv.y = obj_file->m_Uv[id_u].v;

						m_v.push_back(v);
					}
				}
			}



			CreateResource(dev, m_v.size() * sizeof(Layout));
			UpDate(m_v.size() * sizeof(Layout), &m_v[0]);
			m_view.BufferLocation = m_resource->GetGPUVirtualAddress();
			m_view.StrideInBytes = sizeof(Layout);
			m_view.SizeInBytes = m_v.size() * sizeof(Layout);
		}

		D3D12_VERTEX_BUFFER_VIEW	m_view;	//バーテックスビュー
	private:
		//頂点レイアウト構造体
		struct Layout
		{
			XMFLOAT3 Pos;		//位置(X,Y,Z)
			XMFLOAT3 Color;		//色(R,G,B)
			XMFLOAT2 Uv;		//UV（u,v）

		};

		//頂点情報
		//キューブの頂点情報
		vector<Layout> m_v;
	};
	//インデックスバッファ---------------------------
	class Index :public StructBuffer
	{
	public:
		Index(ComPtr<ID3D12Device>& dev,unique_ptr<OBJ_FILE::ObjFile>& obj_file)
		{
			int max = 0;
			//面の数を数える
			for (int i = 0;i < obj_file->m_Face.size();i++)
			{
				if (obj_file->m_Face[i].v.size() == 3)
					max += 3;
				if (obj_file->m_Face[i].v.size() == 4)
					max += 6;
			}
			m_inx.resize(max);
			//m_inxに番号を振る
			for (int i = 0;i < m_inx.size();i++)
			{
				m_inx[i] = i;
			}
			//インデックスバッファにする
			CreateResource(dev, m_inx.size()*sizeof(uint32_t));
			UpDate(m_inx.size()*sizeof(uint32_t), &m_inx[0]);
			m_view.BufferLocation = m_resource->GetGPUVirtualAddress();
			m_view.SizeInBytes = m_inx.size() * sizeof(uint32_t);
			m_view.Format = DXGI_FORMAT_R32_UINT;
		}
		int Size()
		{
			return m_inx.size();
		}

		D3D12_INDEX_BUFFER_VIEW	m_view;	//インデックスビュー
	private:

		//インデックス
		vector<uint32_t> m_inx;
	};

	//データ---------------------------
	unique_ptr<Index>			m_index;
	unique_ptr<Vertex>			m_vertex;

	unique_ptr<OBJ_FILE::ObjFile> m_obj_file;
	unique_ptr<Texture>			  m_tex;		//テクスチャ
};