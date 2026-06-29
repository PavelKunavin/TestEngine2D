#include <scene/te2d_entity.hpp>

te2d_entity te2d_entity::create() {
    te2d_entity e = {};
    return e;
}

te2d_entity te2d_entity::create_sprite(te2d_transform* t, te2d_sprite* s) {
    te2d_entity e = {};
    e.transform = t;
    e.sprite = s;
    e.visible = (s != nullptr);
    return e;
}

te2d_entity te2d_entity::create_physics(te2d_transform* t, te2d_rigidbody* rb, te2d_collider* col) {
    te2d_entity e = {};
    e.transform = t;
    e.rigidbody = rb;
    e.collider = col;
    return e;
}

te2d_entity te2d_entity::create_full(te2d_transform* t, te2d_sprite* s, te2d_rigidbody* rb, te2d_collider* col) {
    te2d_entity e = {};
    e.transform = t;
    e.sprite = s;
    e.rigidbody = rb;
    e.collider = col;
    e.visible = (s != nullptr);
    return e;
}

void te2d_entity::sync_physics_to_transform() {
    if (rigidbody && transform) {
        transform->position = rigidbody->position;
    }
}

void te2d_entity::sync_transform_to_physics() {
    if (rigidbody && transform) {
        rigidbody->position = transform->position;
    }
}