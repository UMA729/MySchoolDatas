#pragma once
#include <tchar.h>
#include <memory>
#include <wrl/client.h>	//COM�̃X�}�[�g��
#include <string>
#include <locale.h>
//Windows��DirectX�̃C���N���[�h
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>


//�l�[���X�y�[�X�V���[�g�J�b�g
using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;

//-------------------------------------------------------
//�\���o�b�t�@
class StructBuffer
{
public:
	StructBuffer() {}
	~StructBuffer()
	{
		m_resource.Reset();
	}
	void CreateResource(ComPtr<ID3D12Device>& dev, size_t size);	//�o�b�t�@���\�[�X�쐬
	void UpDate(size_t size, void* data);							//�o�b�t�@�փf�[�^�A�b�v���[�h
	ComPtr<ID3D12Resource>& Getbuf() { return m_resource; }
protected:
	ComPtr<ID3D12Resource> m_resource;							//���\�[�X
};
//-------------------------------------------------------
//-------------------------------------------------------
//�`��i�e�N�X�`���j���\�[�X�o�b�t�@
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
	ComPtr<ID3D12Resource>				m_resource{ nullptr };			//���\�[�X
	ComPtr<ID3D12DescriptorHeap>		m_descriptorHeap{ nullptr };	//�f�X�N���v�^�q�[�v
	D3D12_CPU_DESCRIPTOR_HANDLE			m_handle{ 0 };					//�n���h���i�r���[�j
};
//--------------------------------------------------------
//�����_�����O�e�N�X�`���o�b�t�@--------------------------
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
	//�^�[�Q�b�g�p
	ComPtr<ID3D12DescriptorHeap>		m_taget_descriptorHeap{ nullptr };	//�f�X�N���v�^�q�[�v
	D3D12_CPU_DESCRIPTOR_HANDLE			m_taget_handle{ 0 };				//�n���h��
};
//--------------------------------------------------------
//Z�o�b�t�@
class ZBuffer :public ResourceBuffer
{
public:
	ZBuffer(ComPtr<ID3D12Device>& dev, int w, int h);
	~ZBuffer() {};
private:
};
//--------------------------------------------------------
//�e�N�X�`���o�b�t�@
//�C���[�W���[�h�pGDI+
namespace GDI_OBJ
{
	constexpr int MAX_SIZE{ 512 };//�C���[�W�ǂݍ��ݍő�̑傫��
	void Initialize();
	void Release();
}

