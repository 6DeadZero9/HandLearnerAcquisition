#ifndef ARM_STATISTICS_H_
#define ARM_STATISTICS_H_

#include <raylib.h>

#include <Component.h>

class ArmStatistics : public Component {
private:
    Texture2D arm_image;
    Color outlines_color;
    Font font;

    float image_ratio;

public:
    ArmStatistics(float x, float y, float width, float height);
    ~ArmStatistics() override = default;

    void draw(int window_width, int window_height, float window_to_screen_ratio) override;
};

#endif