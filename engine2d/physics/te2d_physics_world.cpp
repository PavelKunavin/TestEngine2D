#include <physics/te2d_physics_world.hpp>
#include <physics/te2d_collision.hpp>

te2d_physics_world te2d_physics_world::create(te2d_allocator& allocator) {
    te2d_physics_world world = {};

    world.m_bodies    = allocator.alloc_type<te2d_rigidbody>(TE2D_MAX_BODIES);
    world.m_colliders = allocator.alloc_type<te2d_collider>(TE2D_MAX_BODIES);
    world.m_joints    = allocator.alloc_type<te2d_joint>(TE2D_MAX_JOINTS);

    return world;
}

void te2d_physics_world::destroy() {
    // Память освободится при destroy аллокатора
    m_bodies = nullptr;
    m_colliders = nullptr;
    m_joints = nullptr;
}

void te2d_physics_world::clear() {
    m_body_count = 0;
    m_joint_count = 0;
}

i32 te2d_physics_world::add_body(const te2d_rigidbody& body, const te2d_collider& collider) {
    if (!m_bodies || m_body_count >= TE2D_MAX_BODIES) return -1;

    i32 index = (i32)m_body_count;
    m_bodies[index] = body;
    m_colliders[index] = collider;
    ++m_body_count;

    return index;
}

void te2d_physics_world::add_joint(const te2d_joint& joint) {
    if (!m_joints || m_joint_count >= TE2D_MAX_JOINTS) return;

    m_joints[m_joint_count] = joint;
    ++m_joint_count;
}

te2d_rigidbody* te2d_physics_world::get_body(i32 index) {
    if (!m_bodies || index < 0 || index >= (i32)m_body_count) return nullptr;
    return &m_bodies[index];
}

const te2d_rigidbody* te2d_physics_world::get_body(i32 index) const {
    if (!m_bodies || index < 0 || index >= (i32)m_body_count) return nullptr;
    return &m_bodies[index];
}

const te2d_collider* te2d_physics_world::get_collider(i32 index) const {
    if (!m_colliders || index < 0 || index >= (i32)m_body_count) return nullptr;
    return &m_colliders[index];
}

u32 te2d_physics_world::get_body_count() const {
    return m_body_count;
}

void te2d_physics_world::step() {
    if (!m_bodies) return;

    for (u32 i = 0; i < m_body_count; ++i) {
        te2d_rigidbody& body = m_bodies[i];
        if (body.type == te2d_body_type::static_body) continue;
        if (body.affected_by_gravity) {
            body.apply_force(gravity * body.mass);
        }
    }

    for (u32 i = 0; i < m_body_count; ++i) {
        m_bodies[i].integrate(fixed_dt);
    }

    solve_collisions();

    if (m_joints) {
        for (u32 i = 0; i < m_joint_count; ++i) {
            m_joints[i].solve(m_bodies, m_body_count);
        }
    }
}

void te2d_physics_world::solve_collisions() {
    if (!m_bodies) return;

    for (u32 i = 0; i < m_body_count; ++i) {
        for (u32 j = i + 1; j < m_body_count; ++j) {
            te2d_rigidbody& body_a = m_bodies[i];
            te2d_rigidbody& body_b = m_bodies[j];

            if (body_a.type == te2d_body_type::static_body &&
                body_b.type == te2d_body_type::static_body) continue;

            const te2d_collider& col_a = m_colliders[i];
            const te2d_collider& col_b = m_colliders[j];

            te2d_collision_result result = te2d_collision::detect(
                body_a.position, col_a,
                body_b.position, col_b);

            if (result.hit) {
                te2d_collision::resolve(body_a, body_b, result);
            }
        }
    }
}