#include <scene/te2d_scene.hpp>
#include <render/te2d_render_frontend.hpp>

te2d_scene te2d_scene::create() {
    te2d_scene scene = {};
    scene.allocator = te2d_allocator::create(TE2D_SCENE_MEMORY);

    // Выделяем массив entity
    scene.m_entities = scene.allocator.alloc_type<te2d_entity>(512);
    scene.m_entity_capacity = 512;

    return scene;
}

void te2d_scene::destroy() {
    allocator.destroy();
    m_entities = nullptr;
    m_entity_count = 0;
    m_entity_capacity = 0;
}

void te2d_scene::clear() {
    m_entity_count = 0;
    allocator.reset();

    // Перевыделяем массив entity после сброса
    m_entities = allocator.alloc_type<te2d_entity>(m_entity_capacity);
}

te2d_transform* te2d_scene::create_transform(const te2d_vec2& pos) {
    te2d_transform* t = allocator.alloc_type<te2d_transform>();
    if (!t) return nullptr;
    *t = te2d_transform::create(pos);
    return t;
}

te2d_sprite* te2d_scene::create_sprite(te2d_texture* tex) {
    te2d_sprite* s = allocator.alloc_type<te2d_sprite>();
    if (!s) return nullptr;
    *s = te2d_sprite::create(tex);
    return s;
}

te2d_rigidbody* te2d_scene::create_rigidbody(te2d_body_type type, f32 mass) {
    te2d_rigidbody* rb = allocator.alloc_type<te2d_rigidbody>();
    if (!rb) return nullptr;
    if (type == te2d_body_type::static_body) {
        *rb = te2d_rigidbody::create_static();
    } else {
        *rb = te2d_rigidbody::create_dynamic(mass);
    }
    return rb;
}

te2d_collider* te2d_scene::create_collider(te2d_collider_type type, f32 w, f32 h) {
    te2d_collider* c = allocator.alloc_type<te2d_collider>();
    if (!c) return nullptr;
    if (type == te2d_collider_type::aabb) {
        *c = te2d_collider::create_aabb(w, h);
    } else {
        *c = te2d_collider::create_circle(w);
    }
    return c;
}

i32 te2d_scene::add_entity(const te2d_entity& entity) {
    if (m_entity_count >= m_entity_capacity) return -1;

    i32 index = (i32)m_entity_count;
    m_entities[index] = entity;
    ++m_entity_count;
    return index;
}

te2d_entity* te2d_scene::get_entity(i32 index) {
    if (index < 0 || index >= (i32)m_entity_count) return nullptr;
    return &m_entities[index];
}

u32 te2d_scene::get_entity_count() const {
    return m_entity_count;
}

void te2d_scene::update(f32 dt) {
    (void)dt;
}

void te2d_scene::draw() {
    for (u32 i = 0; i < m_entity_count; ++i) {
        const te2d_entity& e = m_entities[i];
        if (!e.active || !e.visible) continue;
        if (!e.sprite) continue;

        if (e.transform) {
            e.sprite->position = e.transform->position;
            e.sprite->rotation = e.transform->rotation;
            e.sprite->scale = e.transform->scale;
        }

        te2d_render_draw_sprite(*e.sprite);
    }
}

void te2d_scene::sync_physics_to_transforms() {
    for (u32 i = 0; i < m_entity_count; ++i) {
        m_entities[i].sync_physics_to_transform();
    }
}

void te2d_scene::sync_transforms_to_physics() {
    for (u32 i = 0; i < m_entity_count; ++i) {
        m_entities[i].sync_transform_to_physics();
    }
}