#include "Handle.h"

HeapAllocator* HeapAllocator::m_instance = nullptr;
HANDLE HeapAllocator::m_curRelocHandle = -1;
std::uint32_t HeapAllocator::m_size = 0;
std::vector<HandleEntry> HeapAllocator::m_table;

void HeapAllocator::init(std::uint32_t size) {
	if (m_instance == nullptr) {
		m_instance = new HeapAllocator();
		m_table.resize(size);
		m_size = size;
	}
}
HANDLE HeapAllocator::alloc(std::uint32_t size) {
	//Windows API: malloc.h -> _aligned_malloc for malloc with alignment
	void* new_alloc = _aligned_malloc(size, MINIMUM_ALIGNMENT);
	m_table.push_back(
		{
			new_alloc,
			size
		}
	);
	return m_table.size() - 1;
}
void HeapAllocator::dealloc(HANDLE handle) {
	assert(handle < m_table.size() && "Invalid handle");
	free(m_table[handle].ptr);
	m_table.erase(m_table.begin() + handle);
}

void HeapAllocator::destroy() {
	for (auto& entry : m_table) {
		//allocation with _aligned_malloc, freeing with _free_malloc
		_aligned_free(entry.ptr);

	}
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
	assert(new_block && "Failed to allocate new memory for defragmentation.");

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
