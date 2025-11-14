#ifndef ARM_STATISTICS_H_
#define ARM_STATISTICS_H_

#include <raylib.h>

#include <Component.h>

class ArmStatistics : public Component {
private:
    Texture2D arm_image;

public:
    ArmStatistics(float x, float y, float width, float height);
    ~ArmStatistics() override = default;

    void draw(int screen_width, int screen_height) override;
};

#endif