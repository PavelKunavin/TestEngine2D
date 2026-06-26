#include <render/backends/te2d_render_backend_opengl.hpp>
#include <glad/glad.h>
#include <core/te2d_log.hpp>

// ============================================================================
// Внутреннее состояние бэкенда
// ============================================================================

static u32 g_shader_program = 0;
static i32 g_u_projection   = -1;
static i32 g_u_view         = -1;
static i32 g_u_texture0     = -1;

// ============================================================================
// Шейдеры
// ============================================================================

static const char* k_vertex_shader = R"(
#version 330 core
layout(location = 0) in vec4 a_pos_uv;
layout(location = 1) in vec4 a_color;

out vec2 v_uv;
out vec4 v_color;

uniform mat4 u_projection;
uniform mat4 u_view;

void main() {
    gl_Position = u_projection * u_view * vec4(a_pos_uv.xy, 0.0, 1.0);
    v_uv = a_pos_uv.zw;
    v_color = a_color;
}
)";

static const char* k_fragment_shader = R"(
#version 330 core
in vec2 v_uv;
in vec4 v_color;

out vec4 o_color;

uniform sampler2D u_texture0;

void main() {
    o_color = texture(u_texture0, v_uv) * v_color;
}
)";

// ============================================================================
// Компиляция шейдеров
// ============================================================================

static u32 compile_shader(u32 type, const char* source) {
    u32 shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    i32 success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(shader, 512, nullptr, info);
        const char* type_str = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        TE2D_ERROR("SHADER", "%s compilation failed:\n%s", type_str, info);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

static u32 create_shader_program() {
    TE2D_INFO("SHADER", "Compiling...");

    u32 vs = compile_shader(GL_VERTEX_SHADER, k_vertex_shader);
    u32 fs = compile_shader(GL_FRAGMENT_SHADER, k_fragment_shader);

    if (!vs || !fs) {
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
        TE2D_ERROR("SHADER", "Compilation failed. vs=%u fs=%u", vs, fs);
        return 0;
    }

    u32 program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    i32 success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info[512];
        glGetProgramInfoLog(program, 512, nullptr, info);
        TE2D_ERROR("SHADER", "Link failed:\n%s", info);
        glDeleteProgram(program);
        program = 0;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    TE2D_INFO("SHADER", "Program created: %u", program);
    return program;
}

// ============================================================================
// Инициализация / завершение
// ============================================================================

void te2d_render_backend_opengl_init(f32 viewport_width, f32 viewport_height) {
    TE2D_INFO("BACKEND", "Init OpenGL backend...");

    g_shader_program = create_shader_program();

    g_u_projection = glGetUniformLocation(g_shader_program, "u_projection");
    g_u_view       = glGetUniformLocation(g_shader_program, "u_view");
    g_u_texture0   = glGetUniformLocation(g_shader_program, "u_texture0");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, (i32)viewport_width, (i32)viewport_height);

    TE2D_INFO("BACKEND", "Init OK | shader=%u viewport=%dx%d", g_shader_program, (i32)viewport_width, (i32)viewport_height);
}

void te2d_render_backend_opengl_shutdown() {
    if (g_shader_program) {
        glDeleteProgram(g_shader_program);
        g_shader_program = 0;
    }
    TE2D_INFO("BACKEND", "Shutdown OK");
}

// ============================================================================
// Кадр
// ============================================================================

void te2d_render_backend_opengl_begin_frame(const te2d_camera& camera) {
    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(g_shader_program);

    f32 projection[16];
    f32 view[16];
    camera.get_projection(projection);
    camera.get_view(view);

    glUniformMatrix4fv(g_u_projection, 1, GL_FALSE, projection);
    glUniformMatrix4fv(g_u_view, 1, GL_FALSE, view);
    glUniform1i(g_u_texture0, 0);
}

void te2d_render_backend_opengl_end_frame() {
    glUseProgram(0);
}

// ============================================================================
// Сброс батча
// ============================================================================

void te2d_render_backend_opengl_flush(te2d_sprite_batch& batch) {
    if (batch.sprite_count == 0) return;

    TE2D_DEBUG("BATCH", "Flush: %u sprites, texture=%u, vertices=%u",
               batch.sprite_count, batch.current_texture_id, batch.get_vertex_count());

    batch.flush();
}