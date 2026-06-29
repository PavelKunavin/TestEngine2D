#include <scene/te2d_transform.hpp>

te2d_transform te2d_transform::create(const te2d_vec2& pos) {
    te2d_transform t = {};
    t.position = pos;
    return t;
}

te2d_mat3 te2d_transform::get_model_matrix() const {
    te2d_mat3 t = te2d_mat3::translate(position);
    te2d_mat3 r = te2d_mat3::rotate(rotation);
    te2d_mat3 s = te2d_mat3::scale(scale);
    return t * r * s; // сначала масштаб, потом поворот, потом перенос
}