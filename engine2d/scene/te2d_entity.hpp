#pragma once
#include <te2d_types.hpp>
#include <scene/te2d_transform.hpp>
#include <render/te2d_sprite.hpp>
#include <physics/te2d_rigidbody.hpp>
#include <physics/te2d_collider.hpp>

// Игровой объект — контейнер компонентов
struct te2d_entity {
    te2d_transform* transform = nullptr;
    te2d_sprite*    sprite    = nullptr;
    te2d_rigidbody* rigidbody = nullptr;
    te2d_collider*  collider  = nullptr;

    // Флаги
    bool active = true;  // обновлять и рисовать?
    bool visible = true; // рисовать?

    // Создаёт пустой entity
    static te2d_entity create();

    // Удобные конструкторы
    static te2d_entity create_sprite(te2d_transform* t, te2d_sprite* s);
    static te2d_entity create_physics(te2d_transform* t, te2d_rigidbody* rb, te2d_collider* col);
    static te2d_entity create_full(te2d_transform* t, te2d_sprite* s, te2d_rigidbody* rb, te2d_collider* col);

    // Синхронизация transform ↔ rigidbody (после шага физики)
    void sync_physics_to_transform();
    void sync_transform_to_physics();
};