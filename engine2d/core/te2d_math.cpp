#include <core/te2d_math.hpp>
#include <cstring>

te2d_mat3 te2d_mat3::identity() {
    te2d_mat3 mat;
    std::memset(mat.m, 0, 9 * sizeof(f32));
    mat.m[0] = 1.0f;
    mat.m[4] = 1.0f;
    mat.m[8] = 1.0f;
    return mat;
}

te2d_mat3 te2d_mat3::translate(f32 x, f32 y) {
    te2d_mat3 mat = identity();
    mat.m[6] = x;
    mat.m[7] = y;
    return mat;
}

te2d_mat3 te2d_mat3::translate(const te2d_vec2& v) {
    return translate(v.x, v.y);
}

te2d_mat3 te2d_mat3::rotate(f32 angle) {
    te2d_mat3 mat = identity();
    f32 c = std::cos(angle);
    f32 s = std::sin(angle);
    mat.m[0] =  c; mat.m[1] =  s;
    mat.m[3] = -s; mat.m[4] =  c;
    return mat;
}

te2d_mat3 te2d_mat3::scale(f32 sx, f32 sy) {
    te2d_mat3 mat = identity();
    mat.m[0] = sx;
    mat.m[4] = sy;
    return mat;
}

te2d_mat3 te2d_mat3::scale(const te2d_vec2& v) {
    return scale(v.x, v.y);
}

te2d_mat3 te2d_mat3::operator*(const te2d_mat3& other) const {
    te2d_mat3 result;
    for (i32 col = 0; col < 3; ++col) {
        for (i32 row = 0; row < 3; ++row) {
            result.m[col * 3 + row] =
                m[0 * 3 + row] * other.m[col * 3 + 0] +
                m[1 * 3 + row] * other.m[col * 3 + 1] +
                m[2 * 3 + row] * other.m[col * 3 + 2];
        }
    }
    return result;
}

te2d_vec2 te2d_mat3::transform_point(const te2d_vec2& point) const {
    te2d_vec2 result;
    result.x = m[0] * point.x + m[3] * point.y + m[6];
    result.y = m[1] * point.x + m[4] * point.y + m[7];
    return result;
}