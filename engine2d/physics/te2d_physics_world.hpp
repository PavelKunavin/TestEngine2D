#pragma once
#include <physics/te2d_rigidbody.hpp>
#include <physics/te2d_collider.hpp>
#include <physics/te2d_joint.hpp>
#include <core/te2d_allocator.hpp>

constexpr u32 TE2D_MAX_BODIES  = 256;
constexpr u32 TE2D_MAX_JOINTS  = 64;

struct te2d_physics_world {
    te2d_vec2 gravity = {0.0f, 980.0f};
    f32 fixed_dt = 1.0f / 60.0f;

    // Создаёт мир. Аллокатор для внутренних массивов.
    static te2d_physics_world create(te2d_allocator& allocator);
    void destroy();

    void clear();
    i32 add_body(const te2d_rigidbody& body, const te2d_collider& collider);
    void add_joint(const te2d_joint& joint);
    void step();

    te2d_rigidbody* get_body(i32 index);
    const te2d_rigidbody* get_body(i32 index) const;
    const te2d_collider* get_collider(i32 index) const;
    u32 get_body_count() const;

private:
    te2d_rigidbody* m_bodies = nullptr;
    te2d_collider*  m_colliders = nullptr;
    u32 m_body_count = 0;

    te2d_joint* m_joints = nullptr;
    u32 m_joint_count = 0;

    void solve_collisions();
};