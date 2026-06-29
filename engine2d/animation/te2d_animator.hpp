#pragma once
#include <animation/te2d_animation.hpp>

// Состояние аниматора
enum class te2d_anim_state {
    idle,
    run,
    jump,
    attack,
    // ... добавим позже
};

// Проигрыватель анимаций на сущности
struct te2d_animator {
    te2d_sprite* sprite = nullptr;        // спрайт, которым управляем
    te2d_animation* current_anim = nullptr; // текущая анимация
    te2d_anim_state current_state = te2d_anim_state::idle;

    u32 current_frame = 0;     // индекс текущего кадра
    f32 timer = 0.0f;          // таймер текущего кадра

    // Создаёт аниматор, привязанный к спрайту
    static te2d_animator create(te2d_sprite* sprite);

    // Смена анимации
    void play(te2d_animation* anim);

    // Смена анимации по состоянию
    void set_state(te2d_anim_state state,
                   te2d_animation* idle,
                   te2d_animation* run = nullptr,
                   te2d_animation* jump = nullptr,
                   te2d_animation* attack = nullptr);

    // Обновление (вызывается каждый кадр)
    void update(f32 dt);

    // Принудительный сброс в начало текущей анимации
    void reset();
};