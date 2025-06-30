#pragma once
#include "../DrectX12/DX12Library.h"

//-------------------------------------------------------
//�p�C�v���C���N���X
class PipeLine
{
public:
	PipeLine() {};
	~PipeLine()
	{
		m_pipelineState.Reset();
		m_rootSignature.Reset();
	};

	ComPtr<ID3D12PipelineState>& GetPipe() { return m_pipelineState; }
	ComPtr<ID3D12RootSignature>& GetRoot() { return m_rootSignature; }
protected:
	ComPtr<ID3D12PipelineState>	m_pipelineState;//�p�C�v���C��
	ComPtr<ID3D12RootSignature>	m_rootSignature;//���[�g�V�O�l�`��
};
//-------------------------------------------------------

//-------------------------------------------------------
//�h���[�V�F�[�_�p�C�v���C���N���X
class DrawPipeLine :public PipeLine
{
public:
	DrawPipeLine(ComPtr<ID3D12Device>& dev);
	~DrawPipeLine() {};
private:
};
//-------------------------------------------------------
//-------------------------------------------------------
//�L���[�u�RD�V�F�[�_�p�C�v���C���N���X
class Cube3dDrawPipeLine :public PipeLine
{
public:
	Cube3dDrawPipeLine(ComPtr<ID3D12Device>& dev);
	~Cube3dDrawPipeLine() {};
private:
};
//-------------------------------------------------------
