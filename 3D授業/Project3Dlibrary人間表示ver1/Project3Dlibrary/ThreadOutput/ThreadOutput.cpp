#include"ThreadOutput.h"

constexpr auto WINDOW_WIDTH{ 800 };
constexpr auto WINDOW_HEIGHT{ 720 };

void ThrendOutput::StartRun(COMMAND* Com)
{
	//コマンドチェック
	if (Com == nullptr)
		return;

	while (1)
	{
		if (m_run.load(memory_order_acquire) == false)
			break;
	}

	//レンダリング開始
	m_Com = Com;
	m_run.store(true, memory_order_release);
}


ThrendOutput::ThrendOutput()
{
	m_thread.reset(new thread([&]()
	{

		while (m_thread_end.load(memory_order_acquire))
		{
			//スピン
			if (m_run.load(memory_order_acquire)==false)
			{
				continue;
			}
			

			// コマンドリストを実行
			ID3D12CommandList* ppCommandLists[]{ m_Com->GetList().Get() };
			m_Com->GetQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

			//フェンスのシグナルを初期の0に戻す
			m_Com->GetFence()->Signal(0);

			//キューの状態「コマンド実行中」「コマンド完了」をフェンスが確保
			//キューのシグナルが１なら「コマンド完了」設定、　0は「コマンド実行中」にフェンス値が変動する
			if (FAILED(m_Com->GetQueue()->Signal(m_Com->GetFence().Get(), 1)))
				break;

			// 現在のフェンスが0値ならコマンド実行中のため、待機させる
			if (m_Com->GetFence()->GetCompletedValue() != 1)
			{
				//イベントに「コマンド完了」が来たら知らせる指示
				if (FAILED(m_Com->GetFence()->SetEventOnCompletion(1, m_Com->GetFenceEvent())))
					break;
				//イベント「コマンド完了」が来るまで待機
				WaitForSingleObject(m_Com->GetFenceEvent(), INFINITE);
			}

			m_run.store(false, memory_order_release);
		}
	}));

}


ThrendOutput::~ThrendOutput()
{
	m_thread_end.store(false, memory_order_release);
	m_thread->join();
	m_thread.reset();
}