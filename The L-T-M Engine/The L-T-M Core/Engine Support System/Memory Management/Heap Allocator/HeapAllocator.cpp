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

	//try to find the worst fit at most 2 times
	std::vector<HandleEntry>::iterator worst_fit_it = m_freeList.end();
	uint8_t try_ind;
	for (try_ind = 0; try_ind < 2; try_ind++) {
		worst_fit_it = std::max(
			m_freeList.begin(),
			m_freeList.end(),
			[&](const HandleEntry& entry) {return entry.size >= size; }
		);
		if (worst_fit_it != m_freeList.end())
			break;
		if (try_ind == 0)
			relocate(m_memoSize);
	}

	assert(worst_fit_it != m_freeList.end() || "No suitable free block found");

	//align ptr of worst fit block
	HandleEntry& free_block = *worst_fit_it;
	void* aligned_ptr = align_pointer(
		(char*)free_block.ptr, 
		std::max((size_t)MINIMUM_ALIGNMENT, free_block.size), 
		reinterpret_cast<uintptr_t>((char*)free_block.ptr + free_block.size - 1)
		);

	assert(
		(char*)aligned_ptr + size <= (char*)free_block.ptr + free_block.size 
		|| "allocation after alignment is out of free_block's bound"
	);

	//calc and store the shift from original ptr 
	std::uint8_t b_shift = (uintptr_t)aligned_ptr - (uintptr_t)free_block.ptr;
	((char*)aligned_ptr)[-1] = b_shift;

	//update free_block since the free_block has been decremented in size
	//if the size is 0, delete free_block
	free_block.ptr = (char*)aligned_ptr + size;
	free_block.size = free_block.size - size - b_shift;
	if (free_block.size == 0)
		m_freeList.erase(worst_fit_it);

	//add to noreloc table
	auto empty_entry = std::find_if(
		m_table.begin(),
		m_table.end(),
		[](const HandleEntry& entry)
		{
			return entry.ptr == nullptr || entry.size == 0;
		}
	);
	if (empty_entry == m_table.end())
		m_table.push_back({ (void*)aligned_ptr, size });
	else
		*empty_entry = { aligned_ptr, size };
	
	return empty_entry - m_table.begin();
}
void HeapAllocator::dealloc(HANDLE handle) {
	assert(handle < m_table.size() || "Invalid handle");

	//set nonreloc freed entry to empty entry
	HandleEntry& freed_entry = m_table[handle];
	HandleEntry temp_entry = freed_entry;
	freed_entry = { nullptr, 0 };

	//find the adjacent free blocks
	char* org_ptr = (char*)temp_entry.ptr - static_cast<std::uint8_t>(((char*)temp_entry.ptr)[-1]);
	const auto& nearest_lower_it = std::find_if(
		m_freeList.begin(),
		m_freeList.end(),
		[&](const HandleEntry& entry) {return (char*)entry.ptr + entry.size == org_ptr; }
	);
	const auto& nearest_higher_it = std::find_if(
		m_freeList.begin(),
		m_freeList.end(),
		[&](const HandleEntry& entry) {return org_ptr + temp_entry.size == entry.ptr; }
	);

	//merge then push back
	HandleEntry merge_entry = temp_entry;
	if (nearest_lower_it != m_freeList.end()) {
		merge_entry = { nearest_lower_it->ptr, nearest_lower_it->size + merge_entry.size };
	}
	if (nearest_higher_it != m_freeList.end()) {
		merge_entry.size += nearest_higher_it->size;
	}

	m_freeList.push_back(merge_entry);
}

void HeapAllocator::destroy() {
	_aligned_free(m_memory);
}

//this returns the handle for the next realocation in the currently used block count
//returns -1 if the end of the nonreloctable is met
void HeapAllocator::relocate(std::uint32_t bytes) {
	
}
