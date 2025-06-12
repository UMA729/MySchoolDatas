#include "Buffer.h"

RenderTexture::RenderTexture(ComPtr<ID3D12Device>& dev, int w, int h)
{
	//レンダリングテクスチャのリソース------------------------------------------
	//ヒーププロパティ
	D3D12_HEAP_PROPERTIES heap_props;
	ZeroMemory(&heap_props, sizeof(heap_props));
	heap_props.Type = D3D12_HEAP_TYPE_DEFAULT;
	heap_props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap_props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heap_props.CreationNodeMask = 1;
	heap_props.VisibleNodeMask = 1;
	//リソースデスク
	D3D12_RESOURCE_DESC   resource_desc;
	ZeroMemory(&resource_desc, sizeof(resource_desc));
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resource_desc.Width = w;
	resource_desc.Height = h;
	resource_desc.DepthOrArraySize = 1;
	resource_desc.MipLevels = 1;
	resource_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	resource_desc.SampleDesc.Count = 1;
	resource_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;//レンダリングターゲットリソース
	resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	//ベースクリア色の設定
	D3D12_CLEAR_VALUE cv{};
	cv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	cv.Color[0] = 0.2f; cv.Color[1] = 0.45f; cv.Color[2] = 0.8f; cv.Color[3] = 0.0f;
	
	//リソース作成（リソースバリア：ピクセルシェーダーで使用）
	if (FAILED(dev->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &resource_desc,
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&cv, IID_PPV_ARGS(m_resource.GetAddressOf()))))
		return;
	//-------------------------------------------------------------------------

	//レンダリングターゲットビュー---------------------------------------------
	//デスクリプタヒープデスク				
	D3D12_DESCRIPTOR_HEAP_DESC	descriptor_heap_desc{};
	ZeroMemory(&descriptor_heap_desc, sizeof(descriptor_heap_desc));
	descriptor_heap_desc.NumDescriptors = 1;
	descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	// ターゲット用のデスクリプタヒープを作成		
	if (FAILED(dev->CreateDescriptorHeap(&descriptor_heap_desc,
		IID_PPV_ARGS(m_taget_descriptorHeap.ReleaseAndGetAddressOf()))))
		return;
	m_taget_handle = m_taget_descriptorHeap->GetCPUDescriptorHandleForHeapStart();

	//レンダリングターゲットビュー作成(ハンドルをレンダリングターゲットビュー化させる)
	D3D12_RENDER_TARGET_VIEW_DESC taget_view_decs{};
	taget_view_decs.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	taget_view_decs.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dev->CreateRenderTargetView(m_resource.Get(), &taget_view_decs, m_taget_handle);
	//-------------------------------------------------------------------------

	//テクスチャーシェーダーリソースビュー-------------------------------------
	//デスクリプタヒープデスク				
	ZeroMemory(&descriptor_heap_desc, sizeof(descriptor_heap_desc));
	descriptor_heap_desc.NumDescriptors = 1;
	descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	// ターゲット用のデスクリプタヒープを作成		
	if (FAILED(dev->CreateDescriptorHeap(&descriptor_heap_desc,
		IID_PPV_ARGS(m_descriptorHeap.ReleaseAndGetAddressOf()))))
		return;
	m_handle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();

	//テクスチャーシェーダーリソースビューデスク
	D3D12_SHADER_RESOURCE_VIEW_DESC	shader_resource_view_desc{};
	shader_resource_view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	shader_resource_view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	shader_resource_view_desc.Texture2D.MipLevels = 1;
	shader_resource_view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//テクスチャーシェーダーリソースビュー作成（ハンドルをシェーダーリソースビュー化させる）	
	dev->CreateShaderResourceView(m_resource.Get(), &shader_resource_view_desc, m_handle);
	//-------------------------------------------------------------------------

	return;
}

