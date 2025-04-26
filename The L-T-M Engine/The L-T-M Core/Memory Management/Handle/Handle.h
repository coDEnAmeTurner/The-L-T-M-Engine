#pragma once
#include <cstdint>
#include <vector>
#include <cassert>

#define HANDLE std::uint32_t
#define HANDLE_RESOLVER(i) NonRelocTable::getInstance()->getTable()[i]

class NonRelocTable
{
public:
	NonRelocTable() = default;
	~NonRelocTable() = default;

	static void init(std::uint32_t size);
	static void destroy();
	static HANDLE alloc(std::uint32_t size);
	static void dealloc(HANDLE handle);
	static void* relocate(std::uint32_t block_count);
	static NonRelocTable* getInstance() { return m_instance; }
	std::vector<void*>& getTable() const { return m_table; }

private:
	static std::uint32_t m_size;
	static std::vector<void*> m_table;
	static NonRelocTable* m_instance;
};

