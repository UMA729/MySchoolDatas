#pragma once
#include "../DrectX12/DX12Library.h"
#include "../BufferAndImg/Buffer.h"
#include "../LoadFile/ObjLoad.h"
#include "../LoadFile/FbxLoad.h"

//���f�� 3D�L���[�u
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
		
		//�}�e���A���f�[�^
		m_model_mat.resize(m_fbx_file->material.size());
		for (int i = 0; i < m_model_mat.size(); i++)
			m_model_mat[i].reset(new BMaterial(dev, &m_fbx_file->material[i]));

		//�{�[���f�[�^
		m_bone.reset(new B_Bone(dev));

		//�e�N�X�`��
		m_tex.reset(new Texture());
		//string img_name{ "3DModelResource/Color.png" };//�u_/..�v�t�@�C�����ɋL��������̂̓_��
		//											   //�t�@�C�����ɓ��{��_���@���[�}���_��
		//m_tex->LoadTexImage(dev, img_name);

		//�f�B�ӂ��[���}�b�v����m_tex�ɓo�^���ăe�N�X�`��������
		m_tex->LoadTexImage(dev, m_fbx_file->tex[0].const_img);
	};
	~Model_3dCube()
	{
		m_index.reset();
		m_vertex.reset();
	
	};

	//�o�[�e�b�N�X�o�b�t�@--------------------------
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

				memcpy(&m_v[i].Tan, &p[i].tan, sizeof(XMFLOAT3));
				//�X�L���f�[�^������ꍇ�̓f�[�^������
				if (fbx_file->meshs[0].skin.size() != 0)
				{
					memcpy(&m_v[i].Id, &fbx_file->meshs[0].skin[i].mat_id, sizeof(XMUINT4));
					memcpy(&m_v[i].We, &fbx_file->meshs[0].skin[i].weight, sizeof(XMFLOAT4));
				}
			}

			CreateResource(dev, m_v.size() * sizeof(Layout));
			UpDate(m_v.size() * sizeof(Layout), &m_v[0]);
			m_view.BufferLocation = m_resource->GetGPUVirtualAddress();
			m_view.StrideInBytes = sizeof(Layout);
			m_view.SizeInBytes = m_v.size() * sizeof(Layout);
		}

		D3D12_VERTEX_BUFFER_VIEW	m_view;	//�o�[�e�b�N�X�r���[
	private:
		//���_���C�A�E�g�\����
		struct Layout
		{
			XMFLOAT3 Pos;		//�ʒu(X,Y,Z)
			XMFLOAT3 Color;		//�F(R,G,B)
			XMFLOAT2 Uv;		//UV�iu,v�j
			XMFLOAT3 Tan;		//�ڐ�x�N�g��
			XMUINT4 Id;			//�e�����󂯂�s��ID
			XMFLOAT4 We;		//�e���x�i�d�݁j
		};

		//���_���
		//�L���[�u�̒��_���
		vector<Layout> m_v;
	};

	//�C���f�b�N�X�o�b�t�@---------------------------
	class Index :public StructBuffer
	{
	public:
		Index(ComPtr<ID3D12Device>& dev, unique_ptr<FBX_FILE::FbxFile>& fbx_file)
		{
			m_inx=fbx_file->meshs[0].index;
		

			//�C���f�b�N�X�o�b�t�@�ɂ���
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

		D3D12_INDEX_BUFFER_VIEW	m_view;	//�C���f�b�N�X�r���[
	private:

		//�C���f�b�N�X
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
			//�}�e���A�������o�b�t�@�ɂ���
			CreateResource(dev, sizeof(MAT_DATA));
			UpDate(sizeof(MAT_DATA), &material);
		}

		//�}�e���A���f�[�^
		class MAT_DATA
		{
		public:
			XMFLOAT4 diffuse;		   //�f�B�t���[�Y
			XMFLOAT4 specular;		   //�X�y�L����
			XMFLOAT4 shininess_to_amb; //�V���C���ƃA���r�G���g
		};
	
	};

	//�{�[���o�b�t�@------------------------------
	class B_Bone :public StructBuffer
	{
	public:
		B_Bone(ComPtr<ID3D12Device>& dev)
		{
			//�{�[�����i���F�P�ʍs��j���o�b�t�@�ɂ��ēo�^
			XMMATRIX mb;
			mb = XMMatrixIdentity();//�P�ʍs��F���̂܂�
			for (int i = 0; i < 128; i++)
				XMStoreFloat4x4(&mat[i], mb);

			//�{�[�������o�b�t�@�ɂ���
			CreateResource(dev, sizeof(mat));
			UpDate(sizeof(mat), &mat);
		}
		//�{�[���s��
		XMFLOAT4X4 mat[128];
	};
	//�f�[�^---------------------------
	unique_ptr<Index>				m_index;
	unique_ptr<Vertex>				m_vertex;
	int32_t							m_materialId;	//�}�e���A��ID
	unique_ptr<B_Bone>				m_bone;

	vector<unique_ptr<BMaterial>>	m_model_mat;	//�}�e���A���f�[�^

	//unique_ptr<OBJ_FILE::ObjFile> m_obj_file;
	unique_ptr<FBX_FILE::FbxFile> m_fbx_file;		//FBX�t�@�C�����
	unique_ptr<Texture>			  m_tex;		//�e�N�X�`��
};