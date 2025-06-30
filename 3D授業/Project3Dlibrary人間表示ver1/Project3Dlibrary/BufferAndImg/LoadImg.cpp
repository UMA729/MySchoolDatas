#include <windows.h>
#include <gdiplus.h>	//GDI+���g�p
#include <string>
#include <codecvt>
#include <vector>
#include<array>

#include "../DrectX12/DX12Library.h"
#include "Buffer.h"

#pragma comment(lib, "gdiplus.lib")

//------------------------------�C���[�W---------------------------------//
namespace GDI_OBJ
{
	//GDI+�I�u�W�F�N�g
	ULONG_PTR g_GDIplusToken{ NULL };

	//GDI+�̏�����
	void Initialize()
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		if (Gdiplus::GdiplusStartup(&g_GDIplusToken, &gdiplusStartupInput, NULL) != Gdiplus::Ok)
			return;
	}

	//GDI+�̉��
	void Release()
	{
		Gdiplus::GdiplusShutdown(g_GDIplusToken);
	}
}


void Texture::LoadTexImage(ComPtr<ID3D12Device>& dev, float const_img[512][512][4])
{
	//�C���[�W���[�h
	vector<UINT32>p(512 * 512);
	
	//��e�N�X�`�����쐬----------------------------------
	//�A�b�v���[�h�p
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT  footprint{ 0 };//�z�u��T�u���\�[�X�̃t�b�g�v�����g
	UINT64	  m_total_bytes{ 0 };
	ID3D12Resource* resource_up_load{ nullptr };//�A�b�v���[�h�p�e�N�X�`�����\�[�X
	//�A�b�v���[�h�Əo�̓e�N�X�`���쐬
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
				
			//512�~512�e�N�X�`���Ƀf�[�^������
			UINT64 pos = y * 512 + x;
			p[pos] = (rgb[0]) | (rgb[1] << 8) | (rgb[2] << 16) | (rgb[3] << 24);
		}
	}

	//�A�b�v���[�h�p�̃e�N�X�`�����\�[�X�ɉ�f�f�[�^������--------------------
	void* ptr = nullptr;
	resource_up_load->Map(0, nullptr, &ptr);

	//�A�b�v���[�h�p�e�N�X�`�����\�[�X�̃C���[�W���̐擪�A�h���X�擾
	UINT32* p_rgba = (reinterpret_cast<UINT32*>(ptr) + footprint.Offset);

	for (int i = 0; i < 512; i++)
		for (int j = 0; j < 512; j++)
			p_rgba[i * 512 + j] = p[i * 512 + j];
	
	resource_up_load->Unmap(0, 0);

	CopyImageDataTexture(dev, footprint, resource_up_load);
	
	resource_up_load->Release();
}


