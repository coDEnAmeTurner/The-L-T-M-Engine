#include "Handle.h"

NonRelocTable* NonRelocTable::m_instance = nullptr;

void NonRelocTable::init(std::uint32_t size) {
	if (m_instance == nullptr) {
		m_instance = new NonRelocTable();
		m_table.resize(size);
		m_size = size;
	}
}
HANDLE NonRelocTable::alloc(std::uint32_t size) {
	void* new_alloc = malloc(size);
	m_table.push_back(new_alloc);
	return m_table.size() - 1;
}
void NonRelocTable::dealloc(HANDLE handle) {
	assert(handle < m_table.size() && "Invalid handle");
	free(HANDLE_RESOLVER(handle));
	HANDLE_RESOLVER(handle) = nullptr;
}

void NonRelocTable::destroy() {
	for (auto& ptr : m_table) {
		if (ptr != nullptr) {
			free(ptr);
			ptr = nullptr;
		}
	}
}