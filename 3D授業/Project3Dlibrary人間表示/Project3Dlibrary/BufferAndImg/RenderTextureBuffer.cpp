#include "Buffer.h"

RenderTexture::RenderTexture(ComPtr<ID3D12Device>& dev, int w, int h)
{
	//�����_�����O�e�N�X�`���̃��\�[�X------------------------------------------
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
	resource_desc.Width = w;
	resource_desc.Height = h;
	resource_desc.DepthOrArraySize = 1;
	resource_desc.MipLevels = 1;
	resource_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resource_desc.SampleDesc.Count = 1;
	resource_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;//�����_�����O�^�[�Q�b�g���\�[�X
	resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	//�x�[�X�N���A�F�̐ݒ�
	D3D12_CLEAR_VALUE cv{};
	cv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	cv.Color[0] = 0.2f; cv.Color[1] = 0.45f; cv.Color[2] = 0.8f; cv.Color[3] = 0.0f;
	
	//���\�[�X�쐬�i���\�[�X�o���A�F�s�N�Z���V�F�[�_�[�Ŏg�p�j
	if (FAILED(dev->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &resource_desc,
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&cv, IID_PPV_ARGS(m_resource.GetAddressOf()))))
		return;
	//-------------------------------------------------------------------------

	//�����_�����O�^�[�Q�b�g�r���[---------------------------------------------
	//�f�X�N���v�^�q�[�v�f�X�N				
	D3D12_DESCRIPTOR_HEAP_DESC	descriptor_heap_desc{};
	ZeroMemory(&descriptor_heap_desc, sizeof(descriptor_heap_desc));
	descriptor_heap_desc.NumDescriptors = 1;
	descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	// �^�[�Q�b�g�p�̃f�X�N���v�^�q�[�v���쐬		
	if (FAILED(dev->CreateDescriptorHeap(&descriptor_heap_desc,
		IID_PPV_ARGS(m_taget_descriptorHeap.ReleaseAndGetAddressOf()))))
		return;
	m_taget_handle = m_taget_descriptorHeap->GetCPUDescriptorHandleForHeapStart();

	//�����_�����O�^�[�Q�b�g�r���[�쐬(�n���h���������_�����O�^�[�Q�b�g�r���[��������)
	D3D12_RENDER_TARGET_VIEW_DESC taget_view_decs{};
	taget_view_decs.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	taget_view_decs.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dev->CreateRenderTargetView(m_resource.Get(), &taget_view_decs, m_taget_handle);
	//-------------------------------------------------------------------------

	//�e�N�X�`���[�V�F�[�_�[���\�[�X�r���[-------------------------------------
	//�f�X�N���v�^�q�[�v�f�X�N				
	ZeroMemory(&descriptor_heap_desc, sizeof(descriptor_heap_desc));
	descriptor_heap_desc.NumDescriptors = 1;
	descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	// �^�[�Q�b�g�p�̃f�X�N���v�^�q�[�v���쐬		
	if (FAILED(dev->CreateDescriptorHeap(&descriptor_heap_desc,
		IID_PPV_ARGS(m_descriptorHeap.ReleaseAndGetAddressOf()))))
		return;
	m_handle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();

	//�e�N�X�`���[�V�F�[�_�[���\�[�X�r���[�f�X�N
	D3D12_SHADER_RESOURCE_VIEW_DESC	shader_resource_view_desc{};
	shader_resource_view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	shader_resource_view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	shader_resource_view_desc.Texture2D.MipLevels = 1;
	shader_resource_view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//�e�N�X�`���[�V�F�[�_�[���\�[�X�r���[�쐬�i�n���h�����V�F�[�_�[���\�[�X�r���[��������j	
	dev->CreateShaderResourceView(m_resource.Get(), &shader_resource_view_desc, m_handle);
	//-------------------------------------------------------------------------

	return;
}