//�C���[�W���̓ǂݍ���(�摜�t�@�C�����J���F�Ή��摜�`�� BMP, JPEG, PNG, GIF, TIFF, WMF, EMF)
void Texture::LoadTexImage(ComPtr<ID3D12Device>& dev, string tex_name)
{
	//�ǂݍ��݃C���[�W�̕�
	UINT64 w{ 0 }, h{ 0 };
	//�t�@�C��������摜�f�[�^�擾
	wchar_t wc[128]{ L"" };
	int ret{ 0 };
	//�ꎟ�C���[�W�o�b�t�@
	vector<vector<array<float, 4>>> img;
	//�C���[�W���[�h
	vector<UINT32>p(512 * 512);
	
	//��e�N�X�`�����쐬----------------------------------
	//�A�b�v���[�h�p
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT  footprint{ 0 };//�z�u��T�u���\�[�X�̃t�b�g�v�����g
	UINT64	  m_total_bytes{ 0 };
	ID3D12Resource* resource_up_load{ nullptr };//�A�b�v���[�h�p�e�N�X�`�����\�[�X
	//�A�b�v���[�h�Əo�̓e�N�X�`���쐬
	CreateOutPutTexResourceView(dev, footprint, m_total_bytes);
	CreateUpLoadResource(dev, &resource_up_load, m_total_bytes);

	//��e�N�X�`���ɉ摜�f�[�^�����[�h--------------------
	//������ƃ��C�h������ϊ��R���o�[�g
	ret = MultiByteToWideChar(CP_ACP, 0,tex_name.c_str(),tex_name.length() + 1,wc, 128);

	//Gdiplus�ŉ摜�t�@�C���̉�f���擾
	Gdiplus::Bitmap* pBitmap = Gdiplus::Bitmap::FromFile(wc);
	if (pBitmap->GetLastStatus() == Gdiplus::Ok)
	{
		h = pBitmap->GetWidth();
		w = pBitmap->GetHeight();

		//�ꎟ�C���[�W�o�b�t�@�쐬
		img.resize(h);
		for (int i = 0; i < h; i++)
			img[i].resize(w);

		//�O���t�B�b�N���f�[�^����1�s�N�Z�����`�悵�Ă���
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

		//���`��Ԃɂ��g��k��
		for (int y = 0; y < 512; y++)
		{
			for (int x = 0; x < 512; x++)
			{
				UINT64 rgb_w[4];
				for (int c = 0; c < 4; c++)
				{
					// �g��O�摜�ɂ�������W�v�Z
					float original_x = (double)x / (512.0f / w);
					float original_y = (double)y / (512.0f / h);

					// ���͂S�_�̍��W���v�Z
					int	x0 = (int)original_x;	int x1 = x0 + 1;
					int y0 = (int)original_y;	int y1 = y0 + 1;

					// m1��n1���摜���͂ݏo���ꍇ�̏���
					if (x1 >= 512) x1 = 512 - 1;
					if (y1 >= 512) y1 = 512 - 1;

					// ���X�̍��W��m0,n0�Ƃ̋������v�Z
					int dx = original_x - x0;
					int dy = original_y - y0;

					// ���͂S�_�̉�f�̐Fc�̋P�x�l�����`���
					rgb_w[c]= img[y1][x1][c] * dy * dx
							+ img[y1][x0][c] * dy * (1.0f - dx)
							+ img[y0][x1][c] * (1.0f - dy) * dx
							+ img[y0][x0][c] * (1.0f - dy) * (1.0f - dx);
				}

				//512�~512�e�N�X�`���Ƀf�[�^������
				UINT64 pos = x * 512 + y;
				p[pos] = (rgb_w[0]) | (rgb_w[1] << 8) | (rgb_w[2] << 16) | (rgb_w[3] << 24);
			}
		}

		delete pBitmap;
	}
	else
	{
		//�e�N�X�`���������Ƃ���0xFF�Ŗ��߂�
		w = 512;
		h = 512;
		memset(&p[0], 0xFF, sizeof(UINT32) * w * h);
	}
	
	//�A�b�v���[�h�p�̃e�N�X�`�����\�[�X�ɉ�f�f�[�^������--------------------
	void* ptr = nullptr;
	resource_up_load->Map(0, nullptr, &ptr);

	//�A�b�v���[�h�p�e�N�X�`�����\�[�X�̃C���[�W���̐擪�A�h���X�擾
	UINT32* p_rgba = (reinterpret_cast<UINT32*>(ptr) + footprint.Offset);

	//GID+����l�������O���t�B�b�N�����A�b�v���[�h�p�e�N�X�`���ɓ����
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


//�o�͗p�e�N�X�`�����\�[�X�r���[���쐬
bool Texture::CreateOutPutTexResourceView(ComPtr<ID3D12Device>& dev,
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT& footprint, UINT64& total_bytes)
{
	//�e�N�X�`���̃��\�[�X�r���[���쐬
	//�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heap_props;
	ZeroMemory(&heap_props, sizeof(heap_props));
	heap_props.Type = D3D12_HEAP_TYPE_DEFAULT;
	heap_props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap_props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heap_props.CreationNodeMask = 1;
	heap_props.VisibleNodeMask = 1;
	//���\�[�X�f�X�N
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
	// ���\�[�X�𐶐�.��D3D12_RESOURCE_STATE_COPY_DEST(�������ݗp���\�[�X)
	if (FAILED(dev->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &resource_desc,
		D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(m_resource.GetAddressOf()))))
		return false;

	//���\�[�X�̐ݒ�擾
	dev->GetCopyableFootprints(&resource_desc, 0, 1, 0, &footprint, nullptr, nullptr, &total_bytes);

	//�f�X�N���v�^�q�[�v�f�X�N				
	D3D12_DESCRIPTOR_HEAP_DESC	descriptor_heap_desc{};
	descriptor_heap_desc.NumDescriptors = 1;
	descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descriptor_heap_desc.NodeMask = 0;

	// �e�N�X�`���[�p�̃f�X�N���v�^�q�[�v���쐬		
	if (FAILED(dev->CreateDescriptorHeap(&descriptor_heap_desc, IID_PPV_ARGS(m_descriptorHeap.GetAddressOf()))))
		return false;

	//�e�N�X�`���[�V�F�[�_�[���\�[�X�r���[�f�X�N
	D3D12_SHADER_RESOURCE_VIEW_DESC	shader_resource_view_desc{};
	shader_resource_view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	shader_resource_view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	shader_resource_view_desc.Texture2D.MipLevels = 1;
	shader_resource_view_desc.Texture2D.MostDetailedMip = 0;
	shader_resource_view_desc.Texture2D.PlaneSlice = 0;
	shader_resource_view_desc.Texture2D.ResourceMinLODClamp = 0.0F;
	shader_resource_view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	//�e�N�X�`���[�V�F�[�_�[���\�[�X�r���[���쐬	
	m_handle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	dev->CreateShaderResourceView(m_resource.Get(), &shader_resource_view_desc, m_handle);

	return true;
}

