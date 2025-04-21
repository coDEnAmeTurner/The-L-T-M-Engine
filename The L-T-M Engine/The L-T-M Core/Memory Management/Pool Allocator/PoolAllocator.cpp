#include "PoolAllocator.h"

PoolAllocator::PoolAllocator(std::uint32_t block_count, std::uint32_t compo_count, std::uint32_t compo_size, bool manual_align, bool align_by_block) {
	assert(block_count > 0 && compo_count > 0 && compo_size > 0);
	assert(is_pow_of_2(block_count) && is_pow_of_2(compo_count) && is_pow_of_2(compo_size));
	assert(check_u32_divisor(block_count, compo_count));

	m_blockCount = block_count;
	m_compoCount = compo_count;
	m_compoSize = compo_size;
	m_manualAlign = manual_align;

	size_t block_size = compo_count * compo_size;
	size_t org_size = block_count * block_size;
	size_t min_align = 8u;
	size_t align = ternary_pred(align_by_block==true,
									ternary_pred(
										block_size >= 8u, 
										block_size,
										min_align
									),
									ternary_pred(
										compo_size >= 8u,
										static_cast<size_t>(compo_size),
										min_align
									)
								);
	std::uint32_t allocated_size = ternary_pred(manual_align == true, org_size + align, org_size);

	m_memory = new char[allocated_size];

	m_memory = ternary_pred(
		manual_align == true,
		align_pointer(
			m_memory,
			align,
			reinterpret_cast<uintptr_t>(m_memory + allocated_size - 1)
		),
		m_memory
	);

	m_currentFreePtr = m_memory;
	char* near_tailptr_p1 = m_memory + allocated_size - m_compoCount * m_compoSize;

	for (char* block_jump_ptr = m_memory; block_jump_ptr < near_tailptr_p1; block_jump_ptr += compo_count * compo_size) 
	{
		char* next_free_ptr = block_jump_ptr + static_cast<size_t>(m_compoCount * m_compoSize);
		uintptr_t next_free_val = reinterpret_cast<uintptr_t>(next_free_ptr);

		//if block size > 8 bytes, the value saved in block will be
		//0xXXXXXXXXXXXXXXXXffffff
		std::memcpy(
			block_jump_ptr,
			reinterpret_cast<char*>(&next_free_val),
			sizeof(uintptr_t)
		);
		std::memset(
			block_jump_ptr + sizeof(uintptr_t),
			0,
			m_compoCount * m_compoSize - sizeof(uintptr_t)
		);
	}

	//set the final block to point to null
	std::memset(
		near_tailptr_p1,
		0,
		m_compoCount * m_compoSize
	);
}


PoolAllocator::~PoolAllocator() {
	m_memory = ternary_pred(
		m_manualAlign == true,
		m_memory - *reinterpret_cast<uint8_t*>(m_memory - 1),
		m_memory
	);
	delete[] m_memory;
	m_memory = nullptr;
	m_currentFreePtr = nullptr;
	m_blockCount = 0;
	m_compoCount = 0;
	m_compoSize = 0;
	m_manualAlign = false;

}

char* PoolAllocator::alloc() {
	char* free_returned_ptr =  m_currentFreePtr;
	uintptr_t next_free_val = *reinterpret_cast<uintptr_t*>(free_returned_ptr);
	m_currentFreePtr = reinterpret_cast<char*>(next_free_val);

	return free_returned_ptr;
}

void PoolAllocator::free(char* target_ptr) {
	char* next_free_ptr = m_currentFreePtr;
	uintptr_t next_free_val = reinterpret_cast<uintptr_t>(target_ptr);
	std::memcpy(
		target_ptr,
		reinterpret_cast<char*>(&next_free_val),
		sizeof(uintptr_t)
	);
	std::memset(
		target_ptr + sizeof(uintptr_t),
		0,
		m_compoCount * m_compoSize - sizeof(uintptr_t)
	);
	m_currentFreePtr = target_ptr;

}

char* PoolAllocator::getMemory() const {
	return m_memory;

}
size_t PoolAllocator::getSizeInBytes() const {
	return m_blockCount * m_compoCount * m_compoSize;
}
char* PoolAllocator::getCurrentFreePtr() const {
	return m_currentFreePtr;
}
