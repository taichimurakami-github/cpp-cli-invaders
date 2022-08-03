#pragma once


// class Tを最大MAXSIZE個確保可能なPoolAllocatorを実装してください
template <class T, size_t MAXSIZE>
class PoolAllocator
{
public:
	// コンストラクタ
	PoolAllocator()
	{
		// TODO: 必要に応じて実装してください

		//1. bufferの初期化
		for (int i = 0; i < MAXSIZE - 1; i++)
		{
			buffer[i].next_avail_buff = &(buffer[i + 1]);
		}
		buffer[MAXSIZE - 1].next_avail_buff = nullptr; //最後の要素

		//2. now_free_buffの初期化
		now_free_buff = &(buffer[0]);
	}

	// デストラクタ
	~PoolAllocator()
	{
		// TODO: 必要に応じて実装してください
	}

	// 確保できない場合はnullptrを返す事。
	T* Alloc()
	{
		// TODO: 実装してください

		if (now_free_buff == nullptr)
		{
			return nullptr;
		}

		T* result = &(now_free_buff->value);//return値のメモ
		now_free_buff = now_free_buff->next_avail_buff;//now_free_buffの更新

		return result;
	}

	// Free(nullptr)で誤動作しないようにする事。
	void Free(T* addr)
	{
		// TODO: 実装してください
		if (addr == nullptr)
		{
			return;
		}

		elem_type* new_free_buff = reinterpret_cast<elem_type*>(addr);//T* addr -> elem_type* addrに解釈
		new_free_buff->next_avail_buff = now_free_buff;
		now_free_buff = new_free_buff;
	}

private:
	// TODO: 実装してください
	union elem_type {
		T value;
		elem_type* next_avail_buff;
	};

	elem_type buffer[MAXSIZE];
	elem_type* now_free_buff;
}