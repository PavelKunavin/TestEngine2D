#pragma once
#include <te2d_types.hpp>
#include <render/te2d_texture.hpp>

struct te2d_sprite {
    te2d_texture* texture = nullptr;
    te2d_rect uv_region;
    te2d_vec2 position;
    te2d_vec2 origin;
    f32 rotation = 0.0f;
    te2d_vec2 scale = {1.0f, 1.0f};
    te2d_color color = {1.0f, 1.0f, 1.0f, 1.0f};
    te2d_layer layer = te2d_layers::ENTITIES;
    bool flip_x = false;
    bool flip_y = false;
    bool visible = true;

    // Фабрика: создаёт спрайт с текстурой, UV-регион — вся текстура, origin — центр
    static te2d_sprite create(te2d_texture* texture) {
        te2d_sprite result = {};
        result.texture = texture;
        result.visible = true;
        result.color = {1.0f, 1.0f, 1.0f, 1.0f};
        result.scale = {1.0f, 1.0f};

        if (texture && texture->id) {
            result.uv_region = te2d_rect{0.0f, 0.0f, (f32)texture->width, (f32)texture->height};
            result.origin = {
                (f32)texture->width * 0.5f,
                (f32)texture->height * 0.5f
            };
        }

        return result;
    }

    // Устанавливает UV-регион в пикселях текстуры
    void set_frame(te2d_rect pixel_region) {
        if (!texture || !texture->id) return;
        uv_region = pixel_region;
    }

    // Устанавливает UV-регион из нормализованных координат (0.0–1.0)
    void set_frame_normalized(f32 u0, f32 v0, f32 u1, f32 v1) {
        if (!texture || !texture->id) return;

        f32 tw = (f32)texture->width;
        f32 th = (f32)texture->height;

        uv_region = te2d_rect{
            u0 * tw,
            v0 * th,
            (u1 - u0) * tw,
            (v1 - v0) * th
        };
    }

    // Устанавливает размер спрайта в мировых единицах
    void set_size(f32 width, f32 height) {
        if (!texture || !texture->id) return;

        f32 tw = (f32)texture->width;
        f32 th = (f32)texture->height;

        scale = {width / tw, height / th};
    }

    // Ставит origin в центр UV-региона
    void center_origin() {
        origin = {
            uv_region.w * 0.5f,
            uv_region.h * 0.5f
        };
    }
};