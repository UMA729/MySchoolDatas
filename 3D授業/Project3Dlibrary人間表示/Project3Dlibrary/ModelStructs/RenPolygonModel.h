#pragma once
#include "../DrectX12/DX12Library.h"
#include "../BufferAndImg/Buffer.h"

//���f���F�����_�����O�|���S��1��
class Model_RenderPolygon
{
	public:
		Model_RenderPolygon(ComPtr<ID3D12Device>& dev)
		{
			m_index.reset(new Index(dev));
			m_vertex.reset(new Vertex(dev));
			m_render_tex.reset(new RenderTexture(dev, 800, 720));
			m_z_buffer.reset(new ZBuffer(dev, 800,720));


			//����
			//m_flont.reset(new FontTex());
			//wstring str[15]
			//{
			//	{L"0123456789 <>?!"},
			//	{L"abcdefghijklmno"},
			//	{L"pqrstuvwxyz&|()"},
			//	{L"������������������������������"},
			//	{L"�����ĂƂȂɂʂ˂̂͂Ђӂւ�"},
			//	{L"�܂݂ނ߂������������"},
			//	{L"�������������������������Âł�"},
			//	{L"�΂тԂׂڂς҂Ղ؂ۂ���������"},
			//	{L"��僔���|�����������y+-/'"},
			//	{L"�A�C�E�G�I�J�L�N�P�R�T�V�X�Z�\"},
			//	{L"�^�`�c�e�g�i�j�k�l�m�n�q�t�w�z"},
			//	{L"�}�~�����������������K�M�O�Q�S"},
			//	{L"�U�W�Y�[�]�_�a�d�f�h�o�r�u�x�{"},
			//	{L"�p�s�v�y�|�`�a�b�c�d�e�f�g�h�i"},
			//	{L"�j�k�l�m�n�o�p�q�r�s�t�u�v�w�x"},
			//};
			//m_flont->LoadFontImage(dev, str);
		};
		~Model_RenderPolygon()
		{
			m_index.reset();
			m_vertex.reset();
			m_render_tex.reset();
			m_z_buffer.reset();


			//����
			//m_flont.reset();
		};

		//�o�[�e�b�N�X�o�b�t�@--------------------------
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

			D3D12_VERTEX_BUFFER_VIEW	m_view;	//�o�[�e�b�N�X�r���[
		private:
			//���_���C�A�E�g�\����
			struct Layout
			{
				XMFLOAT3 Pos;		//�ʒu(X,Y,Z)
				XMFLOAT3 Color;		//�F(R,G,B)
				XMFLOAT2 Tex;		//�e�N�X�`���ʒu(U,V)
			};

			//���_���
			Layout m_Vertices[4]
			{
				{ XMFLOAT3(-1.0f, -1.0f,  0.0f) , XMFLOAT3(1.0f, 0.0f, 1.0f) , XMFLOAT2(0.0f,1.0f) },
				{ XMFLOAT3( 1.0f, -1.0f,  0.0f) , XMFLOAT3(0.0f, 1.0f, 1.0f) , XMFLOAT2(1.0f,1.0f) },
				{ XMFLOAT3(-1.0f,  1.0f,  0.0f) , XMFLOAT3(1.0f, 1.0f, 0.0f) , XMFLOAT2(0.0f,0.0f) },
				{ XMFLOAT3( 1.0f,  1.0f,  0.0f) , XMFLOAT3(1.0f, 1.0f, 1.0f) , XMFLOAT2(1.0f,0.0f) },
			};
		};
		//�C���f�b�N�X�o�b�t�@---------------------------
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

			D3D12_INDEX_BUFFER_VIEW	m_view;	//�C���f�b�N�X�r���[
		private:

			//�C���f�b�N�X
			uint32_t m_Indices[6]
			{
				0,2,1,	1,2,3,
			};
		};
		
		//--------------------------------------------------
		//�f�[�^
		//--------------------------------------------------
		unique_ptr<Index>			m_index;
		unique_ptr<Vertex>			m_vertex;
		unique_ptr<RenderTexture>	m_render_tex;
		unique_ptr<ZBuffer>			m_z_buffer;


		//����
		//unique_ptr<FontTex>	m_flont;
	
};