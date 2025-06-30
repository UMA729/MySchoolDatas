#include "DX12Library.h"

//LIB�̓o�^
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")


//�R���X�g���N�^
DeviceDX12::DeviceDX12(HWND hWnd)
{
	ID3D12Debug* debug = nullptr;
	UINT flagsDXGI = 0;

	//�f�o�b�N�� DirectX12�̃f�o�b�O���C���[��L���ɂ���
#ifdef _DEBUG
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	if (debug)
	{
		debug->EnableDebugLayer();
		debug->Release();
		debug = nullptr;
	}
	flagsDXGI |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	// DirectX12���T�|�[�g���闘�p�\�ȃn�[�h�E�F�A�A�_�v�^�擾
	if (FAILED(CreateDXGIFactory2(flagsDXGI, IID_PPV_ARGS(&m_factory))))
		return ;

	//factory�̃n�[�h�E�F�A�A�_�v�^��DirectX12�ɑΉ����Ă��邩���m�F
	IDXGIAdapter1* adapter{ nullptr };
	for (UINT i = 0; m_factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			continue;

		// �A�_�v�^��DirectX12�ɑΉ����Ă��邩�m�F�m�F�̂��߃f�o�C�X���쐬
		if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			break;
	}
	m_hardwareAdapter = adapter;

	//�f�o�C�X�̍쐬
	if (FAILED(D3D12CreateDevice(m_hardwareAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device))))
		return ;

	//�O���t�B�b�N�{�[�h�T�|�[�g�`�F�b�N
	D3D12_FEATURE_DATA_D3D12_OPTIONS options{ 0 };
	if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, (void*)&options, sizeof(options))))
		return ;

	// �t���X�N���[���̃T�|�[�g�Ȃ�
	if (FAILED(m_factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER)))
		return ;


	return ;
}

//�f�X�g���N�^
DeviceDX12::~DeviceDX12()
{
	m_factory->Release();
	m_hardwareAdapter->Release();
	m_device.Reset();
}
