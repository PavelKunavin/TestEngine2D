#include <render/te2d_camera.hpp>
#include <cmath>
#include <cstring>

// фабрика 
te2d_camera te2d_camera::create(f32 viewport_width, f32 viewport_height) {
    te2d_camera cam = {};
    cam.width = viewport_width;
    cam.height = viewport_height;
    return cam;
}

void te2d_camera::get_projection(f32* out_mat4) const {
    // Обнуляем матрицу
    std::memset(out_mat4, 0, 16 * sizeof(f32));

    f32 half_w = width * 0.5f / zoom;
    f32 half_h = height * 0.5f / zoom;

    // Ортографическая проекция: лево, право, низ, верх, ближняя, дальняя
    f32 left   = -half_w;
    f32 right  =  half_w;
    f32 bottom = -half_h;  // инвертировано: Y вверх в мировом пространстве
    f32 top    =  half_h;
    f32 near   = -1.0f;
    f32 far    =  1.0f;

    // Заполняем матрицу 4x4 (column-major, как ожидает OpenGL)
    out_mat4[0]  = 2.0f / (right - left);
    out_mat4[5]  = 2.0f / (top - bottom);
    out_mat4[10] = -2.0f / (far - near);
    out_mat4[12] = -(right + left) / (right - left);
    out_mat4[13] = -(top + bottom) / (top - bottom);
    out_mat4[14] = -(far + near) / (far - near);
    out_mat4[15] = 1.0f;
}

void te2d_camera::get_view(f32* out_mat4) const {
    // Начинаем с единичной матрицы
    std::memset(out_mat4, 0, 16 * sizeof(f32));
    out_mat4[0]  = 1.0f;
    out_mat4[5]  = 1.0f;
    out_mat4[10] = 1.0f;
    out_mat4[15] = 1.0f;

    if (rotation != 0.0f) {
        f32 cos_r = std::cos(rotation);
        f32 sin_r = std::sin(rotation);

        out_mat4[0]  =  cos_r;
        out_mat4[1]  =  sin_r;
        out_mat4[4]  = -sin_r;
        out_mat4[5]  =  cos_r;
    }

    // Смещение: двигаем мир в противоположную сторону
    out_mat4[12] = -(out_mat4[0] * position.x + out_mat4[4] * position.y);
    out_mat4[13] = -(out_mat4[1] * position.x + out_mat4[5] * position.y);
}