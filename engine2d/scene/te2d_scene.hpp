#pragma once
#include <scene/te2d_entity.hpp>
#include <core/te2d_allocator.hpp>

constexpr u64 TE2D_SCENE_MEMORY = 1024 * 1024; // 1 MB на сцену

// Сцена — контейнер всех игровых объектов
struct te2d_scene {
    te2d_allocator allocator;

    // Создаёт пустую сцену с аллокатором
    static te2d_scene create();

    // Удаляет все объекты и аллокатор
    void destroy();

    // Удаляет все объекты (сбрасывает аллокатор)
    void clear();

    // Создаёт объект с компонентами (возвращает индекс)
    i32 add_entity(const te2d_entity& entity);

    // Создаёт компоненты внутри аллокатора
    te2d_transform* create_transform(const te2d_vec2& pos = {0.0f, 0.0f});
    te2d_sprite*    create_sprite(te2d_texture* tex);
    te2d_rigidbody* create_rigidbody(te2d_body_type type, f32 mass = 1.0f);
    te2d_collider*  create_collider(te2d_collider_type type, f32 w, f32 h);

    // Доступ
    te2d_entity* get_entity(i32 index);
    u32 get_entity_count() const;

    // Обновление и отрисовка
    void update(f32 dt);
    void draw();

    // Синхронизация transform ↔ rigidbody
    void sync_physics_to_transforms();
    void sync_transforms_to_physics();

private:
    te2d_entity* m_entities = nullptr;
    u32 m_entity_count = 0;
    u32 m_entity_capacity = 512; // начальная ёмкость
};