#include "Buffer.h"

//�o�b�t�@���\�[�X�쐬
void StructBuffer::CreateResource(ComPtr<ID3D12Device>& dev, size_t size)
{
	//�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES heapProps;
	ZeroMemory(&heapProps, sizeof(heapProps));
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProps.CreationNodeMask = 0;
	heapProps.VisibleNodeMask = 0;
	//���\�[�X�f�X�N
	D3D12_RESOURCE_DESC   descResource;
	ZeroMemory(&descResource, sizeof(descResource));
	descResource.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	descResource.Width = size;
	descResource.Height = 1;
	descResource.DepthOrArraySize = 1;
	descResource.MipLevels = 1;
	descResource.Format = DXGI_FORMAT_UNKNOWN;
	descResource.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	descResource.SampleDesc.Count = 1;
	//�o�b�t�@���\�[�X�쐬
	if (FAILED(dev->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &descResource,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_resource))))
		return;
	return;
}

//�o�b�t�@�փf�[�^�A�b�v���[�h
void StructBuffer::UpDate(size_t size, void* data)
{
	//�o�b�t�@�ɒ��_�f�[�^����������
	void* map = nullptr;
	m_resource->Map(0, nullptr, &map);
	memcpy(map, data, size);
	m_resource->Unmap(0, nullptr);
}
