#include "HeapAllocator.h" 

HeapAllocator* HeapAllocator::m_instance = nullptr;
void* HeapAllocator::m_memory = nullptr;
HANDLE HeapAllocator::m_curRelocHandle = -1;
std::uint32_t HeapAllocator::m_memoSize = 0;
std::vector<HandleEntry> HeapAllocator::m_table;
std::vector<HandleEntry> HeapAllocator::m_freeList;


void HeapAllocator::init(std::uint32_t memo_init_size) {
	assert(m_instance == nullptr || "HeapAllocator already initialized");

	m_instance = new HeapAllocator();

	m_memory = _aligned_malloc(memo_init_size, MINIMUM_ALIGNMENT);
	m_memoSize = memo_init_size;

	m_freeList.push_back({ m_memory, m_memoSize });
}
HANDLE HeapAllocator::alloc(std::uint32_t size) {
	assert(size <= m_memoSize || "Allocation size exceeds memory size");

	auto worst_fit_it = m_freeList.end();
	size_t worst_fit_size = 0;

	for (auto it = m_freeList.begin(); it != m_freeList.end(); ++it) {
		if (it->size >= size && it->size > worst_fit_size) {
			worst_fit_it = it;
			worst_fit_size = it->size;
		}
	}

	assert(worst_fit_it != m_freeList.end() || "No suitable free block found");

	HandleEntry& free_block = *worst_fit_it; // Get the first free block that fits the size
	void* aligned_ptr = align_pointer(
		free_block.ptr, 
		std::max((size_t)MINIMUM_ALIGNMENT, free_block.size), 
		reinterpret_cast<uintptr_t>((char*)free_block.ptr + free_block.size - 1)
		);

	assert(
		(char*)aligned_ptr + size <= (char*)free_block.ptr + free_block.size 
		|| "allocation after alignment is out of free_block's bound"
	);

	std::uint8_t b_shift = (uintptr_t)aligned_ptr - (uintptr_t)free_block.ptr;
	((char*)aligned_ptr)[-1] = b_shift;

	free_block.ptr = (char*)aligned_ptr + size;
	free_block.size = free_block.size - size - b_shift;

	m_table.push_back({ (void*)aligned_ptr, size });

}
void HeapAllocator::dealloc(HANDLE handle) {
	assert(handle < m_table.size() || "Invalid handle");
	free(m_table[handle].ptr);
	m_table.erase(m_table.begin() + handle);
}

void HeapAllocator::destroy() {
	_aligned_free(m_memory);
}

//this returns the handle for the next realocation in the currently used block count
//returns -1 if the end of the nonreloctable is met
void HeapAllocator::relocate(std::uint32_t dealloc_block_count) {
	assert(dealloc_block_count < m_table.size());
	assert(m_table.begin() + m_curRelocHandle + dealloc_block_count <= m_table.end());

	// Step 1: Calculate total memory required and determine the maximum alignment
	size_t totalSize = 0;
	size_t maxAlignment = 1;  // Start with the smallest alignment possible
	for (const auto& entry : m_table) {
		totalSize += entry.size;
		// Find the largest alignment requirement
		size_t max_in = std::max((size_t)MINIMUM_ALIGNMENT, entry.size);
		maxAlignment = std::max(maxAlignment, max_in);
	}

	// Step 2: Allocate a new large block for packed memory with the proper alignment
	void* new_block = _aligned_malloc(totalSize, maxAlignment);
	assert(new_block || "Failed to allocate new memory for defragmentation.");

	// Step 3: Move blocks into the new memory block sequentially, respecting alignment
	size_t curr_pos = 0; // This will track the new position in the packed memory
	for (auto& entry : m_table) {
		// Align the offset to the alignment of the current block
		size_t align_max_in = std::max((size_t)MINIMUM_ALIGNMENT, entry.size);
		size_t aligned_pos = (curr_pos + align_max_in - 1) & ~(align_max_in - 1);

		void* old_entry_ptr = entry.ptr;
		// Copy the memory block to the new memory location
		memcpy(static_cast<char*>(new_block) + aligned_pos, entry.ptr, entry.size);
		// Update the handle to point to the new location
		entry.ptr = static_cast<char*>(new_block) + aligned_pos;

		// Increment offset for the next block
		curr_pos = aligned_pos + entry.size;

		_aligned_free(old_entry_ptr); // Free the old memory block	
	}

}
