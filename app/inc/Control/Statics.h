#ifndef STATICS_H_
#define STATICS_H_

#include <array>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

/* Shared config for App/ESP32 */

constexpr uint16_t SCAN_FOR { 1000 };
constexpr uint16_t MAX_ANGLE_VALUE { 4096 };
const std::string HAND_LEARNER_ADDRESS { "AC:15:18:A2:13:4A" };
const std::string HAND_LEARNER_IDENTIFIER { "Hand_Learner" };
const std::string SERVICE_UUID { "6e400001-b5a3-f393-e0a9-e50e24dcca9e" };
const std::string CHARACTERISTIC_UUID { "bdff2ec1-e826-4882-9d33-72a256993eff" };

/* ESP32 defines */

constexpr uint16_t MUX_IN_0 { 4 };
constexpr uint16_t MUX_IN_1 { 16 };
constexpr uint16_t MUX_IN_2 { 17 };
constexpr uint16_t MUX_IN_3 { 5 };
constexpr uint16_t MUX_OUT_0 { 27 };
constexpr uint16_t MUX_OUT_1 { 26 };
constexpr uint16_t NUM_OF_SENS { 32 };
constexpr uint16_t BUILTIN_LED { 2 };

const std::array<uint16_t, 4> inputs = { MUX_IN_0, MUX_IN_1, MUX_IN_2, MUX_IN_3 };

/* Raylib config */

constexpr float GLOBAL_TEXT_SCALE { 0.005 };
constexpr int HEADER_TEXT_SIZE { 15 };
constexpr int RECTANGLE_POINTER_SIZE { 10 };

/* Shared data container */

typedef struct DataContainer {
    float accel_x, accel_y, accel_z;
    float gyro_x, gyro_y, gyro_z;
    float imu_temp;
    uint16_t sensor_data[NUM_OF_SENS];

    std::string print() {
        std::stringstream ss;
        ss << "Hand_learner status:"
           << "\n\tAccelerometer x: " << accel_x << " y: " << accel_y << " z: " << accel_z << "\n\tGyro x: " << gyro_x << " y: " << gyro_y
           << " z: " << gyro_z << "\n\tIMU temperature: " << imu_temp << "\n\tJoints status: ";
        for (size_t step = 0; step < NUM_OF_SENS; step++) {
            ss << "\n\t\tJoint " << step << ": " << sensor_data[step];
        }
        return ss.str();
    }

    static DataContainer fromBytes(const std::vector<uint8_t>& bytes) {
        DataContainer data {};
        if (bytes.size() < sizeof(DataContainer) || bytes.size() > sizeof(DataContainer)) return data;
        std::memcpy(&data, bytes.data(), sizeof(DataContainer));
        return data;
    }

} DataContainer;

#endif /* STATICS_H_ */