//�A�b�v���[�h�e�N�X�`�����\�[�X�쐬
bool Texture::CreateUpLoadResource(ComPtr<ID3D12Device>& dev, ID3D12Resource** resource_up_load, UINT64 total_bytes)
{
	//�C���[�W���MAP�p���\�[�X�쐬
	//�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heap_props;
	ZeroMemory(&heap_props, sizeof(heap_props));
	heap_props.Type = D3D12_HEAP_TYPE_UPLOAD;
	heap_props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap_props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heap_props.CreationNodeMask = 1;
	heap_props.VisibleNodeMask = 1;

	//���\�[�X�f�X�N
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

	//�C���[�W����MAP�p���\�[�X�쐬��D3D12_RESOURCE_STATE_GENERIC_READ(�ǂݎ��p���\�[�X)
	dev->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE,
		&desc_resurce, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(resource_up_load));
	return true;
}

//�C���[�W���e�N�X�`���ɓ]��
bool Texture::CopyImageDataTexture(ComPtr<ID3D12Device>& dev,D3D12_PLACED_SUBRESOURCE_FOOTPRINT& footprint, ID3D12Resource* resource_up_load)
{
	//�e�N�X�`���R�s�[�p�R�}���h�쐬
	unique_ptr<COMMAND> copy_com;
	copy_com.reset(new COMMAND(dev, D3D12_COMMAND_LIST_TYPE_COPY));

	//�e�N�X�`���R�s�[�R�}���h���j��
	if (FAILED(copy_com->GetAlr()->Reset()))
		return false;
	if (FAILED(copy_com->GetList()->Reset(copy_com->GetAlr().Get(), nullptr)))
		return false;

	//�e�N�X�`���̈���R�s�[����
	//�]�����iMAP�p���\�[�X�j
	D3D12_TEXTURE_COPY_LOCATION  src;
	ZeroMemory(&src, sizeof(src));
	src.pResource = resource_up_load;
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint = footprint;
	//�]����(�e�N�X�`�����\�[�X)
	D3D12_TEXTURE_COPY_LOCATION  dest;
	ZeroMemory(&dest, sizeof(dest));
	dest.pResource = m_resource.Get();
	dest.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dest.SubresourceIndex = 0;//�e�N�X�`�����z��̏ꍇ�v�f��������
	//�R�}���h�F�e�N�X�`���̈���R�s�[
	copy_com->GetList()->CopyTextureRegion(&dest, 0, 0, 0, &src, nullptr);

	//�R�}���h���s
	copy_com->GetList()->Close();
	ID3D12CommandList* lists[] = { copy_com->GetList().Get() };
	copy_com->GetQueue()->ExecuteCommandLists(1, lists);

	//�t�F���X�̃V�O�i����������0�ɖ߂�
	copy_com->GetFence()->Signal(0);

	//�L���[�̏�ԁu�R�}���h���s���v�u�R�}���h�����v���t�F���X���m��
	//�L���[�̃V�O�i�����P�Ȃ�u�R�}���h�����v�ݒ�A�@0�́u�R�}���h���s���v�Ƀt�F���X�l���ϓ�����
	if (FAILED(copy_com->GetQueue()->Signal(copy_com->GetFence().Get(), 1)))
		return false;

	// ���݂̃t�F���X��0�l�Ȃ�R�}���h���s���̂��߁A�ҋ@������
	if (copy_com->GetFence()->GetCompletedValue() < 1)
	{
		//�C�x���g�Ɂu�R�}���h�����v��������m�点��w��
		if (FAILED(copy_com->GetFence()->SetEventOnCompletion(1, copy_com->GetFenceEvent())))
			return false;
		//�C�x���g�u�R�}���h�����v������܂őҋ@
		WaitForSingleObject(copy_com->GetFenceEvent(), INFINITE);
	}

	//�e�N�X�`���R�s�[�R�}���h���j��
	if (FAILED(copy_com->GetAlr()->Reset()))
		return false;
	if (FAILED(copy_com->GetList()->Reset(copy_com->GetAlr().Get(), nullptr)))
		return false;

	copy_com.reset();
	return true;
}