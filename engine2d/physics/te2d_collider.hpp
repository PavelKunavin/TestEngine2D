#pragma once
#include <te2d_types.hpp>

// Тип коллайдера
enum class te2d_collider_type {
    aabb,   // прямоугольник, выровненный по осям
    circle, // окружность
};

// Коллайдер для 2D-физики
struct te2d_collider {
    te2d_collider_type type = te2d_collider_type::aabb;

    te2d_vec2 offset;   // смещение относительно позиции тела
    te2d_vec2 size;     // для AABB: ширина/высота, для circle: radius в x (y не используется)

    // Создаёт AABB-коллайдер
    static te2d_collider create_aabb(f32 width, f32 height, te2d_vec2 offset = {0.0f, 0.0f});

    // Создаёт круглый коллайдер
    static te2d_collider create_circle(f32 radius, te2d_vec2 offset = {0.0f, 0.0f});

    // Возвращает мировой AABB (с учётом позиции тела)
    te2d_rect get_world_aabb(const te2d_vec2& body_position) const;

    // Возвращает мировую окружность (с учётом позиции тела)
    void get_world_circle(const te2d_vec2& body_position, te2d_vec2& out_center, f32& out_radius) const;
};