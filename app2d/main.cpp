#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <core/te2d_log.hpp>
#include <render/te2d_render_frontend.hpp>
#include <render/te2d_texture.hpp>
#include <render/te2d_sprite.hpp>
#include <render/te2d_camera.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// ===============================
// Глобальные переменные
// ===============================

static GLFWwindow* g_window        = nullptr;
static i32         g_window_width  = 1280;
static i32         g_window_height = 720;

static te2d_texture g_player_tex;
static te2d_sprite  g_player_sprite;
static te2d_camera  g_camera;
static f32          g_camera_speed = 300.0f;

// ===============================
// Инициализация / завершение игры
// ===============================

static void game_init() {
    TE2D_INFO("APP", "Game init...");

    g_player_tex = te2d_texture::load("assets2d/textures/player.png");
    g_player_sprite = te2d_sprite::create(&g_player_tex);
    g_player_sprite.position = {640.0f, 360.0f};

    g_camera = te2d_camera::create((f32)g_window_width, (f32)g_window_height);
    g_camera.position = {640.0f, 360.0f};

    TE2D_INFO("APP", "Game init OK");
}

static void game_shutdown() {
    g_player_tex.unload();
    TE2D_INFO("APP", "Game shutdown OK");
}

// ===============================
// Игровая логика (update)
// ===============================

static void game_update(f32 dt) {
    if (glfwGetKey(g_window, GLFW_KEY_LEFT)  == GLFW_PRESS) g_camera.position.x -= g_camera_speed * dt;
    if (glfwGetKey(g_window, GLFW_KEY_RIGHT) == GLFW_PRESS) g_camera.position.x += g_camera_speed * dt;
    if (glfwGetKey(g_window, GLFW_KEY_UP)    == GLFW_PRESS) g_camera.position.y -= g_camera_speed * dt;
    if (glfwGetKey(g_window, GLFW_KEY_DOWN)  == GLFW_PRESS) g_camera.position.y += g_camera_speed * dt;

    if (glfwGetKey(g_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        g_camera.position = g_player_sprite.position;
    }
}

// ===============================
// Отрисовка кадра (draw)
// ===============================

static void game_draw() {
    te2d_render_begin_frame(g_camera);
    te2d_render_draw_sprite(g_player_sprite);

    // -------------------------------
    // ImGui
    // -------------------------------
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("TestEngine2D");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Camera: (%.0f, %.0f)", g_camera.position.x, g_camera.position.y);
    ImGui::Text("Texture: %ux%u id=%u", g_player_tex.width, g_player_tex.height, g_player_tex.id);
    ImGui::Text("Sprite: pos=(%.0f,%.0f) scale=(%.2f,%.2f)",
                g_player_sprite.position.x, g_player_sprite.position.y,
                g_player_sprite.scale.x, g_player_sprite.scale.y);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    te2d_render_end_frame();
}

// ===============================
// Точка входа
// ===============================

int main() {
    // Уровень логирования
    g_te2d_log_level = te2d_log_level::debug;

    TE2D_INFO("APP", "=== TestEngine2D Starting ===");

    // -------------------------------
    // GLFW + OpenGL
    // -------------------------------
    if (!glfwInit()) {
        TE2D_ERROR("APP", "glfwInit failed");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    g_window = glfwCreateWindow(g_window_width, g_window_height, "TestEngine2D", nullptr, nullptr);
    if (!g_window) {
        TE2D_ERROR("APP", "glfwCreateWindow failed");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(g_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        TE2D_ERROR("APP", "gladLoadGL failed");
        glfwDestroyWindow(g_window);
        glfwTerminate();
        return -1;
    }

    TE2D_INFO("APP", "OpenGL %s | GLFW %s", glGetString(GL_VERSION), glfwGetVersionString());

    // -------------------------------
    // 2D-рендер
    // -------------------------------
    te2d_render_init((f32)g_window_width, (f32)g_window_height);

    // -------------------------------
    // ImGui
    // -------------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(g_window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // -------------------------------
    // Игра
    // -------------------------------
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

    // ===============================
    // Завершение
    // ===============================
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