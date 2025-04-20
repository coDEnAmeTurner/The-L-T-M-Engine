#include "PoolAllocator.h"

PoolAllocator::PoolAllocator(std::uint32_t block_count, std::uint32_t compo_count, std::uint32_t compo_size, bool manual_align) {
	assert(block_count > 0 && compo_count > 0 && compo_size > 0);
	assert(is_pow_of_2(block_count) && is_pow_of_2(compo_count) && is_pow_of_2(compo_size));
	assert(check_u32_divisor(block_count, compo_count));

	m_blockCount = block_count;
	m_compoCount = compo_count;
	m_compoSize = compo_size;
	m_manualAlign = manual_align;

	size_t must_size = block_count * compo_count * compo_size;
	uint32_t align = ternary_pred(compo_size >= 8u, compo_size, 8u);
	std::uint32_t allocated_size = ternary_pred(manual_align == true, must_size + align, must_size);

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

	uintptr_t next_free = reinterpret_cast<uintptr_t>(m_memory);
	m_currentFreePtr = m_memory;
	char* m_tailptr_p1 = m_memory + allocated_size;

	/*for (char* block_jump_ptr = m_memory; block_jump_ptr < m_tailptr_p1; block_jump_ptr += compo_count * compo_size) {
		memcpy(block_jump_ptr, next_free)
	}*/
}
//~PoolAllocator();

//char* PoolAllocator::alloc() {
//
//}

/*oid free(char* target_ptr);

char* getMemory() const;
std::uint32_t getSizeInBytes() const;
char* getCurrentFreePtr() const;*/