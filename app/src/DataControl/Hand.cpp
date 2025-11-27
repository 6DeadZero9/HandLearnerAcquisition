#include <Hand.h>
#include <Logger.h>
#include <Tools.h>

void Hand::state_update(DataContainer& container) {
    std::lock_guard<std::mutex> lock(hand_usage_mutex);

    raw_data = {
        (Vector3) { container.accel_x, container.accel_y, container.accel_z },
        (Vector3) { container.gyro_x,  container.gyro_y,  container.gyro_z  },
        container.imu_temp
    };
    processed_data = {
        (Vector3) { processed_data.accel_data.x + tools::lpf(container.accel_x, processed_data.accel_data.x),
                   processed_data.accel_data.y + tools::lpf(container.accel_y, processed_data.accel_data.y),
                   processed_data.accel_data.z + tools::lpf(container.accel_z, processed_data.accel_data.z) },
        (Vector3) { processed_data.gyro_data.x + tools::lpf(container.gyro_x,   processed_data.gyro_data.x),
                   processed_data.gyro_data.y + tools::lpf(container.gyro_y,   processed_data.gyro_data.y),
                   processed_data.gyro_data.z + tools::lpf(container.gyro_z,   processed_data.gyro_data.z)  },
        processed_data.temp_data + tools::lpf(container.imu_temp, processed_data.temp_data)
    };

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