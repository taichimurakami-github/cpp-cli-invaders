#pragma once


// class T���ő�MAXSIZE�m�ۉ\��PoolAllocator���������Ă�������
template <class T, size_t MAXSIZE>
class PoolAllocator
{
public:
	// �R���X�g���N�^
	PoolAllocator()
	{
		// TODO: �K�v�ɉ����Ď������Ă�������

		//1. buffer�̏�����
		for (int i = 0; i < MAXSIZE - 1; i++)
		{
			buffer[i].next_avail_buff = &(buffer[i + 1]);
		}
		buffer[MAXSIZE - 1].next_avail_buff = nullptr; //�Ō�̗v�f

		//2. now_free_buff�̏�����
		now_free_buff = &(buffer[0]);
	}

	// �f�X�g���N�^
	~PoolAllocator()
	{
		// TODO: �K�v�ɉ����Ď������Ă�������
	}

	// �m�ۂł��Ȃ��ꍇ��nullptr��Ԃ����B
	T* Alloc()
	{
		// TODO: �������Ă�������

		if (now_free_buff == nullptr)
		{
			return nullptr;
		}

		T* result = &(now_free_buff->value);//return�l�̃���
		now_free_buff = now_free_buff->next_avail_buff;//now_free_buff�̍X�V

		return result;
	}

	// Free(nullptr)�Ō듮�삵�Ȃ��悤�ɂ��鎖�B
	void Free(T* addr)
	{
		// TODO: �������Ă�������
		if (addr == nullptr)
		{
			return;
		}

		elem_type* new_free_buff = reinterpret_cast<elem_type*>(addr);//T* addr -> elem_type* addr�ɉ���
		new_free_buff->next_avail_buff = now_free_buff;
		now_free_buff = new_free_buff;
	}

private:
	// TODO: �������Ă�������
	union elem_type {
		T value;
		elem_type* next_avail_buff;
	};

	elem_type buffer[MAXSIZE];
	elem_type* now_free_buff;
}