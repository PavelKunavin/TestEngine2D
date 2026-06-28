#pragma once
#include <te2d_types.hpp>

// Тип твёрдого тела
enum class te2d_body_type {
    static_body,   // не двигается (платформы, стены)
    dynamic_body,  // двигается под действием сил (персонаж, враги)
};

// Твёрдое тело для 2D-физики
struct te2d_rigidbody {
    te2d_body_type type = te2d_body_type::dynamic_body;

    te2d_vec2 position;       // текущая позиция
    te2d_vec2 velocity;       // линейная скорость
    te2d_vec2 acceleration;   // накопленное ускорение (сбрасывается каждый шаг)

    f32 mass = 1.0f;          // масса (0 = бесконечная, для static)
    f32 drag = 0.0f;          // затухание (0 = без затухания)
    bool affected_by_gravity = true; // действует ли гравитация

    // Создаёт динамическое тело
    static te2d_rigidbody create_dynamic(f32 mass, f32 drag = 0.0f);

    // Создаёт статическое тело
    static te2d_rigidbody create_static();

    // Применяет силу (накапливает ускорение: F/m)
    void apply_force(const te2d_vec2& force);

    // Применяет импульс (мгновенное изменение скорости)
    void apply_impulse(const te2d_vec2& impulse);

    // Интегрирует позицию за dt (полушаг Эйлера)
    void integrate(f32 dt);
};