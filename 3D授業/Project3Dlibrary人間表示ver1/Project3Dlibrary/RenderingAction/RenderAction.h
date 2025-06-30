#pragma once
#include <thread>
#include <atomic>

#include "../DrectX12/DX12Library.h"
#include "../ShaderGraphicPipeline/GraphicPipeLine.h"
#include "../ModelStructs/RenPolygonModel.h"

extern unique_ptr<Model_RenderPolygon>  RenderPolygon;//���f���Q�����_�����O�e�N�X�`���`��|���S��
extern unique_ptr<RENDER>				Render;		  //�����_�����O�o�b�t�@

//�����_�����O�A�N�V����--------------------------------------------
class RenderAction
{
public:
	//�R���X�g���N�^
	RenderAction(ComPtr<ID3D12Device>& dev)
	{
		m_constant.reset(new Constant(dev));
		Com.reset(new COMMAND(dev, D3D12_COMMAND_LIST_TYPE_DIRECT));
	}
	RenderAction(){}
	//�f�X�g���N�^
	~RenderAction()
	{
		m_constant.reset();

		//�I����GPU�������S�I���m�F
		Com->GetFence()->Signal(0);
		Com->GetQueue()->Signal(Com->GetFence().Get(), 1);
		if (Com->GetFence()->GetCompletedValue() != 1)
		{
			Com->GetFence()->SetEventOnCompletion(1, Com->GetFenceEvent());
			WaitForSingleObject(Com->GetFenceEvent(), INFINITE);
		}
		//���
		Com.reset();
	};
	COMMAND* GetCom(){return Com.get();}

	void  virtual Action()=0;

	
	//�R���X�^���g�o�b�t�@
	class Constant :public StructBuffer
	{
	public:
		Constant(ComPtr<ID3D12Device>& dev)
		{
			CreateResource(dev, sizeof(XMFLOAT4X4) + (256 - sizeof(XMFLOAT4X4) % 256));
		}
	};

protected:
	unique_ptr<COMMAND>		Com;	   //�����_�����O�A�N�V�����R�}���h
	unique_ptr<Constant>	m_constant;//WVP�p�����R���X�^���g�o�b�t�@
};



//�����_�����O�A�N�V�����F�����_�����O�e�N�X�`�����o�b�N�o�b�t�@�ɕ`��----------
class RenderAction_Output :public RenderAction
{
public:
	RenderAction_Output(ComPtr<ID3D12Device>& dev, unique_ptr<COMMAND> p):RenderAction()
	{
		m_constant.reset(new Constant(dev));
		Com = move(p);
	}
	void Action();
	void ActionPresent();
private:
	
};

//�����_�����O�A�N�V�����F�RD�h���[--------------------------------------------
class RenderAction_3D_Cube :public RenderAction
{
public:
	RenderAction_3D_Cube(ComPtr<ID3D12Device>& dev):RenderAction(dev)
	{
	}

	void Init(float x, float y, float z)
	{
		m_pos.x = x;
		m_pos.y = y;
		m_pos.z = z;
	}

	void Action();
private:
	XMFLOAT3 m_pos{0,0,0};
	float m_r{0.0f};

};
