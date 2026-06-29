#pragma once
#include <te2d_types.hpp>
#include <render/te2d_sprite.hpp>

// Один кадр анимации
struct te2d_anim_frame {
    te2d_rect uv_region;    // UV-регион на спайсшите (в пикселях)
    f32 duration = 0.1f;    // длительность кадра в секундах
};

// Анимация — последовательность кадров
struct te2d_animation {
    const char* name = nullptr;           // имя анимации (для отладки)
    te2d_anim_frame* frames = nullptr;    // массив кадров
    u32 frame_count = 0;                  // количество кадров
    bool loop = true;                     // зациклена?

    // Создаёт анимацию из массива кадров (данные копируются в аллокатор)
    static te2d_animation create(te2d_allocator& allocator,
                                 const char* name,
                                 const te2d_anim_frame* frames,
                                 u32 frame_count,
                                 bool loop = true);

    // Возвращает кадр по индексу (с учётом зацикливания)
    const te2d_anim_frame& get_frame(u32 index) const;
};