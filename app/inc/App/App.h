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

private:
    std::unordered_map<int, std::function<void()>> key_callbacks;
    std::vector<std::unique_ptr<Component>> components;
    int previous_width, previous_height;

    void register_key_callback(int key, std::function<void()> func);
    void poll_input();
};

#define APP_INSTANCE App::instance()

#endif /* _APP_H_ */