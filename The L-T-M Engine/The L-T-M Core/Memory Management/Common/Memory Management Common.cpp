#include "Memory Management Common.h"

// Shift the given address upwards if/as necessary to
// ensure it is aligned to the given number of bytes.
uintptr_t AlignAddress(uintptr_t addr, size_t align)
{
	const size_t mask = align - 1;
	assert((align & mask) == 0); // pwr of 2
	uintptr_t addrAligned = addr + mask;
	size_t not_mask = ~mask;
	uintptr_t addrAlignedMask = addrAligned & not_mask;
	return addrAlignedMask;
}

template<typename T>
T* AlignPointer(T* ptr, size_t align) {
	const uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
	const uintptr_t addrAligned = AlignAddress(addr, align);
	return reinterpret_cast<T*>(addrAligned);
}

void* AllocAligned(std::uint32_t bytes, size_t align, std::uint32_t& bytes_store_shift)
{
	// Allocate 'align' more bytes than we need.
	size_t actualBytes = bytes + align;
	// Allocate unaligned block.
	char* pRawMem = new char[actualBytes];
	// Align the block. If no alignment occurred,
	// shift it up the full 'align' bytes so we
	// always have room to store the shift.
	char* pAlignedMem = AlignPointer(pRawMem, align);
	if (pAlignedMem == pRawMem)
		pAlignedMem += align;
	// Determine the shift, and store it.
	ptrdiff_t shift = pAlignedMem - pRawMem;
	assert(shift > 0 && shift <= align);

	bytes_store_shift = Log2(shift) / 8 + 1;

	std::memcpy(pAlignedMem - bytes_store_shift, &shift, bytes_store_shift);

	return pAlignedMem;
}

void FreeAligned(void* pMem, std::uint32_t& bytes_store_shift)
{
	if (pMem)
	{
		// Convert to U8 pointer.
		char* pAlignedMem = reinterpret_cast<char*>(pMem) - bytes_store_shift;
		ptrdiff_t shift = 0;
		std::memcpy(&shift, pAlignedMem, bytes_store_shift); // Get the shift value

		// Back up to the actual allocated address,
		// and array-delete it.
		char* pRawMem = reinterpret_cast<char*>(pMem) - shift;
		delete[] pRawMem;
	}
}

std::uint32_t Log2(std::uint32_t x) {
	unsigned long index;
	_BitScanReverse(&index, x);
	return index;
}

bool isPowOf2(std::uint32_t x) {
	return (x != 0) && ((x & (x - 1)) == 0);
}

std::uint32_t BaseAllocator::getSize() const {
	return m_size;
}

char* BaseAllocator::getMemory() const {
	return m_memory;
}

BaseAllocator::BaseAllocator(std::uint32_t bytes, std::uint32_t align) 
							: m_size(bytes), m_align(align)
{
	assert(m_size > 0 && align > 0 && isPowOf2(align)); // Ensure size and alignment are valid
	
	m_memory = reinterpret_cast<char*>(AllocAligned(bytes, align, m_byteStoreShift)); // Allocate memory with alignment

}