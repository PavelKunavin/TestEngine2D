#include <physics/te2d_joint.hpp>
#include <physics/te2d_rigidbody.hpp>
#include <cmath>

te2d_joint te2d_joint::create_distance(i32 body_a, i32 body_b,
                                        te2d_vec2 anchor_a, te2d_vec2 anchor_b,
                                        f32 target_distance, f32 stiffness, f32 damping) {
    te2d_joint joint = {};
    joint.type = te2d_joint_type::distance;
    joint.body_a = body_a;
    joint.body_b = body_b;
    joint.anchor_a = anchor_a;
    joint.anchor_b = anchor_b;
    joint.target_distance = target_distance;
    joint.stiffness = stiffness;
    joint.damping = damping;
    return joint;
}

te2d_joint te2d_joint::create_revolute(i32 body_a, i32 body_b,
                                        te2d_vec2 world_anchor,
                                        f32 lower, f32 upper) {
    te2d_joint joint = {};
    joint.type = te2d_joint_type::revolute;
    joint.body_a = body_a;
    joint.body_b = body_b;
    joint.anchor_a = world_anchor;
    joint.anchor_b = world_anchor;
    joint.enable_limit = true;
    joint.lower_angle = lower;
    joint.upper_angle = upper;
    return joint;
}

void te2d_joint::solve(te2d_rigidbody* bodies, u32 body_count) {
    bool a_valid = (body_a >= 0 && body_a < (i32)body_count);
    bool b_valid = (body_b >= 0 && body_b < (i32)body_count);

    if (!a_valid && !b_valid) return;

    switch (type) {
        // ====================================
        // Distance Joint
        // ====================================
        case te2d_joint_type::distance: {
            te2d_vec2 world_a = anchor_a;
            te2d_vec2 world_b = anchor_b;

            if (a_valid) world_a = bodies[body_a].position + anchor_a;
            if (b_valid) world_b = bodies[body_b].position + anchor_b;

            te2d_vec2 delta = world_b - world_a;
            f32 current_distance = delta.length();

            if (current_distance < 0.0001f) break;

            te2d_vec2 normal = delta / current_distance;
            f32 error = current_distance - target_distance;

            f32 spring_force = stiffness * error;

            f32 rel_vel = 0.0f;
            if (a_valid) rel_vel -= bodies[body_a].velocity.x * normal.x + bodies[body_a].velocity.y * normal.y;
            if (b_valid) rel_vel += bodies[body_b].velocity.x * normal.x + bodies[body_b].velocity.y * normal.y;

            spring_force += damping * rel_vel;

            te2d_vec2 impulse = normal * spring_force;

            if (a_valid) bodies[body_a].apply_impulse(impulse);
            if (b_valid) bodies[body_b].apply_impulse(impulse * -1.0f);
            break;
        }

        // ====================================
        // Revolute Joint
        // ====================================
        case te2d_joint_type::revolute: {
            if (!a_valid) break;

            te2d_rigidbody& body = bodies[body_a];

            te2d_vec2 world_anchor = anchor_a;
            if (b_valid) world_anchor = bodies[body_b].position + anchor_b;

            te2d_vec2 delta = world_anchor - body.position;
            f32 dist = delta.length();

            if (dist > 0.0001f) {
                te2d_vec2 normal = delta / dist;
                body.position = world_anchor;

                f32 vel_along = body.velocity.x * normal.x + body.velocity.y * normal.y;
                if (vel_along > 0.0f) {
                    body.velocity.x -= normal.x * vel_along;
                    body.velocity.y -= normal.y * vel_along;
                }
            }

            // TODO: ограничение угла при добавлении rotation в rigidbody
            break;
        }
    }
}