#pragma once
#include <render/te2d_camera.hpp>
#include <render/te2d_sprite_batch.hpp>

void te2d_render_backend_opengl_init(f32 viewport_width, f32 viewport_height);
void te2d_render_backend_opengl_shutdown();

void te2d_render_backend_opengl_begin_frame(const te2d_camera& camera);
void te2d_render_backend_opengl_end_frame();

void te2d_render_backend_opengl_flush(te2d_sprite_batch& batch);