#include"RenderAction.h"
#include "../ModelStructs/3DcubeModel.h"

extern unique_ptr<Cube3dDrawPipeLine>	CubePipe;		//キューブパイプライン
extern unique_ptr<Model_3dCube> Cuble3D;				//モデル＿キューブ

constexpr auto WINDOW_WIDTH{ 800 };
constexpr auto WINDOW_HEIGHT{ 720 };

//３Dレンダリング用
void RenderAction_3D_Cube::Action()
{
	//コマンドリセット
	if (FAILED(Com->GetAlr()->Reset()))
		return;
	if (FAILED(Com->GetList()->Reset(Com->GetAlr().Get(), CubePipe->GetPipe().Get())))
		return;

	//リソースバリア
	{
		D3D12_RESOURCE_BARRIER	resourceBarrier{};
		resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;// このバリアはリソースの状態遷移に対して設置
		resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		resourceBarrier.Transition.pResource = RenderPolygon->m_render_tex->GetResource().Get();
		resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		Com->GetList()->ResourceBarrier(1, &resourceBarrier);
	}

	//描画内容
	{
		//ビューポート設定(レンダーバッファのどの領域に書き込むのか)
		D3D12_VIEWPORT	viewport{ 0.0f, 0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, 1.0f };
		Com->GetList()->RSSetViewports(1, &viewport);
		//シザー領域( Viewport 変換した後のどの部分に張り付けるか)
		D3D12_RECT		scissorRect{ 0, 0, WINDOW_WIDTH,WINDOW_HEIGHT };
		Com->GetList()->RSSetScissorRects(1, &scissorRect);
		//ルートシグネチャ・パイプライン設定
		Com->GetList()->SetGraphicsRootSignature(CubePipe->GetRoot().Get());
		Com->GetList()->SetPipelineState(CubePipe->GetPipe().Get());
		//バックバッファRTVとZバッファをレンダーターゲットにセット
		Com->GetList()->OMSetRenderTargets(1, &RenderPolygon->m_render_tex->GetTagetHandle(), TRUE, &RenderPolygon->m_z_buffer->GetHandle());
	

		// ビュー・プロジェクション・ワールド行列
		m_r += 0.1f;
		XMMATRIX view = XMMatrixLookAtLH(XMVECTOR{ 0.0f, 100.0f, -300.5f }, XMVECTOR{ 0.0,50.0f,0.0f }, XMVECTOR{ 0.0f, 1.0f, 0.0f });
		XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PI / 3.0f, float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 500.f);
		XMMATRIX world = XMMatrixIdentity();
		world *= XMMatrixRotationY(m_r);
		world *= XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
		XMMATRIX wvp = XMMatrixMultiply(XMMatrixMultiply(world, view), proj);

		//コンスタントバッファに登録
		void* mappedCB;
		m_constant->Getbuf()->Map(0, nullptr, &mappedCB);
		XMFLOAT4X4 m;
		XMStoreFloat4x4(&m, wvp);
		memcpy(mappedCB, &m, sizeof(m));
		m_constant->Getbuf()->Unmap(0, nullptr);
		//コンスタントバッファ設定
		Com->GetList()->SetGraphicsRootConstantBufferView(0, m_constant->Getbuf()->GetGPUVirtualAddress());

		//マテリアルバッファ設定
		int id = Cuble3D->m_materialId;
		Com->GetList()->SetGraphicsRootConstantBufferView(1, Cuble3D->m_model_mat[id]->Getbuf()->GetGPUVirtualAddress());

		////テクスチャを設定
		////使用するデスプリタヒープを設定
		Com->GetList()->SetDescriptorHeaps(1, Cuble3D->m_tex->GetDescriptorHeap().GetAddressOf());
		////デスプリタテーブルに設定
		Com->GetList()->SetGraphicsRootDescriptorTable(2, Cuble3D->m_tex->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());


	
		// ポリゴンを描画するプリミティブの種類の設定
		Com->GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// 描画するバーテックスバッファの設定
		Com->GetList()->IASetVertexBuffers(0, 1, &Cuble3D->m_vertex->m_view);
		// 描画するインデックスバッファの設定
		Com->GetList()->IASetIndexBuffer(&Cuble3D->m_index->m_view);
		//設定を元にモデル描き込み
		Com->GetList()->DrawIndexedInstanced(Cuble3D->m_index->Size(), 1, 0, 0, 0);
	
		
	}

	//リソースバリア
	{
		D3D12_RESOURCE_BARRIER	resourceBarrier{};
		resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		resourceBarrier.Transition.pResource = RenderPolygon->m_render_tex->GetResource().Get();
		resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		Com->GetList()->ResourceBarrier(1, &resourceBarrier);
	}

	//コマンドの入力完了
	Com->GetList()->Close();
}
