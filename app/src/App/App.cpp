#include <iostream>

#include <raygui.h>

#include <App.h>
#include <ArmStatistics.h>
#include <Logger.h>
#include <Statics.h>

App::App() {
    components.emplace_back(std::make_unique<ArmStatistics>(
        ARM_STATISTICS_X_OFFSET, ARM_STATISTICS_Y_OFFSET, ARM_STATISTICS_WIDTH_SCALE, ARM_STATISTICS_HEIGHT_SCALE));

    register_key_callback(KEY_F, [this]() {
        if (!IsWindowFullscreen()) {
            const int current_screen = GetCurrentMonitor();
            SetWindowSize(GetMonitorWidth(current_screen), GetMonitorHeight(current_screen));
        } else {
            SetWindowSize(previous_width, previous_height);
        }
        ToggleFullscreen();
    });

    register_mouse_button_callback(MOUSE_LEFT_BUTTON, [this]() {
        show_zoom = true;
    });
}

void App::poll_input() {
    for (auto& [key, callback] : key_callbacks) {
        if (IsKeyPressed(key)) callback();
    }

    for (auto& [button, callback] : mouse_button_callbacks) {
        if (IsMouseButtonDown(button)) callback();
    }
}

void App::update() {
    bool is_fullscreen = IsWindowFullscreen();
    const int current_screen = GetCurrentMonitor();
    const int window_width = GetScreenWidth();
    const int window_height = GetScreenHeight();
    const int screen_width = GetMonitorWidth(current_screen);
    const int screen_height = GetMonitorHeight(current_screen);
    const float window_to_screen_ratio = (float)(window_width * window_height) / (float)(screen_width * screen_height);

    if (!is_fullscreen) {
        previous_width = window_width;
        previous_height = window_height;
    }

    if (scene_texture.texture.id != 0) UnloadRenderTexture(scene_texture);
    scene_texture = LoadRenderTexture(is_fullscreen ? screen_width : window_width, is_fullscreen ? screen_height : window_height);

    BeginTextureMode(scene_texture);
    {
        ClearBackground(BLACK);
        GuiSetStyle(DEFAULT, TEXT_SIZE, MIN_HEADER_TEXT_SIZE + MAX_HEADER_TEXT_SIZE * window_to_screen_ratio);

        for (auto& component : components) {
            component->draw(window_width, window_height, window_to_screen_ratio);
        }
    }
    EndTextureMode();

    BeginDrawing();
    {
        DrawTextureRec(scene_texture.texture,
                       { 0, (float)scene_texture.texture.height, (float)scene_texture.texture.width, -(float)scene_texture.texture.height },
                       { 0, 0 },
                       WHITE);

        if (show_zoom) {
            Vector2 mouse_position = GetMousePosition();

            if (CheckCollisionPointRec(mouse_position, (Rectangle) { 0, 0, (float)window_width, (float)window_height })) {
                Rectangle zoom_src = { mouse_position.x - BEFORE_ZOOM_SIZE / 2,
                                       (float)scene_texture.texture.height - (mouse_position.y + BEFORE_ZOOM_SIZE / 2),
                                       BEFORE_ZOOM_SIZE,
                                       -BEFORE_ZOOM_SIZE };
                Rectangle zoom_dst = { mouse_position.x - ZOOM_SIZE / 2, mouse_position.y - ZOOM_SIZE / 2, ZOOM_SIZE, ZOOM_SIZE };
                Rectangle zoom_rect = { mouse_position.x - (ZOOM_SIZE * ZOOM_SIZE_EXPANSION) / 2,
                                        mouse_position.y - (ZOOM_SIZE * ZOOM_SIZE_EXPANSION) / 2,
                                        (ZOOM_SIZE * ZOOM_SIZE_EXPANSION),
                                        (ZOOM_SIZE * ZOOM_SIZE_EXPANSION) };

                DrawRectangleRec(zoom_rect, WHITE);
                DrawTexturePro(scene_texture.texture, zoom_src, zoom_dst, { 0, 0 }, 0, WHITE);
                show_zoom = false;
            }
        }
    }
    EndDrawing();
}