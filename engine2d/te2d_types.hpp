#pragma once
#include <cstdint>
#include <cmath>

using f32 = float;
using f64 = double;
using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

static_assert(sizeof(f32) == 4, "f32 must be 4 bytes");
static_assert(sizeof(f64) == 8, "f64 must be 8 bytes");
static_assert(sizeof(u8)  == 1, "u8 must be 1 byte");
static_assert(sizeof(u16) == 2, "u16 must be 2 bytes");
static_assert(sizeof(u32) == 4, "u32 must be 4 bytes");
static_assert(sizeof(u64) == 8, "u64 must be 8 bytes");
static_assert(sizeof(i8)  == 1, "i8 must be 1 byte");
static_assert(sizeof(i16) == 2, "i16 must be 2 bytes");
static_assert(sizeof(i32) == 4, "i32 must be 4 bytes");
static_assert(sizeof(i64) == 8, "i64 must be 8 bytes");

// ===== 2D Math Types =====

struct te2d_vec2 {
    f32 x = 0.0f;
    f32 y = 0.0f;

    te2d_vec2() = default;
    te2d_vec2(f32 x, f32 y) : x(x), y(y) {}

    te2d_vec2 operator+(const te2d_vec2& other) const { return {x + other.x, y + other.y}; }
    te2d_vec2 operator-(const te2d_vec2& other) const { return {x - other.x, y - other.y}; }
    te2d_vec2 operator*(f32 scalar) const { return {x * scalar, y * scalar}; }
    te2d_vec2 operator/(f32 scalar) const { return {x / scalar, y / scalar}; }

    te2d_vec2& operator+=(const te2d_vec2& other) { x += other.x; y += other.y; return *this; }
    te2d_vec2& operator-=(const te2d_vec2& other) { x -= other.x; y -= other.y; return *this; }
    te2d_vec2& operator*=(f32 scalar) { x *= scalar; y *= scalar; return *this; }

    f32 length_sq() const { return x * x + y * y; }
    f32 length() const { return std::sqrt(length_sq()); }

    te2d_vec2 normalized() const {
        f32 len = length();
        return (len > 0.0f) ? *this / len : te2d_vec2{0.0f, 0.0f};
    }

    f32 dot(const te2d_vec2& other) const { return x * other.x + y * other.y; }
};

struct te2d_rect {
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 w = 0.0f;
    f32 h = 0.0f;

    te2d_rect() = default;
    te2d_rect(f32 x, f32 y, f32 w, f32 h) : x(x), y(y), w(w), h(h) {}

    static te2d_rect from_min_max(f32 min_x, f32 min_y, f32 max_x, f32 max_y) {
        return {min_x, min_y, max_x - min_x, max_y - min_y};
    }

    f32 left()   const { return x; }
    f32 right()  const { return x + w; }
    f32 top()    const { return y; }
    f32 bottom() const { return y + h; }

    te2d_vec2 center() const { return {x + w * 0.5f, y + h * 0.5f}; }

    bool contains(f32 px, f32 py) const {
        return px >= x && px <= x + w && py >= y && py <= y + h;
    }

    bool contains(const te2d_vec2& point) const {
        return contains(point.x, point.y);
    }
};

struct te2d_color {
    f32 r = 1.0f;
    f32 g = 1.0f;
    f32 b = 1.0f;
    f32 a = 1.0f;

    te2d_color() = default;
    te2d_color(f32 r, f32 g, f32 b, f32 a = 1.0f) : r(r), g(g), b(b), a(a) {}

    static te2d_color from_rgba(u32 hex) {
        return {
            ((hex >> 24) & 0xFF) / 255.0f,
            ((hex >> 16) & 0xFF) / 255.0f,
            ((hex >> 8)  & 0xFF) / 255.0f,
            ((hex)       & 0xFF) / 255.0f
        };
    }

    static te2d_color from_rgb(u32 hex) {
        return from_rgba((hex << 8) | 0xFF);
    }
};

// ===== Layer =====
using te2d_layer = i32;

namespace te2d_layers {
    constexpr te2d_layer DEEP_BACKGROUND     = 0;    // Небо, далёкие здания
    constexpr te2d_layer PARALLAX_BACK       = 5;    // Параллакс-слои
    constexpr te2d_layer MID_BACKGROUND      = 10;   // Архитектура, крупный фон
    constexpr te2d_layer BACKGROUND_DETAILS  = 15;   // Декор позади (кабели, трубы)
    constexpr te2d_layer TERRAIN             = 20;   // Платформы, стены, пол
    constexpr te2d_layer SHADOWS             = 25;   // Тени от персонажа и врагов
    constexpr te2d_layer ENTITIES            = 30;   // Персонаж, враги, NPC, предметы
    constexpr te2d_layer BEHIND_FX           = 35;   // Пыль, фоновые искры
    constexpr te2d_layer FOREGROUND          = 40;   // Передний план (трубы, решётки)
    constexpr te2d_layer FRONT_FX            = 45;   // Взрывы, ударные искры, неон
    constexpr te2d_layer LIGHTING            = 50;   // Bloom, glow, световые эффекты
    constexpr te2d_layer UI                  = 60;   // HUD, здоровье, патроны
    constexpr te2d_layer DEBUG               = 100;  // Хитбоксы, FPS
}