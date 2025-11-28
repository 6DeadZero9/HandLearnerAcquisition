#include <format>
#include <vector>

#include <BLEManager.h>
#include <Hand.h>
#include <Logger.h>
#include <Statics.h>

void BLEManager::shutdown() {
    has_to_shutdown = true;
    std::string last_message = "\n\tLast message from BLEManager:\n";

    try {
        if (adapter.scan_is_active()) {
            adapter.scan_stop();
            last_message += "\t\tBLE Scanning stopped";
        }
    } catch (const std::exception& e) {
        last_message += std::format("\t\tscan_stop() threw: {}", e.what());
    }

    if (finder_thread.joinable()) {
        finder_thread.join();
        last_message += "\n\t\tfinder_thread finished successfully";
    }

    if (hand_learner.initialized() && hand_learner.is_connected()) {
        hand_learner.disconnect();
        last_message += std::format("\n\t\tDisconnected successfully from: {}", hand_learner.identifier());
    }

    LOG(logger::LOG_INFO, last_message);
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
        if (!has_to_shutdown) {
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
        while (!has_to_shutdown) {
            try {
                adapter.scan_for(SCAN_FOR);
            } catch (const std::exception& e) {
                LOG(logger::LOG_ERR, std::format("scan_for() threw: {}", e.what()));
            }

            for (auto& peripheral : adapter.scan_get_results()) {
                if (peripheral.address() == HAND_LEARNER_ADDRESS || peripheral.identifier() == HAND_LEARNER_IDENTIFIER) {
                    std::lock_guard<std::mutex> lock(worker_mutex);

                    hand_learner = peripheral;

                    std::string msg = std::format("\n\tFound Hand Learner:\n"
                                                  "\t\tAddress: {}\n"
                                                  "\t\tIdentifier: {}\n"
                                                  "\t\tIs connectable: {}\n"
                                                  "\t\tSignal strength: {}",
                                                  hand_learner.address(),
                                                  hand_learner.identifier(),
                                                  hand_learner.is_connectable(),
                                                  hand_learner.rssi());

                    LOG(logger::LOG_INFO, msg);

                    if (hand_learner.is_connectable()) {
                        hand_learner.connect();

                        msg.clear();
                        msg = std::format("\n\tConnected successfully to: {}\n\t\tMTU: {}", peripheral.identifier(), hand_learner.mtu());

                        for (auto& service : hand_learner.services()) {
                            msg += std::format("\n\t\tService: {}", service.uuid());

                            for (auto& characteristic : service.characteristics()) {
                                msg += std::format("\n\t\t\tCharacteristic: {}\n\t\t\t\tCapabilities: ", characteristic.uuid());

                                for (auto& capability : characteristic.capabilities())
                                    msg += std::format("{} ", capability);

                                for (auto& descriptor : characteristic.descriptors())
                                    msg += std::format("\n\t\t\t\tDescriptor: {}", descriptor.uuid());

                                if (characteristic.uuid() == CHARACTERISTIC_UUID && service.uuid() == SERVICE_UUID) {
                                    setup_device_callbacks();

                                    msg += "\n\tFound the needed service.";
                                    LOG(logger::LOG_INFO, msg);
                                    return;
                                }
                            }
                        }

                        msg += "\n\tDesired service was not found.";
                        LOG(logger::LOG_INFO, msg);
                    }

                    return;
                }
            }
        }
    });
}
