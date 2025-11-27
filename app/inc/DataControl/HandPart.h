#ifndef _HAND_PART_H_
#define _HAND_PART_H_

#include <cstdint>
#include <string>

#include <raylib.h>

#include <Statics.h>

enum GUIAlignment {
    LEFT,
    RIGHT
};

typedef struct {
    Vector2 box_pointer;
    Vector2 text_pointer;
    GUIAlignment alignment;
} GUIData;

class HandPart {
public:
    HandPart(std::string given_name, uint8_t given_index, GUIData given_gui_data, bool is_inverted = false);
    HandPart() = delete;
    ~HandPart() = default;

    void update(const DataContainer& container);
    std::string print();

    const GUIData get_gui_data() const {
        return gui_data;
    }

    const std::string get_name() const {
        return name;
    }

    const float get_angle(bool processed = false) const {
        return processed ? processed_angle : raw_angle;
    }

private:
    const GUIData gui_data;
    const std::string name;
    const uint8_t index;
    const bool inverted;
    float raw_angle, processed_angle = 0.0f;
};

#endif /* _HAND_PART_H_ */
