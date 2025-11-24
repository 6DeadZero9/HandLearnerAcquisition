#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <raylib.h>

class Component {
protected:
    float x, y, width, height;

public:
    Component(float x, float y, float width, float height)
        : x(x),
          y(y),
          width(width),
          height(height) {}
    virtual ~Component() = default;

    virtual void update() {}
    virtual void draw(int window_width, int window_height, float window_to_screen_ratio) = 0;
};

#endif /* COMPONENT_H_ */