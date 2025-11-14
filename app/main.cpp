#define RAYGUI_IMPLEMENTATION
#include <chrono>
#include <csignal>
#include <execinfo.h>
#include <iostream>
#include <sstream>
#include <thread>

#include <simpleble/Adapter.h>
#include <simpleble/SimpleBLE.h>

#include <raygui.h>
#include <raylib.h>

#include <App.h>
#include <BLEManager.h>
#include <Logger.h>
#include <Statics.h>
#include <Tools.h>

void signalHandler(int32_t signal) {
    LOG(logger::LOG_CRIT, "Got an interrupt signal: %d", signal);

    exit(signal);
}

void setupSignalHandlers() {
    struct sigaction sa;

    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    // intercept fault signals
    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGPWR, &sa, nullptr);
    sigaction(SIGTERM, &sa, nullptr);
    sigaction(SIGSEGV, &sa, nullptr);
    sigaction(SIGBUS, &sa, nullptr);
    sigaction(SIGFPE, &sa, nullptr);
    sigaction(SIGILL, &sa, nullptr);
    sigaction(SIGABRT, &sa, nullptr);
    sigaction(SIGSTKFLT, &sa, nullptr);

    // ignore SIGPIPE signal
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = SA_NOMASK;
    sigaction(SIGPIPE, &sa, nullptr);

    LOG(logger::LOG_INFO, "Signal handler was initialised");
}

int main() {
    /* Logger setup section */

    LOGGER_INSTANCE.init(logger::LOG_DEBUG);
    LOG(logger::LOG_INFO, "Logger was initialised");
    LOG(logger::LOG_INFO, "Sizeof BLE data struct: %d", sizeof(DataContainer));
    setupSignalHandlers();

    /* Raylib setup section */

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(0, 0, "Hand Learner Visualization");
    SetTargetFPS(144);

    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();

    SetWindowSize(screen_width / 2, screen_height / 2);

    auto adapter_optional = tools::getAdapter();

    if (!adapter_optional.has_value()) {
        LOG(logger::LOG_ERR, "Adapter retrieval error. Exiting...");
        return EXIT_FAILURE;
    }

    BLE_MANAGER_INSTANCE.initiate(adapter_optional.value());
    BLE_MANAGER_INSTANCE.find_hand_learner();

    while (!WindowShouldClose()) {
        BeginDrawing();

        APP_INSTANCE.update();

        EndDrawing();
    }

    CloseWindow();

    return EXIT_SUCCESS;
}