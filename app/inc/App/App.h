#ifndef _APP_H_
#define _APP_H_

#include <functional>
#include <unordered_map>

#include <Component.h>
#include <Singleton.h>

class App : public Singleton<App> {
    friend class Singleton<App>;

public:
    App();
    ~App() = default;
    void update();
    void poll_input();

private:
    std::unordered_map<int, std::function<void()>> key_callbacks;
    std::unordered_map<int, std::function<void()>> mouse_button_callbacks;
    std::vector<std::unique_ptr<Component>> components;

    RenderTexture2D scene_texture;

    int previous_width, previous_height;
    bool show_zoom = false;

    void register_key_callback(int key, std::function<void()> func) {
        key_callbacks[key] = func;
    }

    void register_mouse_button_callback(int button, std::function<void()> func) {
        mouse_button_callbacks[button] = func;
    }
};

#define APP_INSTANCE App::instance()

#endif /* _APP_H_ */