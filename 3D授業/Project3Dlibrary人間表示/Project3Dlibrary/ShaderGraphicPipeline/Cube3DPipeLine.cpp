#include "GraphicPipeLine.h"


Cube3dDrawPipeLine::Cube3dDrawPipeLine(ComPtr<ID3D12Device>& dev)
{
	//�O���t�B�b�N�p�C�v���C���쐬
	// �O���t�B�b�N�X�p�C�v���C���̃X�e�[�^�X�I�u�W�F�N�g���쐬
	D3D12_GRAPHICS_PIPELINE_STATE_DESC	psoDesc{};

	// �V�F�[�_�[���R���p�C��
	ComPtr<ID3DBlob>	vertexShader, pixelShader, RootSignature;

#if defined(_DEBUG)
	// �O���t�B�b�N�f�o�b�O�c�[���ɂ��V�F�[�_�[�̃f�o�b�O��L���ɂ���
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT compileFlags = 0;
#endif

	wstring shader_name{ L"ShaderGraphicPipeline\\Cube3DShader.hlsl" };

	//���_�V�F�[�_�̃f�[�^�ݒ�
	//HLSL�ŏ����ꂽ���@�[�e�b�N�X�V�F�[�_�̃R���p�C��
	if (FAILED(D3DCompileFromFile(shader_name.c_str(), nullptr, nullptr, "VSMain", "vs_5_1", compileFlags, 0, &vertexShader, nullptr)))
		return;
	D3D12_SHADER_BYTECODE	shaderBytecodeVs;
	shaderBytecodeVs.pShaderBytecode = vertexShader->GetBufferPointer();
	shaderBytecodeVs.BytecodeLength = vertexShader->GetBufferSize();
	psoDesc.VS = shaderBytecodeVs;//���_�V�F�[�_�o�^


	//�s�N�Z���V�F�[�_�̃f�[�^�ݒ�
	//HLSL�ŏ����ꂽ�s�N�Z���V�F�[�_�̃R���p�C��
	if (FAILED(D3DCompileFromFile(shader_name.c_str(), nullptr, nullptr, "PSMain", "ps_5_1", compileFlags, 0, &pixelShader, nullptr)))
		return;
	D3D12_SHADER_BYTECODE	shaderBytecodePs;
	shaderBytecodePs.pShaderBytecode = pixelShader->GetBufferPointer();
	shaderBytecodePs.BytecodeLength = pixelShader->GetBufferSize();
	psoDesc.PS = shaderBytecodePs;//�s�N�Z���V�F�[�_�ݒ�


	//���[�g�T�C���ݒ�
	//�V�F�[�_�[���烋�[�g�T�C�������擾
	D3DGetBlobPart(vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &RootSignature);
	dev->CreateRootSignature(0, RootSignature->GetBufferPointer(), RootSignature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));//���[�g�T�C���̐���
	psoDesc.pRootSignature = m_rootSignature.Get();//���[�g�T�C���o�^


	// ���_���̓��C�A�E�g���`
	const D3D12_INPUT_ELEMENT_DESC inputElementDescs[]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEX_UV"  , 0, DXGI_FORMAT_R32G32_FLOAT   , 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};
	psoDesc.InputLayout = { inputElementDescs,  _countof(inputElementDescs) };// ���_���̓��C�A�E�g�o�^


	//���X�^���C�Y�ݒ�
	D3D12_RASTERIZER_DESC	rasterizerDesc{};
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;//D3D12_CULL_MODE_BACK;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	psoDesc.RasterizerState = rasterizerDesc;//���X�^���C�Y�o�^

	//�u�����h�ݒ�
	D3D12_BLEND_DESC	blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
	{
		blendDesc.RenderTarget[i].BlendEnable = TRUE;
		blendDesc.RenderTarget[i].LogicOpEnable = FALSE;
		blendDesc.RenderTarget[i].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[i].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;
		blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	}
	psoDesc.BlendState = blendDesc;//�u�����h�o�^

	//�c��̓o�^
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.DepthEnable = TRUE;		//Z�o�b�t�@�ݒ�(�L��)
	psoDesc.DepthStencilState.StencilEnable = FALSE;	//�X�e���V���o�b�t�@�ݒ�i�����j
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;//�v���~�e�B�u��ʂɐݒ�
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;

	// �O���t�B�b�N�X�p�C�v���C���̍쐬
	if (FAILED(dev->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState))))
		return;

	return;
}
