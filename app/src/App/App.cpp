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
}

void App::register_key_callback(int key, std::function<void()> func) {
    key_callbacks[key] = func;
}

void App::poll_input() {
    for (auto& [key, callback] : key_callbacks) {
        if (IsKeyPressed(key)) callback();
    }
}

void App::update() {
    const int current_screen = GetCurrentMonitor();
    const int window_width = GetScreenWidth();
    const int window_height = GetScreenHeight();
    const int screen_width = GetMonitorWidth(current_screen);
    const int screen_height = GetMonitorHeight(current_screen);
    const float window_to_screen_ratio = (float)(window_width * window_height) / (float)(screen_width * screen_height);

    if (!IsWindowFullscreen()) {
        previous_width = window_width;
        previous_height = window_height;
    }

    ClearBackground(BLACK);
    GuiSetStyle(DEFAULT, TEXT_SIZE, MIN_HEADER_TEXT_SIZE + MAX_HEADER_TEXT_SIZE * window_to_screen_ratio);

    poll_input();

    for (auto& component : components) {
        component->draw(window_width, window_height, window_to_screen_ratio);
    }
}