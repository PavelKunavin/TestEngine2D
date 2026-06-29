#pragma once
#include <te2d_types.hpp>
#include <cmath>

// ============================================================================
// Матрица 3×3 (column-major, как OpenGL)
// ============================================================================

struct te2d_mat3 {
    f32 m[9]; // [0-2] первый столбец, [3-5] второй, [6-8] третий

    // Единичная матрица
    static te2d_mat3 identity();

    // Матрица переноса
    static te2d_mat3 translate(f32 x, f32 y);
    static te2d_mat3 translate(const te2d_vec2& v);

    // Матрица поворота (радианы)
    static te2d_mat3 rotate(f32 angle);

    // Матрица масштаба
    static te2d_mat3 scale(f32 sx, f32 sy);
    static te2d_mat3 scale(const te2d_vec2& v);

    // Умножение матриц: this * other
    te2d_mat3 operator*(const te2d_mat3& other) const;

    // Трансформация точки (умножение на вектор)
    te2d_vec2 transform_point(const te2d_vec2& point) const;
};