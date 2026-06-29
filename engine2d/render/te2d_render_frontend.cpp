#include <render/te2d_render_frontend.hpp>
#include <render/backends/te2d_render_backend_opengl.hpp>
#include <render/te2d_render_frontend.hpp>
#include <render/backends/te2d_render_backend_opengl.hpp>
#include <core/te2d_allocator.hpp>

static te2d_allocator g_batch_allocator; // аллокатор для батча

static te2d_sprite_batch g_batch;


void te2d_render_init(f32 viewport_width, f32 viewport_height) {
    g_batch_allocator = te2d_allocator::create(1024 * 1024); // 1 MB
    g_batch = te2d_sprite_batch::create(g_batch_allocator);
    te2d_render_backend_opengl_init(viewport_width, viewport_height);
}

void te2d_render_shutdown() {
    g_batch.destroy();
    g_batch_allocator.destroy();
    te2d_render_backend_opengl_shutdown();
}


void te2d_render_begin_frame(const te2d_camera& camera) {
    te2d_render_backend_opengl_begin_frame(camera);
    g_batch.begin();
}

void te2d_render_end_frame() {
    te2d_render_flush();
    te2d_render_backend_opengl_end_frame();
}

void te2d_render_draw_sprite(const te2d_sprite& sprite) {
    g_batch.draw(sprite);
}

void te2d_render_flush() {
    te2d_render_backend_opengl_flush(g_batch);
}