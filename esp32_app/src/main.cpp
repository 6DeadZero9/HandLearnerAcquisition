#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <Wire.h>

#include <Statics.h>

Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;

BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;
TaskHandle_t bluetoothTaskHandle = nullptr;
bool bleConnected = false;

DataContainer primary, secondary;
DataContainer* volatile writeBuffer = &primary;
DataContainer* volatile readBuffer  = &secondary;
constexpr TickType_t SAMPLE_PERIOD_MS = 10;

static uint32_t muxStates[NUM_OF_SENS / 2];

class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) override {
        bleConnected = true;
    }
    void onDisconnect(BLEServer* pServer) override {
        bleConnected = false;
        pServer->getAdvertising()->start();
    }
};

static inline void setMuxState(uint32_t pattern) {
    for (size_t i = 0; i < inputs.size(); ++i) {
        gpio_set_level((gpio_num_t)inputs[i], (pattern >> i) & 1);
    }
}

void bluetoothSendTask(void* pvParameters) {
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (bleConnected && pCharacteristic) {
            pCharacteristic->setValue(reinterpret_cast<uint8_t*>(readBuffer), sizeof(DataContainer));
            pCharacteristic->notify();
        }
    }
}

void sensorCollectionTask(void* pvParameters) {
    const TickType_t period = pdMS_TO_TICKS(SAMPLE_PERIOD_MS);
    TickType_t lastWake = xTaskGetTickCount();

    while (true) {
        DataContainer* buf = writeBuffer;

        mpu.getEvent(&a, &g, &temp);

        buf->accel_x = a.acceleration.x;
        buf->accel_y = a.acceleration.y;
        buf->accel_z = a.acceleration.z;

        buf->gyro_x = g.gyro.x;
        buf->gyro_y = g.gyro.y;
        buf->gyro_z = g.gyro.z;

        buf->imu_temp = temp.temperature;

        const uint16_t half = NUM_OF_SENS / 2;
        for (uint16_t sensor = 0; sensor < half; sensor++) {
            setMuxState(muxStates[sensor]);
            ets_delay_us(10);
            buf->sensor_data[sensor] = analogRead(MUX_OUT_0);
            buf->sensor_data[sensor + half] = analogRead(MUX_OUT_1);
        }

        {
            DataContainer* tmp = readBuffer;
            readBuffer = buf;
            writeBuffer = tmp;
        }
        if (bluetoothTaskHandle) xTaskNotifyGive(bluetoothTaskHandle);
        
        vTaskDelayUntil(&lastWake, period);
    }
}

void setupMuxStates() {
    const uint16_t half = NUM_OF_SENS / 2;
    for (uint16_t ch = 0; ch < half; ++ch) {
        uint32_t pat = 0;
        for (size_t b = 0; b < inputs.size(); ++b) {
            if ((ch >> b) & 1) pat |= (1UL << b);
        }
        muxStates[ch] = pat;
    }
}

void setupBLE() {
    BLEDevice::init("Hand_Learner");
    BLEDevice::setMTU(sizeof(DataContainer) + 3);

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    BLEService* pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ);
    pCharacteristic->addDescriptor(new BLE2902());

    pService->start();
    pServer->getAdvertising()->start();
}

void setupMPU() {
    Wire.begin();
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
            digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
            delay(500);
        }
    }
    Serial.println("MPU6050 connection successful");
}

void setup() {
    Serial.begin(115200);

    pinMode(BUILTIN_LED, OUTPUT);
    pinMode(MUX_OUT_0, INPUT);
    pinMode(MUX_OUT_1, INPUT);

    for (uint16_t pin : inputs) {
        pinMode(pin, OUTPUT);
        gpio_set_level((gpio_num_t)pin, 0);
    }


    setupMuxStates();
    setupMPU();
    setupBLE();

    xTaskCreatePinnedToCore(bluetoothSendTask, "BluetoothStreamer", 4096, NULL, 1, &bluetoothTaskHandle, 1);
    xTaskCreatePinnedToCore(sensorCollectionTask, "SensorCollector", 4096, NULL, 1, NULL, 0);

}

void loop() {
    delay(1000);
}
