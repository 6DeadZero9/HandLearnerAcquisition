#ifndef _HAND_H_
#define _HAND_H_

#include <array>
#include <cstdint>
#include <utility>

#include <raylib.h>

#include <HandPart.h>
#include <Singleton.h>
#include <Statics.h>

class Hand : public Singleton<Hand> {
    friend class Singleton<Hand>;

public:
    ~Hand() = default;

    void state_update(DataContainer& container);
    std::string print();

    auto begin() const {
        return hand_parts.begin();
    }
    auto end() const {
        return hand_parts.end();
    }

private:
    std::mutex hand_usage_mutex;

    Vector3 accel_data, gyro_data;
    float temp_data;

    std::array<HandPart, 17> hand_parts = {
        { { "WRIST_LATERAL", 0, { { 0.42f, 0.375f }, { 0.1f, 0.45f }, LEFT } },
         { "WRIST_LONGITUDINAL", 1, { { 0.5485f, 0.375f }, { 0.8f, 0.45f }, RIGHT }, true },

         { "THUMB_PROXIMAL", 2, { { 0.3f, 0.3f }, { 0.1f, 0.3f }, LEFT } },
         { "THUMB_MIDDLE", 3, { { 0.22f, 0.28f }, { 0.1f, 0.275f }, LEFT }, true },
         { "THUMB_DISTAL", 4, { { 0.17f, 0.223f }, { 0.1f, 0.25f }, LEFT }, true },

         { "INDEX_PROXIMAL", 5, { { 0.3215f, 0.165f }, { 0.1f, 0.175f }, LEFT } },
         { "INDEX_MIDDLE", 6, { { 0.3215f, 0.1165f }, { 0.1f, 0.125f }, LEFT }, true },
         { "INDEX_DISTAL", 7, { { 0.31f, 0.075f }, { 0.1f, 0.075f }, LEFT }, true },

         { "MIDDLE_PROXIMAL", 8, { { 0.45f, 0.1485f }, { 0.1f, 0.15f }, LEFT } },
         { "MIDDLE_MIDDLE", 9, { { 0.445f, 0.095f }, { 0.1f, 0.1f }, LEFT }, true },
         { "MIDDLE_DISTAL", 10, { { 0.45f, 0.055f }, { 0.1f, 0.05f }, LEFT }, true },

         { "RING_PROXIMAL", 11, { { 0.5575f, 0.155f }, { 0.8f, 0.15f }, RIGHT } },
         { "RING_MIDDLE", 12, { { 0.57f, 0.11f }, { 0.8f, 0.1f }, RIGHT }, true },
         { "RING_DISTAL", 13, { { 0.58f, 0.0765f }, { 0.8f, 0.05f }, RIGHT }, true },

         { "PINKY_PROXIMAL", 14, { { 0.65f, 0.17f }, { 0.8f, 0.175f }, RIGHT } },
         { "PINKY_MIDDLE", 15, { { 0.662f, 0.1265f }, { 0.8f, 0.125f }, RIGHT }, true },
         { "PINKY_DISTAL", 16, { { 0.685f, 0.09f }, { 0.8f, 0.075f }, RIGHT }, true } }
    };
};

#define HAND_INSTANCE Hand::instance()

#endif /* _HAND_H_ */
