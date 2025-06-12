#pragma once
#include <thread>
#include <atomic>

#include "../DrectX12/DX12Library.h"
#include "../ShaderGraphicPipeline/GraphicPipeLine.h"
#include "../ModelStructs/RenPolygonModel.h"


//アウトプットスレッドクラス
class ThrendOutput
{
public:
	ThrendOutput();
	~ThrendOutput();

	void StartRun(COMMAND* Com);
	bool CheckRun(){return m_run.load(memory_order_acquire);}
	

private:
	COMMAND* m_Com;

	atomic<bool> m_run{false};
	unique_ptr<thread> m_thread;
	atomic<bool>	   m_thread_end{ true};
};