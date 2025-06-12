#pragma once
#include "../DrectX12/DX12Library.h"
#include "../BufferAndImg/Buffer.h"
#include "../LoadFile/ObjLoad.h"
#include "../LoadFile/FbxLoad.h"

//モデル 3Dキューブ
class Model_3dCube
{
public:
	Model_3dCube(ComPtr<ID3D12Device>& dev)
	{
		//m_obj_file.reset(new OBJ_FILE::ObjFile());
		m_fbx_file.reset(new FBX_FILE::FbxFile());

		m_index.reset(new Index(dev, m_fbx_file));
		m_vertex.reset(new Vertex(dev, m_fbx_file));

		m_materialId = m_fbx_file->meshs[0].materialId;
		
		//マテリアルデータ
		m_model_mat.resize(m_fbx_file->material.size());
		for (int i = 0; i < m_model_mat.size(); i++)
			m_model_mat[i].reset(new BMaterial(dev, &m_fbx_file->material[i]));


		//テクスチャ
		m_tex.reset(new Texture());
		//string img_name{ "3DModelResource/Color.png" };//「_/..」ファイル名に記号を入れるのはダメ
		//											   //ファイル名に日本語ダメ　ローマ字ダメ
		//m_tex->LoadTexImage(dev, img_name);

		//ディふぃーずマップ情報をm_texに登録してテクスチャ化する
		m_tex->LoadTexImage(dev, m_fbx_file->tex[0].const_img);
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
		Vertex(ComPtr<ID3D12Device>& dev, unique_ptr<FBX_FILE::FbxFile>& fbx_file)
		{
			m_v.resize(fbx_file->meshs[0].vertexs.size());
			FBX_FILE::MeshVertex* p = &fbx_file->meshs[0].vertexs[0];

			for (int i = 0; i < m_v.size(); i++)
			{
				memcpy(&m_v[i].Pos	, &p[i].pos, sizeof(XMFLOAT3));
				memcpy(&m_v[i].Color, &p[i].nol, sizeof(XMFLOAT3));
				memcpy(&m_v[i].Uv	, &p[i].uv , sizeof(XMFLOAT3));
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
		Index(ComPtr<ID3D12Device>& dev, unique_ptr<FBX_FILE::FbxFile>& fbx_file)
		{
			m_inx=fbx_file->meshs[0].index;
		

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

	class BMaterial :public StructBuffer
	{
	public:
		BMaterial(ComPtr<ID3D12Device>& dev, FBX_FILE::Material* mat)
		{
			MAT_DATA material;
			material.diffuse.x = mat->diffuse[0];
			material.diffuse.y = mat->diffuse[1];
			material.diffuse.z = mat->diffuse[2];
			material.diffuse.w = 1.0f;

			material.specular.x = mat->diffuse[0];
			material.specular.y = mat->diffuse[1];
			material.specular.z = mat->diffuse[2];
			material.specular.w = 1.0f;

			material.shininess_to_amb.x = mat->shininess;
			material.shininess_to_amb.y = mat->ambient;
			material.shininess_to_amb.z = 1.0f;
			material.shininess_to_amb.w = 1.0f;
			//マテリアル情報をバッファにする
			CreateResource(dev, sizeof(MAT_DATA));
			UpDate(sizeof(MAT_DATA), &material);
		}

		//マテリアルデータ
		class MAT_DATA
		{
		public:
			XMFLOAT4 diffuse;		   //ディフューズ
			XMFLOAT4 specular;		   //スペキュラ
			XMFLOAT4 shininess_to_amb; //シャインとアンビエント
		};
	};
	//データ---------------------------
	unique_ptr<Index>				m_index;
	unique_ptr<Vertex>				m_vertex;
	int32_t							m_materialId;	//マテリアルID

	vector<unique_ptr<BMaterial>>	m_model_mat;	//マテリアルデータ

	//unique_ptr<OBJ_FILE::ObjFile> m_obj_file;
	unique_ptr<FBX_FILE::FbxFile> m_fbx_file;		//FBXファイル解析
	unique_ptr<Texture>			  m_tex;		//テクスチャ
};