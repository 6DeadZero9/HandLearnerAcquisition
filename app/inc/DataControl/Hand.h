#ifndef _HAND_H_
#define _HAND_H_

#include <array>
#include <cstdint>
#include <utility>

#include <raylib.h>

#include <HandPart.h>
#include <Singleton.h>
#include <Statics.h>

typedef struct {
    Vector3 accel_data;
    Vector3 gyro_data;
    float temp_data;
} GeneralHandLearnerData;

class Hand : public Singleton<Hand> {
    friend class Singleton<Hand>;

public:
    std::mutex hand_usage_mutex;
    ~Hand() = default;

    void state_update(DataContainer& container);
    std::string print();

    auto begin() const {
        return hand_parts.begin();
    }
    auto end() const {
        return hand_parts.end();
    }
    GeneralHandLearnerData get_general_data() const {
        return { accel_data, gyro_data, temp_data };
    }

private:
    Vector3 accel_data;
    Vector3 gyro_data;
    float temp_data;

    std::array<HandPart, 17> hand_parts = {
        { { "W_L", 0, { { 0.42f, 0.375f }, { 0.15f, 0.45f }, LEFT } },
         { "W_LNG", 1, { { 0.5485f, 0.375f }, { 0.85f, 0.45f }, RIGHT }, true },

         { "T_PROX", 2, { { 0.3f, 0.3f }, { 0.15f, 0.3f }, LEFT }, true },
         { "T_MID", 3, { { 0.22f, 0.28f }, { 0.15f, 0.275f }, LEFT }, true },
         { "T_DIST", 4, { { 0.17f, 0.223f }, { 0.15f, 0.25f }, LEFT }, true },

         { "I_PROX", 5, { { 0.3215f, 0.165f }, { 0.15f, 0.175f }, LEFT }, true },
         { "I_MID", 6, { { 0.3215f, 0.1165f }, { 0.15f, 0.125f }, LEFT }, true },
         { "I_DIST", 7, { { 0.31f, 0.075f }, { 0.15f, 0.075f }, LEFT }, true },

         { "M_PROX", 8, { { 0.45f, 0.1485f }, { 0.15f, 0.15f }, LEFT }, true },
         { "M_MID", 9, { { 0.445f, 0.095f }, { 0.15f, 0.1f }, LEFT }, true },
         { "M_DIST", 10, { { 0.45f, 0.055f }, { 0.15f, 0.05f }, LEFT }, true },

         { "R_PROX", 11, { { 0.5575f, 0.155f }, { 0.85f, 0.15f }, RIGHT }, true },
         { "R_MID", 12, { { 0.57f, 0.11f }, { 0.85f, 0.1f }, RIGHT }, true },
         { "R_DIST", 13, { { 0.58f, 0.0765f }, { 0.85f, 0.05f }, RIGHT }, true },

         { "P_PROX", 14, { { 0.65f, 0.17f }, { 0.85f, 0.175f }, RIGHT }, true },
         { "P_MID", 15, { { 0.662f, 0.1265f }, { 0.85f, 0.125f }, RIGHT }, true },
         { "P_DIST", 16, { { 0.685f, 0.09f }, { 0.85f, 0.075f }, RIGHT }, true } }
    };
};

#define HAND_INSTANCE Hand::instance()

#endif /* _HAND_H_ */
