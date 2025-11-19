#include <vector>

#include <BLEManager.h>
#include <Hand.h>
#include <Logger.h>
#include <Statics.h>

BLEManager::~BLEManager() {
    has_to_finish = true;
    std::stringstream last_message;

    last_message << "\n\tLast message from BLEManager:\n";
    if (finder_thread.joinable()) {
        finder_thread.join();
        last_message << "\t\tfinder_thread threads finished successfully";
    }

    if (hand_learner.initialized() && hand_learner.is_connected()) {
        hand_learner.disconnect();
        last_message << "\n\t\tDisconnected successfully from: " + hand_learner.identifier();
    }
    LOG(logger::LOG_INFO, last_message.str());
}

void BLEManager::initiate(const SimpleBLE::Adapter& given_adapter) {
    adapter = given_adapter;

    adapter.set_callback_on_scan_start([]() {
        LOG(logger::LOG_INFO, "Started scanning for devices...");
    });

    adapter.set_callback_on_scan_stop([]() {
        LOG(logger::LOG_INFO, "Scan finished...");
    });
}

void BLEManager::setup_device_callbacks() {
    hand_learner.notify(SERVICE_UUID, CHARACTERISTIC_UUID, [&](SimpleBLE::ByteArray bytes) {
        DataContainer hand_learner_data {};
        std::memcpy(&hand_learner_data, bytes.data(), sizeof(DataContainer));
        HAND_INSTANCE.state_update(hand_learner_data);
    });

    hand_learner.set_callback_on_disconnected([this]() {
        LOG(logger::LOG_WARNING, "Device disconnected...");
        if (!has_to_finish) {
            LOG(logger::LOG_WARNING, "Starting finder thread...");

            find_hand_learner();
        }
    });
}

void BLEManager::find_hand_learner() {
    if (finder_thread.joinable()) {
        finder_thread.join();
    }

    finder_thread = std::thread([this]() {
        while (!has_to_finish) {
            adapter.scan_for(SCAN_FOR);

            for (auto& peripheral : adapter.scan_get_results()) {
                if (peripheral.address() == HAND_LEARNER_ADDRESS || peripheral.identifier() == HAND_LEARNER_IDENTIFIER) {
                    std::lock_guard<std::mutex> lock(worker_mutex);
                    std::stringstream ss;
                    hand_learner = peripheral;
                    ss << "\n\tFound Hand Learner:" << "\n\t\tAddress: " << hand_learner.address()
                       << "\n\t\tIdentifier: " << hand_learner.identifier() << "\n\t\tIs connectable: " << hand_learner.is_connectable()
                       << "\n\t\tSignal strength: " << hand_learner.rssi();

                    LOG(logger::LOG_INFO, ss.str());
                    ss.str(std::string());

                    if (hand_learner.is_connectable()) {
                        hand_learner.connect();

                        ss << "\n\tConnected successfully to: " << peripheral.identifier() << "\n\t\tMTU: " << hand_learner.mtu();
                        for (auto& service : hand_learner.services()) {
                            ss << "\n\t\tService: " << service.uuid();

                            for (auto& characteristic : service.characteristics()) {
                                ss << "\n\t\t\tCharacteristic: " << characteristic.uuid() << "\n\t\t\t\tCapabilities: ";

                                for (auto& capability : characteristic.capabilities())
                                    ss << capability << " ";

                                for (auto& descriptor : characteristic.descriptors())
                                    ss << "\n\t\t\t\tDescriptor: " << descriptor.uuid();

                                if (characteristic.uuid() == CHARACTERISTIC_UUID && service.uuid() == SERVICE_UUID) {
                                    setup_device_callbacks();

                                    ss << "\n\tFound the needed service.";
                                    LOG(logger::LOG_INFO, ss.str());
                                    return;
                                }
                            }
                        }
                        ss << "\n\tDesired service was not found.";
                        LOG(logger::LOG_INFO, ss.str());
                    }

                    return;
                }
            }
        }
    });
}
