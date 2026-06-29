#pragma once
#include <te2d_types.hpp>

// Линейный аллокатор
struct te2d_allocator {
    u8* memory = nullptr;   // начало блока памяти
    u64 capacity = 0;       // общий размер блока
    u64 used = 0;           // сколько уже выделено

    // Создаёт аллокатор с указанным объёмом памяти
    static te2d_allocator create(u64 size_bytes);

    // Уничтожает аллокатор (освобождает память)
    void destroy();

    // Выделяет память (возвращает nullptr если не хватает)
    void* alloc(u64 size_bytes);

    // Выделяет память, выровненную по alignment (степень двойки)
    void* alloc_aligned(u64 size_bytes, u64 alignment);

    // Сбрасывает аллокатор (освобождает всю выделенную память)
    void reset();

    // Шаблонный alloc для удобства
    template<typename T>
    T* alloc_type() {
        return (T*)alloc(sizeof(T));
    }
};