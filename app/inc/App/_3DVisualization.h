#ifndef _3D_VISUALIZATION_H_
#define _3D_VISUALIZATION_H_

#include <raylib.h>

#include <Component.h>

class _3DVisualization : public Component {
private:
    Camera3D camera;

public:
    _3DVisualization(float x, float y, float width, float height);
    ~_3DVisualization() override = default;

    void draw(int window_width, int window_height, float window_to_screen_ratio) override;
};

#endif