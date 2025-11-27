#define _USE_MATH_DEFINES

#include <cmath>

#include <HandPart.h>
#include <Tools.h>

HandPart::HandPart(std::string given_name, uint8_t given_index, GUIData given_gui_data, bool is_inverted)
    : name(given_name),
      index(given_index),
      gui_data(given_gui_data),
      inverted(is_inverted) {}

void HandPart::update(const DataContainer& container) {
    uint16_t value = inverted ? MAX_ANGLE_VALUE - container.sensor_data[index] : container.sensor_data[index];

    raw_angle = tools::map_range(value, 0, MAX_ANGLE_VALUE, 0, 2 * M_PI);
    processed_angle += tools::lpf(raw_angle, processed_angle);
}

std::string HandPart::print() {
    std::stringstream ss;

    ss << "\n\t\t" << name << ":"
       << "\n\t\t\tIndex: " << static_cast<int>(index) << "\n\t\t\tAngle: " << raw_angle * (180 / M_PI) << "°";

    return ss.str();
}