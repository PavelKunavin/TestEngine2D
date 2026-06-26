#pragma once
#include <cstdio>

// ============================================================================
// Уровни логирования
// ============================================================================

enum class te2d_log_level {
    none  = 0,
    error = 1,
    warn  = 2,
    info  = 3,
    debug = 4,
};

// Текущий уровень (определён в te2d_log.cpp)
extern te2d_log_level g_te2d_log_level;

// ============================================================================
// Макросы
// ============================================================================

#define TE2D_LOG(level, category, fmt, ...) \
    do { \
        if ((int)level <= (int)g_te2d_log_level) { \
            FILE* f = fopen("debug2d.log", "a"); \
            if (f) { \
                fprintf(f, "[%s] [%s] " fmt "\n", #level, category, ##__VA_ARGS__); \
                fclose(f); \
            } \
        } \
    } while(0)

#define TE2D_ERROR(cat, fmt, ...)   TE2D_LOG(te2d_log_level::error, cat, fmt, ##__VA_ARGS__)
#define TE2D_WARN(cat, fmt, ...)    TE2D_LOG(te2d_log_level::warn,  cat, fmt, ##__VA_ARGS__)
#define TE2D_INFO(cat, fmt, ...)    TE2D_LOG(te2d_log_level::info,  cat, fmt, ##__VA_ARGS__)
#define TE2D_DEBUG(cat, fmt, ...)   TE2D_LOG(te2d_log_level::debug, cat, fmt, ##__VA_ARGS__)