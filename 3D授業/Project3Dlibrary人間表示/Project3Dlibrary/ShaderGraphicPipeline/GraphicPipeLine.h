#pragma once
#include "../DrectX12/DX12Library.h"

//-------------------------------------------------------
//パイプラインクラス
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
	ComPtr<ID3D12PipelineState>	m_pipelineState;//パイプライン
	ComPtr<ID3D12RootSignature>	m_rootSignature;//ルートシグネチャ
};
//-------------------------------------------------------

//-------------------------------------------------------
//ドローシェーダパイプラインクラス
class DrawPipeLine :public PipeLine
{
public:
	DrawPipeLine(ComPtr<ID3D12Device>& dev);
	~DrawPipeLine() {};
private:
};
//-------------------------------------------------------
//-------------------------------------------------------
//キューブ３Dシェーダパイプラインクラス
class Cube3dDrawPipeLine :public PipeLine
{
public:
	Cube3dDrawPipeLine(ComPtr<ID3D12Device>& dev);
	~Cube3dDrawPipeLine() {};
private:
};
//-------------------------------------------------------
