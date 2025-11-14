#ifndef BLE_MANAGER_H_
#define BLE_MANAGER_H_

#include <mutex>
#include <thread>

#include <simpleble/Adapter.h>

#include <Singleton.h>
#include <Tools.h>

class BLEManager : public Singleton<BLEManager> {
    friend class Singleton<BLEManager>;

public:
    ~BLEManager();

    void initiate(const SimpleBLE::Adapter& given_adapter);
    void find_hand_learner();
    void setup_device_callbacks();

private:
    bool has_to_finish = false;
    std::mutex worker_mutex;
    std::thread finder_thread;

    SimpleBLE::Adapter adapter;
    SimpleBLE::Peripheral hand_learner;
};

#define BLE_MANAGER_INSTANCE BLEManager::instance()

#endif