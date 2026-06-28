#include <physics/te2d_rigidbody.hpp>

te2d_rigidbody te2d_rigidbody::create_dynamic(f32 mass, f32 drag) {
    te2d_rigidbody body = {};
    body.type = te2d_body_type::dynamic_body;
    body.mass = mass;
    body.drag = drag;
    body.affected_by_gravity = true;
    return body;
}

te2d_rigidbody te2d_rigidbody::create_static() {
    te2d_rigidbody body = {};
    body.type = te2d_body_type::static_body;
    body.mass = 0.0f;
    body.affected_by_gravity = false;
    return body;
}

void te2d_rigidbody::apply_force(const te2d_vec2& force) {
    if (type == te2d_body_type::static_body || mass <= 0.0f) return;
    acceleration += force / mass;
}

void te2d_rigidbody::apply_impulse(const te2d_vec2& impulse) {
    if (type == te2d_body_type::static_body || mass <= 0.0f) return;
    velocity += impulse / mass;
}

void te2d_rigidbody::integrate(f32 dt) {
    if (type == te2d_body_type::static_body) return;

    // Затухание
    velocity.x *= (1.0f - drag * dt);
    velocity.y *= (1.0f - drag * dt);

    // Интеграция: v += a * dt, p += v * dt
    velocity += acceleration * dt;
    position += velocity * dt;

    // Сбрасываем ускорение на следующий кадр
    acceleration = {0.0f, 0.0f};
}