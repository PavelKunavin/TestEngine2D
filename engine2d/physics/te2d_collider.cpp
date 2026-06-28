#include <physics/te2d_collider.hpp>

te2d_collider te2d_collider::create_aabb(f32 width, f32 height, te2d_vec2 offset) {
    te2d_collider col = {};
    col.type = te2d_collider_type::aabb;
    col.offset = offset;
    col.size = {width, height};
    return col;
}

te2d_collider te2d_collider::create_circle(f32 radius, te2d_vec2 offset) {
    te2d_collider col = {};
    col.type = te2d_collider_type::circle;
    col.offset = offset;
    col.size = {radius, 0.0f};
    return col;
}

te2d_rect te2d_collider::get_world_aabb(const te2d_vec2& body_position) const {
    f32 half_w = size.x * 0.5f;
    f32 half_h = size.y * 0.5f;

    return {
        body_position.x + offset.x - half_w,
        body_position.y + offset.y - half_h,
        size.x,
        size.y
    };
}

void te2d_collider::get_world_circle(const te2d_vec2& body_position, te2d_vec2& out_center, f32& out_radius) const {
    out_center = {
        body_position.x + offset.x,
        body_position.y + offset.y
    };
    out_radius = size.x;
}