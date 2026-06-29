#include <core/te2d_allocator.hpp>
#include <cstdlib>   // malloc, free
#include <cstring>   // memset

te2d_allocator te2d_allocator::create(u64 size_bytes) {
    te2d_allocator alloc = {};
    alloc.memory = (u8*)std::malloc(size_bytes);
    if (alloc.memory) {
        std::memset(alloc.memory, 0, size_bytes);
        alloc.capacity = size_bytes;
        alloc.used = 0;
    }
    return alloc;
}

void te2d_allocator::destroy() {
    if (memory) {
        std::free(memory);
        memory = nullptr;
    }
    capacity = 0;
    used = 0;
}

void* te2d_allocator::alloc(u64 size_bytes) {
    if (used + size_bytes > capacity) return nullptr;

    void* ptr = memory + used;
    used += size_bytes;
    return ptr;
}

void* te2d_allocator::alloc_aligned(u64 size_bytes, u64 alignment) {
    // Вычисляем смещение до выровненного адреса
    u64 current_addr = (u64)(memory + used);
    u64 aligned_addr = (current_addr + alignment - 1) & ~(alignment - 1);
    u64 padding = aligned_addr - current_addr;

    if (used + padding + size_bytes > capacity) return nullptr;

    used += padding;
    void* ptr = memory + used;
    used += size_bytes;
    return ptr;
}

void te2d_allocator::reset() {
    used = 0;
}