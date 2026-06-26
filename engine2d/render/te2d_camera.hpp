#pragma once
#include <te2d_types.hpp>

struct te2d_camera {
    te2d_vec2 position = {0.0f, 0.0f};   // центр камеры в мировых координатах
    f32 rotation = 0.0f;                  // поворот камеры в радианах
    f32 zoom = 1.0f;                      // зум (1.0 = без зума)
    f32 width = 1280.0f;                  // ширина viewport в пикселях
    f32 height = 720.0f;                  // высота viewport в пикселях

    // Создаёт камеру с ортографической проекцией
    static te2d_camera create(f32 viewport_width, f32 viewport_height);

    // Возвращает матрицу проекции (ортографическую)
    void get_projection(f32* out_mat4) const;

    // Возвращает матрицу вида (перемещение и поворот)
    void get_view(f32* out_mat4) const;
};