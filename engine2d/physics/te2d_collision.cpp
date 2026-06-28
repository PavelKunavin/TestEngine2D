#include <physics/te2d_collision.hpp>
#include <cmath>
#include <algorithm>

namespace te2d_collision {

// ============================================================================
// AABB vs AABB
// ============================================================================

te2d_collision_result aabb_vs_aabb(
    const te2d_vec2& pos_a, const te2d_collider& col_a,
    const te2d_vec2& pos_b, const te2d_collider& col_b)
{
    te2d_collision_result result = {};

    te2d_rect a = col_a.get_world_aabb(pos_a);
    te2d_rect b = col_b.get_world_aabb(pos_b);

    f32 overlap_x = 0.0f;
    f32 overlap_y = 0.0f;

    // Пересечение по X
    f32 a_right  = a.x + a.w;
    f32 b_right  = b.x + b.w;
    if (a.x < b_right && a_right > b.x) {
        f32 left_overlap  = b_right - a.x;
        f32 right_overlap = a_right - b.x;
        overlap_x = (left_overlap < right_overlap) ? left_overlap : right_overlap;
    } else {
        return result; // нет пересечения
    }

    // Пересечение по Y
    f32 a_bottom = a.y + a.h;
    f32 b_bottom = b.y + b.h;
    if (a.y < b_bottom && a_bottom > b.y) {
        f32 top_overlap    = b_bottom - a.y;
        f32 bottom_overlap = a_bottom - b.y;
        overlap_y = (top_overlap < bottom_overlap) ? top_overlap : bottom_overlap;
    } else {
        return result; // нет пересечения
    }

    // Определяем нормаль и глубину по минимальной оси
    if (overlap_x < overlap_y) {
        result.depth = overlap_x;
        result.normal.x = (a.x < b.x) ? -1.0f : 1.0f;
        result.normal.y = 0.0f;
    } else {
        result.depth = overlap_y;
        result.normal.x = 0.0f;
        result.normal.y = (a.y < b.y) ? -1.0f : 1.0f;
    }

    result.hit = true;
    return result;
}

// ============================================================================
// Circle vs Circle
// ============================================================================

te2d_collision_result circle_vs_circle(
    const te2d_vec2& pos_a, const te2d_collider& col_a,
    const te2d_vec2& pos_b, const te2d_collider& col_b)
{
    te2d_collision_result result = {};

    te2d_vec2 center_a, center_b;
    f32 radius_a, radius_b;
    col_a.get_world_circle(pos_a, center_a, radius_a);
    col_b.get_world_circle(pos_b, center_b, radius_b);

    te2d_vec2 delta = center_b - center_a;
    f32 dist_sq = delta.x * delta.x + delta.y * delta.y;
    f32 sum_r = radius_a + radius_b;

    if (dist_sq >= sum_r * sum_r) {
        return result; // нет пересечения
    }

    f32 dist = std::sqrt(dist_sq);
    if (dist < 0.0001f) {
        // Центры совпадают — выталкиваем в произвольном направлении
        result.normal = {1.0f, 0.0f};
        result.depth = sum_r;
    } else {
        result.normal = delta / dist;
        result.depth = sum_r - dist;
    }

    result.hit = true;
    return result;
}

// ============================================================================
// AABB vs Circle
// ============================================================================

te2d_collision_result aabb_vs_circle(
    const te2d_vec2& pos_aabb, const te2d_collider& col_aabb,
    const te2d_vec2& pos_circle, const te2d_collider& col_circle)
{
    te2d_collision_result result = {};

    te2d_rect aabb = col_aabb.get_world_aabb(pos_aabb);

    te2d_vec2 circle_center;
    f32 radius;
    col_circle.get_world_circle(pos_circle, circle_center, radius);

    // Ближайшая точка на AABB к центру круга
    f32 closest_x = std::max(aabb.x, std::min(circle_center.x, aabb.x + aabb.w));
    f32 closest_y = std::max(aabb.y, std::min(circle_center.y, aabb.y + aabb.h));

    te2d_vec2 delta = circle_center - te2d_vec2{closest_x, closest_y};
    f32 dist_sq = delta.x * delta.x + delta.y * delta.y;

    if (dist_sq >= radius * radius) {
        return result; // нет пересечения
    }

    f32 dist = std::sqrt(dist_sq);
    if (dist < 0.0001f) {
        // Центр круга внутри AABB — выталкиваем по ближайшей стороне
        f32 left   = circle_center.x - aabb.x;
        f32 right  = (aabb.x + aabb.w) - circle_center.x;
        f32 top    = circle_center.y - aabb.y;
        f32 bottom = (aabb.y + aabb.h) - circle_center.y;

        f32 min_dist = left;
        result.normal = {-1.0f, 0.0f};

        if (right < min_dist) { min_dist = right; result.normal = {1.0f, 0.0f}; }
        if (top < min_dist)   { min_dist = top;   result.normal = {0.0f, -1.0f}; }
        if (bottom < min_dist){ min_dist = bottom; result.normal = {0.0f, 1.0f}; }

        result.depth = radius + min_dist;
    } else {
        result.normal = delta / dist;
        result.depth = radius - dist;
    }

    result.hit = true;
    return result;
}

// ============================================================================
// Диспетчер
// ============================================================================

te2d_collision_result detect(
    const te2d_vec2& pos_a, const te2d_collider& col_a,
    const te2d_vec2& pos_b, const te2d_collider& col_b)
{
    if (col_a.type == te2d_collider_type::aabb && col_b.type == te2d_collider_type::aabb) {
        return aabb_vs_aabb(pos_a, col_a, pos_b, col_b);
    }
    if (col_a.type == te2d_collider_type::circle && col_b.type == te2d_collider_type::circle) {
        return circle_vs_circle(pos_a, col_a, pos_b, col_b);
    }
    if (col_a.type == te2d_collider_type::aabb && col_b.type == te2d_collider_type::circle) {
        return aabb_vs_circle(pos_a, col_a, pos_b, col_b);
    }
    if (col_a.type == te2d_collider_type::circle && col_b.type == te2d_collider_type::aabb) {
        // Меняем порядок — AABB первым
        te2d_collision_result result = aabb_vs_circle(pos_b, col_b, pos_a, col_a);
        // Инвертируем нормаль
        result.normal = result.normal * -1.0f;
        return result;
    }

    return {}; // неизвестная комбинация
}

// ============================================================================
// Разрешение столкновения
// ============================================================================

void resolve(
    te2d_rigidbody& body_a, te2d_rigidbody& body_b,
    const te2d_collision_result& result)
{
    if (!result.hit) return;

    // Вычисляем веса (статическое тело = бесконечная масса, не двигается)
    f32 inv_mass_a = (body_a.type == te2d_body_type::dynamic_body) ? 1.0f / body_a.mass : 0.0f;
    f32 inv_mass_b = (body_b.type == te2d_body_type::dynamic_body) ? 1.0f / body_b.mass : 0.0f;
    f32 total_inv_mass = inv_mass_a + inv_mass_b;

    if (total_inv_mass < 0.0001f) return; // оба статические — не двигаем

    // Выталкивание: распределяем глубину пропорционально массе
    f32 move_a = result.depth * (inv_mass_a / total_inv_mass);
    f32 move_b = result.depth * (inv_mass_b / total_inv_mass);

    body_a.position = body_a.position - result.normal * move_a;
    body_b.position = body_b.position + result.normal * move_b;

    // Коррекция скорости (отражение вдоль нормали)
    f32 rel_vel_x = body_b.velocity.x - body_a.velocity.x;
    f32 rel_vel_y = body_b.velocity.y - body_a.velocity.y;
    f32 vel_along_normal = rel_vel_x * result.normal.x + rel_vel_y * result.normal.y;

    // Если тела уже расходятся — не меняем скорость
    if (vel_along_normal > 0.0f) return;

    // Импульс отскока (коэффициент восстановления = 0.2)
    f32 restitution = 0.2f;
    f32 impulse = -(1.0f + restitution) * vel_along_normal / total_inv_mass;

    te2d_vec2 impulse_vec = result.normal * impulse;

    body_a.apply_impulse(impulse_vec * -1.0f);
    body_b.apply_impulse(impulse_vec);
}

} // namespace te2d_collision