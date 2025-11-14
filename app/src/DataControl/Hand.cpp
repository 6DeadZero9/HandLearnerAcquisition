#include <Hand.h>
#include <Logger.h>

void Hand::state_update(DataContainer& container) {
    std::lock_guard<std::mutex> lock(hand_usage_mutex);

    accel_data = (Vector3) { container.accel_x, container.accel_y, container.accel_z };
    gyro_data = (Vector3) { container.gyro_x, container.gyro_y, container.gyro_z };
    temp_data = container.imu_temp;

    for (auto& hand_part : hand_parts) {
        hand_part.update(container);
    }
}

std::string Hand::print() {
    std::lock_guard<std::mutex> lock(hand_usage_mutex);
    std::stringstream ss;

    ss << "\n\tHAND:";
    for (auto& hand_part : hand_parts) {
        ss << hand_part.print();
    }
    ss << std::endl;

    return ss.str();
}