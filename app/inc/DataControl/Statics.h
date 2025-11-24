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

// Needed by both
constexpr uint16_t NUM_OF_SENS { 32 };

#ifndef APPLICATION
constexpr uint16_t MUX_IN_0 { 4 };
constexpr uint16_t MUX_IN_1 { 16 };
constexpr uint16_t MUX_IN_2 { 17 };
constexpr uint16_t MUX_IN_3 { 5 };
constexpr uint16_t MUX_OUT_0 { 27 };
constexpr uint16_t MUX_OUT_1 { 26 };
constexpr uint16_t BUILTIN_LED { 2 };

const std::array<uint16_t, 4> inputs = { MUX_IN_0, MUX_IN_1, MUX_IN_2, MUX_IN_3 };
#endif

/* Raylib config */

#ifdef APPLICATION

enum KeyState {
    PRESSED,
    DOWN
};

constexpr int MIN_HEADER_TEXT_SIZE { 15 };
constexpr int MAX_HEADER_TEXT_SIZE { 35 };

constexpr int BEFORE_ZOOM_SIZE { 100 };
constexpr int ZOOM_SIZE { 400 };
constexpr float ZOOM_SIZE_EXPANSION { 1.05f };

// Arm Statistics box values

constexpr int MIN_ARM_STATISTICS_TEXT_SIZE { 10 };
constexpr int MAX_ARM_STATISTICS_TEXT_SIZE { 15 };

constexpr int MIN_RECTANGLE_POINTER_SIZE { 10 };
constexpr int MAX_RECTANGLE_POINTER_SIZE { 10 };

constexpr int MIN_LINE_THICKNESS_SIZE { 1 };
constexpr int MAX_LINE_THICKNESS_SIZE { 2 };

constexpr float ARM_STATISTICS_X_OFFSET { 0.01f };
constexpr float ARM_STATISTICS_Y_OFFSET { 0.02f };
constexpr float ARM_STATISTICS_WIDTH_SCALE { 0.3f };
constexpr float ARM_STATISTICS_HEIGHT_SCALE { 0.97f };

constexpr float GENERAL_STATISTICS_X_TEST_POSITION_OFFSET { 0.05f };
constexpr float GENERAL_STATISTICS_Y_TEST_POSITION_OFFSET { 0.05f };
constexpr float GENERAL_STATISTICS_X_PADDING { 0.1f };
constexpr float GENERAL_STATISTICS_Y_PADDING { 0.1f };

constexpr float ARM_IMAGE_BOX_PADDING_SCALE { 0.01f };

constexpr float ANGLE_TEXT_BOX_X_PADDING { 0.1f };
constexpr float ANGLE_TEXT_BOX_Y_PADDING { 0.15f };

#endif

/* Shared data container */

#pragma pack(push, 1)
typedef struct DataContainer {
    float accel_x, accel_y, accel_z;
    float gyro_x, gyro_y, gyro_z;
    float imu_temp;
    uint16_t sensor_data[NUM_OF_SENS];
} DataContainer;
#pragma pack(pop)

#endif /* STATICS_H_ */