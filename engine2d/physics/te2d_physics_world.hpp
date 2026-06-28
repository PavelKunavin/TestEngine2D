#pragma once
#include <physics/te2d_rigidbody.hpp>
#include <physics/te2d_collider.hpp>
#include <physics/te2d_joint.hpp>

constexpr u32 TE2D_MAX_BODIES  = 256;
constexpr u32 TE2D_MAX_JOINTS  = 64;

struct te2d_physics_world {
    te2d_vec2 gravity = {0.0f, 980.0f};   // гравитация (пиксели/сек²)
    f32 fixed_dt = 1.0f / 60.0f;          // фиксированный шаг симуляции

    // Создаёт физический мир
    static te2d_physics_world create();

    // Сброс мира
    void clear();

    // Добавляет тело (возвращает индекс)
    i32 add_body(const te2d_rigidbody& body, const te2d_collider& collider);

    // Добавляет джойнт
    void add_joint(const te2d_joint& joint);

    // Выполняет один шаг симуляции
    void step();

    // Доступ к телам и коллайдерам
    te2d_rigidbody* get_body(i32 index);
    const te2d_rigidbody* get_body(i32 index) const;
    const te2d_collider* get_collider(i32 index) const;

    u32 get_body_count() const;

private:
    te2d_rigidbody m_bodies[TE2D_MAX_BODIES];
    te2d_collider m_colliders[TE2D_MAX_BODIES];
    u32 m_body_count = 0;

    te2d_joint m_joints[TE2D_MAX_JOINTS];
    u32 m_joint_count = 0;

    void solve_collisions();
};