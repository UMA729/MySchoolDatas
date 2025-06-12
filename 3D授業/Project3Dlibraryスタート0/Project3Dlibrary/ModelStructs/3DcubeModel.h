#pragma once
#include "../DrectX12/DX12Library.h"
#include "../BufferAndImg/Buffer.h"
#include "../LoadFile/ObjLoad.h"

//���f�� 3D�L���[�u
class Model_3dCube
{
public:
	Model_3dCube(ComPtr<ID3D12Device>& dev)
	{
		m_obj_file.reset(new OBJ_FILE::ObjFile());

		m_index.reset(new Index(dev,m_obj_file));
		m_vertex.reset(new Vertex(dev,m_obj_file));
		
		//�e�N�X�`��
		m_tex.reset(new Texture());
		string img_name{ "3DModelResource/Color.png" };//�u_/..�v�t�@�C�����ɋL��������̂̓_��
													   //�t�@�C�����ɓ��{��_���@���[�}���_��
		m_tex->LoadTexImage(dev, img_name);
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
		Vertex(ComPtr<ID3D12Device>& dev, unique_ptr<OBJ_FILE::ObjFile>& obj_file)
		{
			Layout v;
			int id_v, id_n ,id_u;

			for (int count = 0;count < obj_file->m_Face.size();count++)
			{
				int order[3]{ 0,1,2 };
				for (int i = 0;i < 3;i++)
				{
					//�쐬���钸�_��ID�����擾
					id_v = obj_file->m_Face[count].v[order[i]].pos - 1;//obj�́u1�v�X�^�[�g�Ȃ̂�
					id_n = obj_file->m_Face[count].v[order[i]].nol - 1;//obj�́u1�v�X�^�[�g�Ȃ̂�
					id_u = obj_file->m_Face[count].v[order[i]].uv - 1;//obj�́u1�v�X�^�[�g�Ȃ̂�
				
					//�l������ID���璸�_�ʒu���쐬
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

				//�l�p�`�|���S���̏ꍇ�́A����ɎO�p�`��ǉ�
				if (obj_file->m_Face[count].v.size() == 4)
				{
					int order[3]{ 3,2,0 };
					for (int i = 0;i < 3;i++)
					{
						//�쐬���钸�_��ID�����擾
						id_v = obj_file->m_Face[count].v[order[i]].pos - 1;//obj�́u1�v�X�^�[�g�Ȃ̂�
						id_n = obj_file->m_Face[count].v[order[i]].nol - 1;//obj�́u1�v�X�^�[�g�Ȃ̂�
						id_u = obj_file->m_Face[count].v[order[i]].uv - 1;//obj�́u1�v�X�^�[�g�Ȃ̂�

						//�l������ID���璸�_�ʒu���쐬
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

		D3D12_VERTEX_BUFFER_VIEW	m_view;	//�o�[�e�b�N�X�r���[
	private:
		//���_���C�A�E�g�\����
		struct Layout
		{
			XMFLOAT3 Pos;		//�ʒu(X,Y,Z)
			XMFLOAT3 Color;		//�F(R,G,B)
			XMFLOAT2 Uv;		//UV�iu,v�j

		};

		//���_���
		//�L���[�u�̒��_���
		vector<Layout> m_v;
	};
	//�C���f�b�N�X�o�b�t�@---------------------------
	class Index :public StructBuffer
	{
	public:
		Index(ComPtr<ID3D12Device>& dev,unique_ptr<OBJ_FILE::ObjFile>& obj_file)
		{
			int max = 0;
			//�ʂ̐��𐔂���
			for (int i = 0;i < obj_file->m_Face.size();i++)
			{
				if (obj_file->m_Face[i].v.size() == 3)
					max += 3;
				if (obj_file->m_Face[i].v.size() == 4)
					max += 6;
			}
			m_inx.resize(max);
			//m_inx�ɔԍ���U��
			for (int i = 0;i < m_inx.size();i++)
			{
				m_inx[i] = i;
			}
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

	//�f�[�^---------------------------
	unique_ptr<Index>			m_index;
	unique_ptr<Vertex>			m_vertex;

	unique_ptr<OBJ_FILE::ObjFile> m_obj_file;
	unique_ptr<Texture>			  m_tex;		//�e�N�X�`��
};