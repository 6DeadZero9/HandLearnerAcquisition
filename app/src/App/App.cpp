#include <iostream>
#include <raygui.h>

#include <App.h>
#include <ArmStatistics.h>
#include <Statics.h>

App::App() {
    components.emplace_back(std::make_unique<ArmStatistics>(0.01f, 0.02f, 0.3f, 0.97f));

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
    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();

    if (!IsWindowFullscreen()) {
        previous_width = screen_width;
        previous_height = screen_height;
    }

    ClearBackground(BLACK);
    GuiSetStyle(DEFAULT, TEXT_SIZE, HEADER_TEXT_SIZE + GLOBAL_TEXT_SCALE * screen_width);

    poll_input();

    for (auto& component : components) {
        component->draw(screen_width, screen_height);
    }
}