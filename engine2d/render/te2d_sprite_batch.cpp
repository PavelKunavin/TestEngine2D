#include <render/te2d_sprite_batch.hpp>
#include <glad/glad.h>
#include <cstring>

static constexpr u32 VERTICES_PER_SPRITE = 4;
static constexpr u32 INDICES_PER_SPRITE  = 6;
static constexpr u32 MAX_VERTICES = TE2D_MAX_SPRITES * VERTICES_PER_SPRITE;
static constexpr u32 MAX_INDICES  = TE2D_MAX_SPRITES * INDICES_PER_SPRITE;

static te2d_sprite_batch::vertex vertex_buffer[MAX_VERTICES];
static u32 vertex_buffer_count = 0;

static u32 index_buffer[MAX_INDICES];

static void init_indices() {
    static bool done = false;
    if (done) return;
    done = true;

    for (u32 i = 0; i < TE2D_MAX_SPRITES; ++i) {
        u32 v = i * VERTICES_PER_SPRITE;
        u32 idx = i * INDICES_PER_SPRITE;

        index_buffer[idx + 0] = v + 0;
        index_buffer[idx + 1] = v + 1;
        index_buffer[idx + 2] = v + 2;
        index_buffer[idx + 3] = v + 0;
        index_buffer[idx + 4] = v + 2;
        index_buffer[idx + 5] = v + 3;
    }
}

te2d_sprite_batch te2d_sprite_batch::create() {
    te2d_sprite_batch batch = {};
    init_indices();

    glGenVertexArrays(1, &batch.vao);
    glBindVertexArray(batch.vao);

    glGenBuffers(1, &batch.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, batch.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer), nullptr, GL_DYNAMIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer), index_buffer, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(4 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    return batch;
}

void te2d_sprite_batch::destroy() {
    if (vao) { glDeleteVertexArrays(1, &vao); vao = 0; }
    if (vbo) { glDeleteBuffers(1, &vbo); vbo = 0; }
}

void te2d_sprite_batch::begin() {
    sprite_count = 0;
    vertex_buffer_count = 0;
    current_texture_id = 0;
}

void te2d_sprite_batch::draw(const te2d_sprite& sprite) {
    if (!sprite.visible || !sprite.texture || !sprite.texture->id) return;

    // Если текстура сменилась — сбрасываем батч
    if (sprite_count > 0 && sprite.texture->id != current_texture_id) {
        flush();
    }
    current_texture_id = sprite.texture->id;

    if (sprite_count >= TE2D_MAX_SPRITES) {
        flush();
    }

    f32 px = sprite.position.x;
    f32 py = sprite.position.y;
    f32 ox = sprite.origin.x;
    f32 oy = sprite.origin.y;
    f32 sx = sprite.scale.x;
    f32 sy = sprite.scale.y;
    f32 r  = sprite.rotation;

    f32 rw = sprite.uv_region.w;
    f32 rh = sprite.uv_region.h;

    f32 tw = (f32)sprite.texture->width;
    f32 th = (f32)sprite.texture->height;

    f32 u0 = sprite.uv_region.x / tw;
    f32 v0 = (sprite.uv_region.y + rh) / th;
    f32 u1 = (sprite.uv_region.x + rw) / tw;
    f32 v1 = sprite.uv_region.y / th;

    if (sprite.flip_x) { f32 tmp = u0; u0 = u1; u1 = tmp; }
    if (sprite.flip_y) { f32 tmp = v0; v0 = v1; v1 = tmp; }

    f32 cr = sprite.color.r;
    f32 cg = sprite.color.g;
    f32 cb = sprite.color.b;
    f32 ca = sprite.color.a;

    f32 left   = -ox * sx;
    f32 right  = (rw - ox) * sx;
    f32 top    = -oy * sy;
    f32 bottom = (rh - oy) * sy;

    f32 cos_r = 1.0f;
    f32 sin_r = 0.0f;
    if (r != 0.0f) {
        cos_r = std::cos(r);
        sin_r = std::sin(r);
    }

    vertex* v = &vertex_buffer[vertex_buffer_count];

    // v0: левый верх
    {
        f32 lx = left;
        f32 ly = top;
        v[0].x = px + lx * cos_r - ly * sin_r;
        v[0].y = py + lx * sin_r + ly * cos_r;
        v[0].u = u0; v[0].v = v0;
        v[0].r = cr; v[0].g = cg; v[0].b = cb; v[0].a = ca;
    }
    // v1: правый верх
    {
        f32 lx = right;
        f32 ly = top;
        v[1].x = px + lx * cos_r - ly * sin_r;
        v[1].y = py + lx * sin_r + ly * cos_r;
        v[1].u = u1; v[1].v = v0;
        v[1].r = cr; v[1].g = cg; v[1].b = cb; v[1].a = ca;
    }
    // v2: правый низ
    {
        f32 lx = right;
        f32 ly = bottom;
        v[2].x = px + lx * cos_r - ly * sin_r;
        v[2].y = py + lx * sin_r + ly * cos_r;
        v[2].u = u1; v[2].v = v1;
        v[2].r = cr; v[2].g = cg; v[2].b = cb; v[2].a = ca;
    }
    // v3: левый низ
    {
        f32 lx = left;
        f32 ly = bottom;
        v[3].x = px + lx * cos_r - ly * sin_r;
        v[3].y = py + lx * sin_r + ly * cos_r;
        v[3].u = u0; v[3].v = v1;
        v[3].r = cr; v[3].g = cg; v[3].b = cb; v[3].a = ca;
    }

    vertex_buffer_count += VERTICES_PER_SPRITE;
    ++sprite_count;
}

void te2d_sprite_batch::upload() {
    if (vertex_buffer_count == 0) return;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_buffer_count * sizeof(vertex), vertex_buffer);
}

void te2d_sprite_batch::flush() {
    if (sprite_count == 0) return;

    upload();

    // Биндим текстуру текущего батча
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, current_texture_id);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, sprite_count * INDICES_PER_SPRITE, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    sprite_count = 0;
    vertex_buffer_count = 0;
}

const te2d_sprite_batch::vertex* te2d_sprite_batch::get_vertex_buffer() const {
    return vertex_buffer;
}

u32 te2d_sprite_batch::get_vertex_count() const {
    return vertex_buffer_count;
}