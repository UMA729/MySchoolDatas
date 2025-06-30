#include"ThreadOutput.h"

constexpr auto WINDOW_WIDTH{ 800 };
constexpr auto WINDOW_HEIGHT{ 720 };

void ThrendOutput::StartRun(COMMAND* Com)
{
	//�R�}���h�`�F�b�N
	if (Com == nullptr)
		return;

	while (1)
	{
		if (m_run.load(memory_order_acquire) == false)
			break;
	}

	//�����_�����O�J�n
	m_Com = Com;
	m_run.store(true, memory_order_release);
}


ThrendOutput::ThrendOutput()
{
	m_thread.reset(new thread([&]()
	{

		while (m_thread_end.load(memory_order_acquire))
		{
			//�X�s��
			if (m_run.load(memory_order_acquire)==false)
			{
				continue;
			}
			

			// �R�}���h���X�g�����s
			ID3D12CommandList* ppCommandLists[]{ m_Com->GetList().Get() };
			m_Com->GetQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

			//�t�F���X�̃V�O�i����������0�ɖ߂�
			m_Com->GetFence()->Signal(0);

			//�L���[�̏�ԁu�R�}���h���s���v�u�R�}���h�����v���t�F���X���m��
			//�L���[�̃V�O�i�����P�Ȃ�u�R�}���h�����v�ݒ�A�@0�́u�R�}���h���s���v�Ƀt�F���X�l���ϓ�����
			if (FAILED(m_Com->GetQueue()->Signal(m_Com->GetFence().Get(), 1)))
				break;

			// ���݂̃t�F���X��0�l�Ȃ�R�}���h���s���̂��߁A�ҋ@������
			if (m_Com->GetFence()->GetCompletedValue() != 1)
			{
				//�C�x���g�Ɂu�R�}���h�����v��������m�点��w��
				if (FAILED(m_Com->GetFence()->SetEventOnCompletion(1, m_Com->GetFenceEvent())))
					break;
				//�C�x���g�u�R�}���h�����v������܂őҋ@
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