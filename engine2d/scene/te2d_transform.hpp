#pragma once
#include <te2d_types.hpp>
#include <core/te2d_math.hpp>

struct te2d_transform {
    te2d_vec2 position;
    f32 rotation = 0.0f;
    te2d_vec2 scale = {1.0f, 1.0f};

    static te2d_transform create(const te2d_vec2& pos = {0.0f, 0.0f});

    // Матрица model как te2d_mat3
    te2d_mat3 get_model_matrix() const;
};