#pragma once
#include <te2d_types.hpp>

// Тип джойнта
enum class te2d_joint_type {
    distance,  // сохраняет расстояние между двумя телами
    revolute,  // вращение вокруг общей точки
};

// Джойнт — связь между двумя телами
struct te2d_joint {
    te2d_joint_type type = te2d_joint_type::distance;

    i32 body_a = -1;        // индекс первого тела в мире (-1 = якорь в мире)
    i32 body_b = -1;        // индекс второго тела (-1 = якорь в мире)

    // Общие параметры
    te2d_vec2 anchor_a;     // точка крепления на теле A (локальные координаты)
    te2d_vec2 anchor_b;     // точка крепления на теле B (локальные координаты)

    // Distance joint
    f32 target_distance = 0.0f;   // желаемое расстояние
    f32 stiffness = 1.0f;         // жёсткость пружины (0 = мягкая, 1 = жёсткая)
    f32 damping = 0.0f;           // затухание пружины

    // Revolute joint
    bool enable_limit = false;    // включить ограничение угла
    f32 lower_angle = 0.0f;       // минимальный угол (радианы)
    f32 upper_angle = 0.0f;       // максимальный угол (радианы)

    // Создаёт distance joint
    static te2d_joint create_distance(i32 body_a, i32 body_b,
                                      te2d_vec2 anchor_a, te2d_vec2 anchor_b,
                                      f32 target_distance, f32 stiffness = 1.0f, f32 damping = 0.0f);

    // Создаёт revolute joint (якорь в точке world_anchor)
    static te2d_joint create_revolute(i32 body_a, i32 body_b,
                                      te2d_vec2 world_anchor,
                                      f32 lower = 0.0f, f32 upper = 0.0f);

    // Применяет силы джойнта к телам
    void solve(te2d_rigidbody* bodies, u32 body_count);
};