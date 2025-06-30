#pragma once
#include <tchar.h>
#include <memory>
#include <wrl/client.h>	//COM�̃X�}�[�g��
#include <string>
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

#include "../BufferAndImg/Buffer.h"

//------------------------------------------------------
//�f�o�C�XDirectX12�쐬�N���X
class DeviceDX12
{
public:
	DeviceDX12(HWND hWnd);
	~DeviceDX12();

	ComPtr<ID3D12Device>& GetDev() { return m_device; }
	IDXGIFactory4& GetFac() { return *m_factory; }

private:
	ComPtr<ID3D12Device>m_device{ nullptr };			//�f�o�C�X
	IDXGIFactory4*		m_factory{ nullptr };			//�t�@�N�g���[
	IDXGIAdapter1*		m_hardwareAdapter{ nullptr };	//�n�[�h�E�F�A�A�_�v�^�[
};

using  DX12=DeviceDX12;
//------------------------------------------------------

//-------------------------------------------------------
//�`��R�}���h�N���X
class Command
{
public:
	Command(ComPtr<ID3D12Device>& dev, D3D12_COMMAND_LIST_TYPE type);
	~Command();

	ComPtr<ID3D12GraphicsCommandList>&	GetList()		{ return m_commandList; }
	ComPtr<ID3D12CommandAllocator>&		GetAlr()		{ return m_commandAllocator; }
	ComPtr<ID3D12CommandQueue>&			GetQueue()		{ return m_commandQueue; }
	ComPtr<ID3D12Fence>&				GetFence()		{ return m_fence; }
	HANDLE&								GetFenceEvent() { return m_fenceEvent; }

private:
	ComPtr<ID3D12GraphicsCommandList>	m_commandList		{ nullptr };	//�R�}���h���X�g
	ComPtr<ID3D12CommandAllocator>		m_commandAllocator	{ nullptr };	//�R�}���h�A���[�P�[�^
	ComPtr<ID3D12CommandQueue>			m_commandQueue		{ nullptr };	//�R�}���h�L���[

	ComPtr<ID3D12Fence>				m_fence{ nullptr };						//�t�F���X
	HANDLE							m_fenceEvent{ 0 };						//�t�F���X�C�x���g
};

using  COMMAND = Command;
//-------------------------------------------------------
//------------------------------------------------------
//�萔
constexpr int FRAME_COUNT{ 2 };
//�����_�����O�r���[�쐬�N���X
class RenderBuffer
{
public:
	RenderBuffer(ComPtr<ID3D12Device>& dev, ComPtr<ID3D12CommandQueue>& queue,
		IDXGIFactory4& fac,HWND hWnd,int w, int h);
	~RenderBuffer();

	ComPtr<IDXGISwapChain3>& GetSwapChain()				{ return m_swapChain; }
	ComPtr<ID3D12Resource>& GetRenderTaget(int id)		{ return m_renderTargets[id]; }
	D3D12_CPU_DESCRIPTOR_HANDLE& GetRenderView(int id)  { return m_handleRTV[id]; }
	D3D12_CPU_DESCRIPTOR_HANDLE& GetDepthView()			{ return m_z_buffer->GetHandle(); }
	UINT& GetFrameIndex()								{ return m_frameIndex; }

private:
	//�X���b�v�`�F�C��
	ComPtr<IDXGISwapChain3>		m_swapChain{ nullptr };				//�X���b�v�`�F�C��
	UINT						m_frameIndex{ 0 };					//�t���[���C���f�b�N�X
	//�����_�����O���\�[�X�ƃr���[
	ComPtr<ID3D12Resource>		m_renderTargets[FRAME_COUNT]{ nullptr };//���\�[�X			�iGPU��̃��\�[�X�o�b�t�@�j
	ComPtr<ID3D12DescriptorHeap>m_descriptorHeapRTV{ nullptr };			//�f�B�X�v���^�q�[�v�iGPU��̃��\�[�X�A�N�Z�X�j
	D3D12_CPU_DESCRIPTOR_HANDLE	m_handleRTV[FRAME_COUNT]{0, 0};			//�n���h���i�r���[�j�iCPU����f�X�N���v�^�ɃA�N�Z�X�j
	//Z�o�b�t�@���\�[�X�ƃr���[
	unique_ptr<ZBuffer>				m_z_buffer;

	// �f�X�N���v�^�Ƃ́AGPU��������ɑ��݂���l�X�ȃf�[�^��o�b�t�@�̎�ނ�ʒu�A�傫���������\���̂ȃ��m
	// �f�X�N���v�^�����|�C���^��GPU��̃o�b�t�@�f�[�^���A�h���X�i�[���Ă�
	// �f�X�N���v�^�q�[�v�Ƃ́A���̍\���̂�GPU��̔z��ł���B
};
using RENDER = RenderBuffer;
//-------------------------------------------------------

