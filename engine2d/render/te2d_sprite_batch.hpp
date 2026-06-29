#pragma once
#include <render/te2d_sprite.hpp>
#include <core/te2d_allocator.hpp>

constexpr u32 TE2D_MAX_SPRITES = 1024;

struct te2d_sprite_batch {
    struct vertex {
        f32 x, y;
        f32 u, v;
        f32 r, g, b, a;
    };

    u32 vao = 0;
    u32 vbo = 0;
    u32 sprite_count = 0;
    u32 current_texture_id = 0;

    // Буферы теперь через указатели (выделяются в аллокаторе)
    vertex* vertex_buffer = nullptr;
    u32* index_buffer = nullptr;
    u32 vertex_buffer_count = 0;
    u32 max_vertices = 0;

    // Создаёт батч. Принимает аллокатор для буферов.
    static te2d_sprite_batch create(te2d_allocator& allocator);

    void destroy();
    void begin();
    void draw(const te2d_sprite& sprite);
    void flush();
    void upload();

    const vertex* get_vertex_buffer() const;
    u32 get_vertex_count() const;
};