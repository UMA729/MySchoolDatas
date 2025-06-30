#include"RenderAction.h"
#include "../ModelStructs/3DcubeModel.h"

extern unique_ptr<Cube3dDrawPipeLine>	CubePipe;		//�L���[�u�p�C�v���C��
extern unique_ptr<Model_3dCube> Cuble3D;				//���f���Q�L���[�u

constexpr auto WINDOW_WIDTH{ 800 };
constexpr auto WINDOW_HEIGHT{ 720 };

//�RD�����_�����O�p
void RenderAction_3D_Cube::Action()
{
	//�R�}���h���Z�b�g
	if (FAILED(Com->GetAlr()->Reset()))
		return;
	if (FAILED(Com->GetList()->Reset(Com->GetAlr().Get(), CubePipe->GetPipe().Get())))
		return;

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
		//�r���[�|�[�g�ݒ�(�����_�[�o�b�t�@�̂ǂ̗̈�ɏ������ނ̂�)
		D3D12_VIEWPORT	viewport{ 0.0f, 0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, 1.0f };
		Com->GetList()->RSSetViewports(1, &viewport);
		//�V�U�[�̈�( Viewport �ϊ�������̂ǂ̕����ɒ���t���邩)
		D3D12_RECT		scissorRect{ 0, 0, WINDOW_WIDTH,WINDOW_HEIGHT };
		Com->GetList()->RSSetScissorRects(1, &scissorRect);
		//���[�g�V�O�l�`���E�p�C�v���C���ݒ�
		Com->GetList()->SetGraphicsRootSignature(CubePipe->GetRoot().Get());
		Com->GetList()->SetPipelineState(CubePipe->GetPipe().Get());
		//�o�b�N�o�b�t�@RTV��Z�o�b�t�@�������_�[�^�[�Q�b�g�ɃZ�b�g
		Com->GetList()->OMSetRenderTargets(1, &RenderPolygon->m_render_tex->GetTagetHandle(), TRUE, &RenderPolygon->m_z_buffer->GetHandle());
	

		// �r���[�E�v���W�F�N�V�����E���[���h�s��
		m_r += 0.1f;
		XMMATRIX view = XMMatrixLookAtLH(XMVECTOR{ 0.0f, 100.0f, -300.5f }, XMVECTOR{ 0.0,50.0f,0.0f }, XMVECTOR{ 0.0f, 1.0f, 0.0f });
		XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PI / 3.0f, float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 500.f);
		XMMATRIX world = XMMatrixIdentity();
		world *= XMMatrixRotationY(m_r);
		world *= XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
		XMMATRIX wvp = XMMatrixMultiply(XMMatrixMultiply(world, view), proj);

		//�R���X�^���g�o�b�t�@�ɓo�^
		void* mappedCB;
		m_constant->Getbuf()->Map(0, nullptr, &mappedCB);
		XMFLOAT4X4 m;
		XMStoreFloat4x4(&m, wvp);
		memcpy(mappedCB, &m, sizeof(m));
		m_constant->Getbuf()->Unmap(0, nullptr);
		//�R���X�^���g�o�b�t�@�ݒ�
		Com->GetList()->SetGraphicsRootConstantBufferView(0, m_constant->Getbuf()->GetGPUVirtualAddress());

		//�}�e���A���o�b�t�@�ݒ�
		int id = Cuble3D->m_materialId;
		Com->GetList()->SetGraphicsRootConstantBufferView(1, Cuble3D->m_model_mat[id]->Getbuf()->GetGPUVirtualAddress());

		////�e�N�X�`����ݒ�
		////�g�p����f�X�v���^�q�[�v��ݒ�
		Com->GetList()->SetDescriptorHeaps(1, Cuble3D->m_tex->GetDescriptorHeap().GetAddressOf());
		////�f�X�v���^�e�[�u���ɐݒ�
		Com->GetList()->SetGraphicsRootDescriptorTable(2, Cuble3D->m_tex->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());


	
		// �|���S����`�悷��v���~�e�B�u�̎�ނ̐ݒ�
		Com->GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// �`�悷��o�[�e�b�N�X�o�b�t�@�̐ݒ�
		Com->GetList()->IASetVertexBuffers(0, 1, &Cuble3D->m_vertex->m_view);
		// �`�悷��C���f�b�N�X�o�b�t�@�̐ݒ�
		Com->GetList()->IASetIndexBuffer(&Cuble3D->m_index->m_view);
		//�ݒ�����Ƀ��f���`������
		Com->GetList()->DrawIndexedInstanced(Cuble3D->m_index->Size(), 1, 0, 0, 0);
	
		
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
