#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <core/te2d_log.hpp>
#include <core/te2d_allocator.hpp>
#include <render/te2d_render_frontend.hpp>
#include <render/te2d_texture.hpp>
#include <render/te2d_sprite.hpp>
#include <render/te2d_camera.hpp>
#include <physics/te2d_physics_world.hpp>
#include <physics/te2d_collision.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// ===============================
// Глобальные переменные
// ===============================

static GLFWwindow* g_window        = nullptr;
static i32         g_window_width  = 1280;
static i32         g_window_height = 720;

// Текстуры и спрайты
static te2d_texture g_player_tex;
static te2d_sprite  g_player_sprite;
static te2d_texture g_platform_tex;
static te2d_sprite  g_platform_sprite;

// Камера
static te2d_camera g_camera;
static f32         g_camera_speed = 300.0f;

// Физика
static te2d_allocator     g_physics_allocator;
static te2d_physics_world g_world;
static i32                g_player_body_index   = -1;
static i32                g_platform_body_index = -1;
static f32                g_move_speed          = 200.0f;

// ===============================
// Инициализация / завершение игры
// ===============================

static void game_init() {
    TE2D_INFO("APP", "Game init...");

    // Аллокатор для физического мира
    g_physics_allocator = te2d_allocator::create(1024 * 1024); // 1 MB

    // Текстура и спрайт персонажа
    g_player_tex = te2d_texture::load("assets2d/textures/player.png");
    g_player_sprite = te2d_sprite::create(&g_player_tex);
    g_player_sprite.position = {640.0f, 100.0f};

    // Текстура и спрайт платформы
    g_platform_tex = te2d_texture::load("assets2d/textures/platform.png");
    g_platform_sprite = te2d_sprite::create(&g_platform_tex);
    g_platform_sprite.position = {640.0f, 500.0f};

    // Камера
    g_camera = te2d_camera::create((f32)g_window_width, (f32)g_window_height);
    g_camera.position = {640.0f, 360.0f};

    // Физический мир
    g_world = te2d_physics_world::create(g_physics_allocator);

    // Персонаж (динамическое тело + AABB)
    {
        te2d_rigidbody body = te2d_rigidbody::create_dynamic(1.0f, 0.0f);
        body.position = g_player_sprite.position;
        te2d_collider col = te2d_collider::create_aabb(
            (f32)g_player_tex.width, (f32)g_player_tex.height);
        g_player_body_index = g_world.add_body(body, col);
    }

    // Платформа (статическое тело + AABB)
    {
        te2d_rigidbody body = te2d_rigidbody::create_static();
        body.position = g_platform_sprite.position;
        te2d_collider col = te2d_collider::create_aabb(
            (f32)g_platform_tex.width, (f32)g_platform_tex.height);
        g_platform_body_index = g_world.add_body(body, col);
    }

    TE2D_INFO("APP", "Game init OK | player=%d platform=%d",
              g_player_body_index, g_platform_body_index);
}

static void game_shutdown() {
    g_player_tex.unload();
    g_platform_tex.unload();
    g_physics_allocator.destroy();
    TE2D_INFO("APP", "Game shutdown OK");
}

// ===============================
// Игровая логика (update)
// ===============================

static void game_update(f32 dt) {
    // Управление персонажем
    te2d_rigidbody* player_body = g_world.get_body(g_player_body_index);
    if (player_body) {
        if (glfwGetKey(g_window, GLFW_KEY_LEFT)  == GLFW_PRESS)
            player_body->apply_force({-g_move_speed, 0.0f});
        if (glfwGetKey(g_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            player_body->apply_force({g_move_speed, 0.0f});
    }

    // Шаг физики
    g_world.step();

    // Синхронизация спрайтов с телами
    if (player_body) {
        g_player_sprite.position = player_body->position;
    }

    // Камера следует за персонажем
    g_camera.position = g_player_sprite.position;

    // Ручное управление камерой
    if (glfwGetKey(g_window, GLFW_KEY_UP)   == GLFW_PRESS) g_camera.position.y -= g_camera_speed * dt;
    if (glfwGetKey(g_window, GLFW_KEY_DOWN) == GLFW_PRESS) g_camera.position.y += g_camera_speed * dt;
}

// ===============================
// Отрисовка кадра (draw)
// ===============================

static void game_draw() {
    te2d_render_begin_frame(g_camera);

    te2d_render_draw_sprite(g_platform_sprite);
    te2d_render_draw_sprite(g_player_sprite);

    // -------------------------------
    // ImGui
    // -------------------------------
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("TestEngine2D - Physics");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    const te2d_rigidbody* player_body = g_world.get_body(g_player_body_index);
    if (player_body) {
        ImGui::Text("Player: pos=(%.0f,%.0f) vel=(%.0f,%.0f)",
                    player_body->position.x, player_body->position.y,
                    player_body->velocity.x, player_body->velocity.y);
    }

    const te2d_rigidbody* platform_body = g_world.get_body(g_platform_body_index);
    if (platform_body) {
        ImGui::Text("Platform: pos=(%.0f,%.0f)",
                    platform_body->position.x, platform_body->position.y);
    }

    ImGui::Text("Bodies: %u", g_world.get_body_count());
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    te2d_render_end_frame();
}

// ===============================
// Точка входа
// ===============================

int main() {
    g_te2d_log_level = te2d_log_level::debug;
    TE2D_INFO("APP", "=== TestEngine2D Starting ===");

    if (!glfwInit()) { TE2D_ERROR("APP", "glfwInit failed"); return -1; }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    g_window = glfwCreateWindow(g_window_width, g_window_height, "TestEngine2D - Physics", nullptr, nullptr);
    if (!g_window) { TE2D_ERROR("APP", "glfwCreateWindow failed"); glfwTerminate(); return -1; }

    glfwMakeContextCurrent(g_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        TE2D_ERROR("APP", "gladLoadGL failed");
        glfwDestroyWindow(g_window);
        glfwTerminate();
        return -1;
    }

    TE2D_INFO("APP", "OpenGL %s", glGetString(GL_VERSION));

    te2d_render_init((f32)g_window_width, (f32)g_window_height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(g_window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    game_init();

    f64 last_time = glfwGetTime();

    // ===============================
    // Главный цикл
    // ===============================
    while (!glfwWindowShouldClose(g_window)) {
        f64 now = glfwGetTime();
        f32 dt  = (f32)(now - last_time);
        last_time = now;

        glfwPollEvents();
        game_update(dt);
        game_draw();
        glfwSwapBuffers(g_window);
    }

    TE2D_INFO("APP", "=== TestEngine2D Shutting down ===");

    game_shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    te2d_render_shutdown();
    glfwDestroyWindow(g_window);
    glfwTerminate();

    return 0;
}