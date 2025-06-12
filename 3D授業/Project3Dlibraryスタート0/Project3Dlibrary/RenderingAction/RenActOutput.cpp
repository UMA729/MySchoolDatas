#include"RenderAction.h"
extern unique_ptr<DrawPipeLine>			DrawPipe;		//パイプライン

constexpr auto WINDOW_WIDTH{ 800 };
constexpr auto WINDOW_HEIGHT{ 720 };

//アウトプット用
void RenderAction_Output::Action()
{
	//コマンドリセット
	if (FAILED(Com->GetAlr()->Reset()))
		return;
	if (FAILED(Com->GetList()->Reset(Com->GetAlr().Get(), DrawPipe->GetPipe().Get())))
		return;

	//テクスチャポリゴンを描画-------------------------
	//バックバッファへのバリア
	// バックバッファが描画ターゲットとして使用できるようになるまで待つ
	{
		D3D12_RESOURCE_BARRIER	resourceBarrier{};
		resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;// このバリアはリソースの状態遷移に対して設置
		resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		resourceBarrier.Transition.pResource = Render->GetRenderTaget(Render->GetFrameIndex()).Get();
		resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;		 // 遷移前はプレゼン
		resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;	 // 遷移後は描画ターゲット
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
		Com->GetList()->SetGraphicsRootSignature(DrawPipe->GetRoot().Get());
		Com->GetList()->SetPipelineState(DrawPipe->GetPipe().Get());
		//RTVとDSVをクリア
		float clearColor[4]{ 0.2f, 0.45f, 0.8f, 0.0f };
		Com->GetList()->ClearRenderTargetView(Render->GetRenderView(Render->GetFrameIndex()), clearColor, 0, nullptr);
		Com->GetList()->ClearDepthStencilView(Render->GetDepthView(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		//バックバッファRTVとZバッファをレンダーターゲットにセット
		Com->GetList()->OMSetRenderTargets(1, &Render->GetRenderView(Render->GetFrameIndex()), TRUE, &Render->GetDepthView());

		// ポリゴンを描画するプリミティブの種類の設定
		Com->GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// 描画するバーテックスバッファの設定
		Com->GetList()->IASetVertexBuffers(0, 1, &RenderPolygon->m_vertex->m_view);
		// 描画するインデックスバッファの設定
		Com->GetList()->IASetIndexBuffer(&RenderPolygon->m_index->m_view);
		//コンスタントバッファ設定
		Com->GetList()->SetGraphicsRootConstantBufferView(0, m_constant->Getbuf()->GetGPUVirtualAddress());
		//テクスチャを設定
		//使用するデスプリタヒープを設定
		Com->GetList()->SetDescriptorHeaps(1, RenderPolygon->m_render_tex->GetDescriptorHeap().GetAddressOf());
		//デスプリタテーブルに設定
		Com->GetList()->SetGraphicsRootDescriptorTable(1, RenderPolygon->m_render_tex->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
		//設定を元にモデル描き込み
		Com->GetList()->DrawIndexedInstanced(RenderPolygon->m_index->Size(), 1, 0, 0, 0);

	}

	//バックバッファのバリア
	// バックバッファの描画完了を待つためのバリアを設置
	{
		D3D12_RESOURCE_BARRIER	resourceBarrier{};
		resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		resourceBarrier.Transition.pResource = Render->GetRenderTaget(Render->GetFrameIndex()).Get();
		resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	// 遷移前は描画ターゲット
		resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			// 遷移後はプレゼン
		resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		Com->GetList()->ResourceBarrier(1, &resourceBarrier);
	}

	//テクスチャクリア-------------------------
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
		//RTVとDSVをクリア
		float clearColor[4]{ 0.2f, 0.45f, 0.8f, 0.0f };
		Com->GetList()->ClearRenderTargetView(RenderPolygon->m_render_tex->GetTagetHandle(), clearColor, 0, nullptr);
		Com->GetList()->ClearDepthStencilView(RenderPolygon->m_z_buffer->GetHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
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

//フレーム出力
void RenderAction_Output::ActionPresent()
{
	// フレームを最終出力
	if (FAILED(Render->GetSwapChain()->Present(1, 0)))
		return;
	//次回のバックバッファインデックスを渡す
	Render->GetFrameIndex() = Render->GetSwapChain()->GetCurrentBackBufferIndex();
}

