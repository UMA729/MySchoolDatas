#include "GraphicPipeLine.h"


Cube3dDrawPipeLine::Cube3dDrawPipeLine(ComPtr<ID3D12Device>& dev)
{
	//グラフィックパイプライン作成
	// グラフィックスパイプラインのステータスオブジェクトを作成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC	psoDesc{};

	// シェーダーをコンパイル
	ComPtr<ID3DBlob>	vertexShader, pixelShader, RootSignature;

#if defined(_DEBUG)
	// グラフィックデバッグツールによるシェーダーのデバッグを有効にする
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT compileFlags = 0;
#endif

	wstring shader_name{ L"ShaderGraphicPipeline\\Cube3DShader.hlsl" };

	//頂点シェーダのデータ設定
	//HLSLで書かれたヴァーテックスシェーダのコンパイル
	if (FAILED(D3DCompileFromFile(shader_name.c_str(), nullptr, nullptr, "VSMain", "vs_5_1", compileFlags, 0, &vertexShader, nullptr)))
		return;
	D3D12_SHADER_BYTECODE	shaderBytecodeVs;
	shaderBytecodeVs.pShaderBytecode = vertexShader->GetBufferPointer();
	shaderBytecodeVs.BytecodeLength = vertexShader->GetBufferSize();
	psoDesc.VS = shaderBytecodeVs;//頂点シェーダ登録


	//ピクセルシェーダのデータ設定
	//HLSLで書かれたピクセルシェーダのコンパイル
	if (FAILED(D3DCompileFromFile(shader_name.c_str(), nullptr, nullptr, "PSMain", "ps_5_1", compileFlags, 0, &pixelShader, nullptr)))
		return;
	D3D12_SHADER_BYTECODE	shaderBytecodePs;
	shaderBytecodePs.pShaderBytecode = pixelShader->GetBufferPointer();
	shaderBytecodePs.BytecodeLength = pixelShader->GetBufferSize();
	psoDesc.PS = shaderBytecodePs;//ピクセルシェーダ設定


	//ルートサイン設定
	//シェーダーからルートサイン情報を取得
	D3DGetBlobPart(vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &RootSignature);
	dev->CreateRootSignature(0, RootSignature->GetBufferPointer(), RootSignature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));//ルートサインの生成
	psoDesc.pRootSignature = m_rootSignature.Get();//ルートサイン登録


	// 頂点入力レイアウトを定義
	const D3D12_INPUT_ELEMENT_DESC inputElementDescs[]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEX_UV"  , 0, DXGI_FORMAT_R32G32_FLOAT   , 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};
	psoDesc.InputLayout = { inputElementDescs,  _countof(inputElementDescs) };// 頂点入力レイアウト登録


	//ラスタライズ設定
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
	psoDesc.RasterizerState = rasterizerDesc;//ラスタライズ登録

	//ブレンド設定
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
	psoDesc.BlendState = blendDesc;//ブレンド登録

	//残りの登録
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.DepthEnable = TRUE;		//Zバッファ設定(有効)
	psoDesc.DepthStencilState.StencilEnable = FALSE;	//ステンシルバッファ設定（無効）
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;//プリミティブを面に設定
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;

	// グラフィックスパイプラインの作成
	if (FAILED(dev->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState))))
		return;

	return;
}