//�e�N�X�`���o�b�t�@
class Texture :public ResourceBuffer
{
public:
	Texture() {};
	~Texture() {};
	void LoadTexImage(ComPtr<ID3D12Device>& dev, string tex_name);
	void LoadTexImage(ComPtr<ID3D12Device>& dev, float const_img[512][512][4]);
protected:
	//�o�͗p�e�N�X�`�����\�[�X�r���[�쐬
	bool CreateOutPutTexResourceView(ComPtr<ID3D12Device>& dev, D3D12_PLACED_SUBRESOURCE_FOOTPRINT& footprint, UINT64& total_bytes);
	//�A�b�v���[�h�e�N�X�`�����\�[�X�쐬
	bool CreateUpLoadResource(ComPtr<ID3D12Device>& dev, ID3D12Resource** resource_up_load, UINT64 total_bytes);
	//�A�b�v���[�h�����o�̓e�N�X�`���ɃR�s�[ 
	bool CopyImageDataTexture(ComPtr<ID3D12Device>& dev,D3D12_PLACED_SUBRESOURCE_FOOTPRINT& footprint, ID3D12Resource* resource_up_load);
};
//--------------------------------------------------------
//�t�H���g�o�b�t�@
class FontTex :public Texture
{
public:
	FontTex()
	{
		// �t�H���g���
		HFONT hFont = CreateFont(
			FONT_SIZE,0, 0, 0, 0, FALSE, FALSE, FALSE,
			SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
			FIXED_PITCH | FF_MODERN,
			L"�l�r �S�V�b�N"	//�g�p�t�H���g
		);
		m_hdc = GetDC(NULL);
		m_oldFont = (HFONT)SelectObject(m_hdc, hFont);
		GetTextMetrics(m_hdc, &m_TM);
		m_hFont = hFont;
		return;
	
	};
	~FontTex()
	{
		//�t�H���g�I�u�W�F�N�g�̊J��
		DeleteObject(m_oldFont);
		DeleteObject(m_hFont);
		ReleaseDC(NULL, m_hdc);
	};
	void LoadFontImage(ComPtr<ID3D12Device>& dev, wstring str[15]);
	
private:
	void LoadCharImage( wstring c, UINT32* p, int pos_x, int pos_y);
	//�t�H���g���
	HFONT		m_hFont;
	HDC			m_hdc;
	HFONT		m_oldFont;
	TEXTMETRIC	m_TM;
	//�t�H���g�e�N�X�`���T�C�Y
	const int  TEX_SIZE{ 512 };
	const int  FONT_SIZE{ 31 };
	const int  FONT_SHIFT{ 4 };
	const MAT2 MAT{ {0,1},{0,0},{0,0},{0,1} };
	const int  CHAR_STEP{ 1 };
};
//--------------------------------------------------------
//�e�N�X�`���z�񉻃w���p�[�N���X
class TextureArray
{
public:
	TextureArray(ComPtr<ID3D12Device>& dev,int texmax)
	{
		if (texmax > SET_TEX_MAX) return;
		//�f�B�X�N���v�^�q�[�v���쐬
		D3D12_DESCRIPTOR_HEAP_DESC	descriptor_heap_desc{};
		ZeroMemory(&descriptor_heap_desc, sizeof(descriptor_heap_desc));
		descriptor_heap_desc.NumDescriptors = texmax;
		descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;		
		if (FAILED(dev->CreateDescriptorHeap(&descriptor_heap_desc,
			IID_PPV_ARGS(m_descriptorHeap.ReleaseAndGetAddressOf()))))
			return;
		//�z��̗v�f�̃T�C�Y
		strideHandleBytes = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	};

	~TextureArray()
	{
		m_descriptorHeap.Reset();
	}

	//�e�N�X�`����z��ɓo�^
	void SetTexture(ComPtr<ID3D12Device>& dev, ResourceBuffer* b,int set_id)
	{
		if (set_id >= SET_TEX_MAX) return;
		//���\�[�X�r���[�f�B�X�N
		D3D12_SHADER_RESOURCE_VIEW_DESC	shader_resource_view_desc{};
		shader_resource_view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		shader_resource_view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		shader_resource_view_desc.Texture2D.MipLevels = 1;
		shader_resource_view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//���\�[�X���f�B�X�N���v�^�[�q�[�v�̊e�v�f�ɐݒ�
		D3D12_CPU_DESCRIPTOR_HANDLE	handle;
		handle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += strideHandleBytes*set_id;
		dev->CreateShaderResourceView(b->GetResource().Get(), &shader_resource_view_desc, handle);
	}

	//SetDescriptorHeaps�p
	ComPtr<ID3D12DescriptorHeap>& GetD()
	{
		return m_descriptorHeap;
	}
	//SetGraphicsRootDescriptorTable�p
	D3D12_GPU_DESCRIPTOR_HANDLE GetH(int id)
	{
		if (id >= SET_TEX_MAX) id =0;
		D3D12_GPU_DESCRIPTOR_HANDLE	h;
		h=m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
		h.ptr += strideHandleBytes*id;
		return h;
	}

	ComPtr<ID3D12DescriptorHeap> m_descriptorHeap{ nullptr };	//�f�B�X�N���v�^�q�[�v
	UINT strideHandleBytes;
	const int SET_TEX_MAX{ 8 };//�e�N�X�`���ݒ�ő吔
};