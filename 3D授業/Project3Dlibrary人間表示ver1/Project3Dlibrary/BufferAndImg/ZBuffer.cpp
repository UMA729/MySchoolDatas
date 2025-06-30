#include "../DrectX12/DX12Library.h"
#include "Buffer.h"

ZBuffer::ZBuffer(ComPtr<ID3D12Device>& dev, int w, int h)
{
	// Z�o�b�t�@�̃f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC descDescriptorHeapDSB;
	ZeroMemory(&descDescriptorHeapDSB, sizeof(descDescriptorHeapDSB));
	descDescriptorHeapDSB.NumDescriptors = 1;
	descDescriptorHeapDSB.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	descDescriptorHeapDSB.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	if (FAILED(dev->CreateDescriptorHeap(&descDescriptorHeapDSB,IID_PPV_ARGS(m_descriptorHeap.GetAddressOf()))))
		return;

	//Z�X�e���V���o�b�t�@�̃��\�[�X�f�X�N
	D3D12_RESOURCE_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	descDepth.Width = w;
	descDepth.Height = h;
	descDepth.DepthOrArraySize = 1;
	descDepth.MipLevels = 0;
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
	descDepth.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	descDepth.SampleDesc.Count = 1;
	descDepth.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//Z�X�e���V���o�b�t�@�̃q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heapProps;
	ZeroMemory(&heapProps, sizeof(heapProps));
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProps.CreationNodeMask = 0;
	heapProps.VisibleNodeMask = 0;

	//Z�X�e���V���N���A�l
	D3D12_CLEAR_VALUE dsvClearValue;
	dsvClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	dsvClearValue.DepthStencil.Depth = 1.0f;
	dsvClearValue.DepthStencil.Stencil = 0;

	//Z�X�e���V�����\�[�X�쐬
	dev->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &descDepth,D3D12_RESOURCE_STATE_DEPTH_WRITE, &dsvClearValue,
		IID_PPV_ARGS(m_resource.ReleaseAndGetAddressOf()));

	//���\�[�X���f�X�N���v�^�q�[�v�ƃn���h�����w���A�A�N�Z�X�ł���悤�Ƀr���[��������
	D3D12_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.Texture2D.MipSlice = 0;
	dev->CreateDepthStencilView(m_resource.Get(), &descDSV,m_descriptorHeap->GetCPUDescriptorHandleForHeapStart());
	m_handle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();

}