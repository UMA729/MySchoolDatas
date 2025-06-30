#include"RenderAction.h"
extern unique_ptr<DrawPipeLine>			DrawPipe;		//�p�C�v���C��

constexpr auto WINDOW_WIDTH{ 800 };
constexpr auto WINDOW_HEIGHT{ 720 };

//�A�E�g�v�b�g�p
void RenderAction_Output::Action()
{
	//�R�}���h���Z�b�g
	if (FAILED(Com->GetAlr()->Reset()))
		return;
	if (FAILED(Com->GetList()->Reset(Com->GetAlr().Get(), DrawPipe->GetPipe().Get())))
		return;

	//�e�N�X�`���|���S����`��-------------------------
	//�o�b�N�o�b�t�@�ւ̃o���A
	// �o�b�N�o�b�t�@���`��^�[�Q�b�g�Ƃ��Ďg�p�ł���悤�ɂȂ�܂ő҂�
	{
		D3D12_RESOURCE_BARRIER	resourceBarrier{};
		resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;// ���̃o���A�̓��\�[�X�̏�ԑJ�ڂɑ΂��Đݒu
		resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		resourceBarrier.Transition.pResource = Render->GetRenderTaget(Render->GetFrameIndex()).Get();
		resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;		 // �J�ڑO�̓v���[��
		resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;	 // �J�ڌ�͕`��^�[�Q�b�g
		resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		Com->GetList()->ResourceBarrier(1, &resourceBarrier);
	}

	//�`����e
	{
		//�r���[�|�[�g�ݒ�(�����_�[�o�b�t�@�̂ǂ̗̈�ɏ������ނ̂�)
		D3D12_VIEWPORT	viewport{ 0.0f, 0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, 1.0f };
		Com->GetList()->RSSetViewports(1, &viewport);
		//�V�U�[�̈�( Viewport �ϊ�������̂ǂ̕����ɒ���t���邩)
		D3D12_RECT		scissorRect{ 0, 0, WINDOW_WIDTH,WINDOW_HEIGHT };
		Com->GetList()->RSSetScissorRects(1, &scissorRect);
		//���[�g�V�O�l�`���E�p�C�v���C���ݒ�
		Com->GetList()->SetGraphicsRootSignature(DrawPipe->GetRoot().Get());
		Com->GetList()->SetPipelineState(DrawPipe->GetPipe().Get());
		//RTV��DSV���N���A
		float clearColor[4]{ 0.2f, 0.45f, 0.8f, 0.0f };
		Com->GetList()->ClearRenderTargetView(Render->GetRenderView(Render->GetFrameIndex()), clearColor, 0, nullptr);
		Com->GetList()->ClearDepthStencilView(Render->GetDepthView(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		//�o�b�N�o�b�t�@RTV��Z�o�b�t�@�������_�[�^�[�Q�b�g�ɃZ�b�g
		Com->GetList()->OMSetRenderTargets(1, &Render->GetRenderView(Render->GetFrameIndex()), TRUE, &Render->GetDepthView());

		// �|���S����`�悷��v���~�e�B�u�̎�ނ̐ݒ�
		Com->GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// �`�悷��o�[�e�b�N�X�o�b�t�@�̐ݒ�
		Com->GetList()->IASetVertexBuffers(0, 1, &RenderPolygon->m_vertex->m_view);
		// �`�悷��C���f�b�N�X�o�b�t�@�̐ݒ�
		Com->GetList()->IASetIndexBuffer(&RenderPolygon->m_index->m_view);
		//�R���X�^���g�o�b�t�@�ݒ�
		Com->GetList()->SetGraphicsRootConstantBufferView(0, m_constant->Getbuf()->GetGPUVirtualAddress());
		//�e�N�X�`����ݒ�
		//�g�p����f�X�v���^�q�[�v��ݒ�
		Com->GetList()->SetDescriptorHeaps(1, RenderPolygon->m_render_tex->GetDescriptorHeap().GetAddressOf());
		//�f�X�v���^�e�[�u���ɐݒ�
		Com->GetList()->SetGraphicsRootDescriptorTable(1, RenderPolygon->m_render_tex->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
		//�ݒ�����Ƀ��f���`������
		Com->GetList()->DrawIndexedInstanced(RenderPolygon->m_index->Size(), 1, 0, 0, 0);

	}

	//�o�b�N�o�b�t�@�̃o���A
	// �o�b�N�o�b�t�@�̕`�抮����҂��߂̃o���A��ݒu
	{
		D3D12_RESOURCE_BARRIER	resourceBarrier{};
		resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		resourceBarrier.Transition.pResource = Render->GetRenderTaget(Render->GetFrameIndex()).Get();
		resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	// �J�ڑO�͕`��^�[�Q�b�g
		resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			// �J�ڌ�̓v���[��
		resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		Com->GetList()->ResourceBarrier(1, &resourceBarrier);
	}

	//�e�N�X�`���N���A-------------------------
	//���\�[�X�o���A
	{
		D3D12_RESOURCE_BARRIER	resourceBarrier{};
		resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;// ���̃o���A�̓��\�[�X�̏�ԑJ�ڂɑ΂��Đݒu
		resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		resourceBarrier.Transition.pResource = RenderPolygon->m_render_tex->GetResource().Get();
		resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		Com->GetList()->ResourceBarrier(1, &resourceBarrier);
	}

	//�`����e
	{
		//RTV��DSV���N���A
		float clearColor[4]{ 0.2f, 0.45f, 0.8f, 0.0f };
		Com->GetList()->ClearRenderTargetView(RenderPolygon->m_render_tex->GetTagetHandle(), clearColor, 0, nullptr);
		Com->GetList()->ClearDepthStencilView(RenderPolygon->m_z_buffer->GetHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}

	//���\�[�X�o���A
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
		
	//�R�}���h�̓��͊���
	Com->GetList()->Close();
}

//�t���[���o��
void RenderAction_Output::ActionPresent()
{
	// �t���[�����ŏI�o��
	if (FAILED(Render->GetSwapChain()->Present(1, 0)))
		return;
	//����̃o�b�N�o�b�t�@�C���f�b�N�X��n��
	Render->GetFrameIndex() = Render->GetSwapChain()->GetCurrentBackBufferIndex();
}

