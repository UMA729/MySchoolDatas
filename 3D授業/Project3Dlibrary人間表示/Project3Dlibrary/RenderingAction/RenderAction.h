#pragma once
#include <thread>
#include <atomic>

#include "../DrectX12/DX12Library.h"
#include "../ShaderGraphicPipeline/GraphicPipeLine.h"
#include "../ModelStructs/RenPolygonModel.h"

extern unique_ptr<Model_RenderPolygon>  RenderPolygon;//モデル＿レンダリングテクスチャ描画ポリゴン
extern unique_ptr<RENDER>				Render;		  //レンダリングバッファ

//レンダリングアクション--------------------------------------------
class RenderAction
{
public:
	//コンストラクタ
	RenderAction(ComPtr<ID3D12Device>& dev)
	{
		m_constant.reset(new Constant(dev));
		Com.reset(new COMMAND(dev, D3D12_COMMAND_LIST_TYPE_DIRECT));
	}
	RenderAction(){}
	//デストラクタ
	~RenderAction()
	{
		m_constant.reset();

		//終了時GPU処理完全終了確認
		Com->GetFence()->Signal(0);
		Com->GetQueue()->Signal(Com->GetFence().Get(), 1);
		if (Com->GetFence()->GetCompletedValue() != 1)
		{
			Com->GetFence()->SetEventOnCompletion(1, Com->GetFenceEvent());
			WaitForSingleObject(Com->GetFenceEvent(), INFINITE);
		}
		//解放
		Com.reset();
	};
	COMMAND* GetCom(){return Com.get();}

	void  virtual Action()=0;

	
	//コンスタントバッファ
	class Constant :public StructBuffer
	{
	public:
		Constant(ComPtr<ID3D12Device>& dev)
		{
			CreateResource(dev, sizeof(XMFLOAT4X4) + (256 - sizeof(XMFLOAT4X4) % 256));
		}
	};

protected:
	unique_ptr<COMMAND>		Com;	   //レンダリングアクションコマンド
	unique_ptr<Constant>	m_constant;//WVP用初期コンスタントバッファ
};



//レンダリングアクション：レンダリングテクスチャをバックバッファに描画----------
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

//レンダリングアクション：３Dドロー--------------------------------------------
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
