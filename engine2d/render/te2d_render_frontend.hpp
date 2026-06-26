#pragma once
#include <render/te2d_camera.hpp>
#include <render/te2d_sprite_batch.hpp>

void te2d_render_init(f32 viewport_width, f32 viewport_height);
void te2d_render_shutdown();
void te2d_render_begin_frame(const te2d_camera& camera);
void te2d_render_end_frame();
void te2d_render_draw_sprite(const te2d_sprite& sprite);
void te2d_render_flush();