#pragma once
#include <physics/te2d_collider.hpp>
#include <physics/te2d_rigidbody.hpp>

// Данные о столкновении
struct te2d_collision_result {
    bool hit = false;              // было ли столкновение
    te2d_vec2 normal;              // нормаль столкновения (направление выталкивания)
    f32 depth = 0.0f;              // глубина проникновения
};

// Обнаружение столкновений
namespace te2d_collision {

    // AABB vs AABB
    te2d_collision_result aabb_vs_aabb(
        const te2d_vec2& pos_a, const te2d_collider& col_a,
        const te2d_vec2& pos_b, const te2d_collider& col_b);

    // Circle vs Circle
    te2d_collision_result circle_vs_circle(
        const te2d_vec2& pos_a, const te2d_collider& col_a,
        const te2d_vec2& pos_b, const te2d_collider& col_b);

    // AABB vs Circle
    te2d_collision_result aabb_vs_circle(
        const te2d_vec2& pos_aabb, const te2d_collider& col_aabb,
        const te2d_vec2& pos_circle, const te2d_collider& col_circle);

    // Диспетчер: определяет типы коллайдеров и вызывает нужную функцию
    te2d_collision_result detect(
        const te2d_vec2& pos_a, const te2d_collider& col_a,
        const te2d_vec2& pos_b, const te2d_collider& col_b);

    // Разрешение столкновения: выталкивание + коррекция скорости
    void resolve(
        te2d_rigidbody& body_a, te2d_rigidbody& body_b,
        const te2d_collision_result& result);

} // namespace te2d_collision