
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

BLEServer* pServer;
BLECharacteristic* pCharacteristic;
bool bleConnected = false;

DataContainer primary, secondary;
DataContainer* volatile writeBuffer = &primary;
DataContainer* volatile readBuffer  = &secondary;
volatile bool newDataReady = false;

class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        bleConnected = true;
    }
    void onDisconnect(BLEServer* pServer) {
        bleConnected = false;
        pServer->getAdvertising()->start();
    }
};

void bluetoothSendTask(void* pvParameters) {
    DataContainer container;

    while (true) {
        if (bleConnected && pCharacteristic && newDataReady) {
            uint8_t buffer[sizeof(DataContainer)];
            memcpy(buffer, readBuffer, sizeof(DataContainer));
            pCharacteristic->setValue(buffer, sizeof(DataContainer));
            pCharacteristic->notify();

            newDataReady = false;
        }

        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}

void sensorCollectionTask(void* pvParameters) {
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

        for (uint8_t sensor = 0; sensor < (NUM_OF_SENS / 2); sensor++) {
            for (uint8_t pin = 0; pin < inputs.size(); pin++) {
                digitalWrite(inputs[pin], (sensor >> pin) & 0x1);
            }
            buf->sensor_data[sensor] = analogRead(MUX_OUT_0);
            buf->sensor_data[sensor + NUM_OF_SENS / 2] = analogRead(MUX_OUT_1);
        }

        if (!newDataReady) {
            DataContainer* temp = readBuffer;
            readBuffer = buf;
            writeBuffer = temp;
            newDataReady = true;
        }
        
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void setupBLE() {
    BLEDevice::init("Hand_Learner");
    BLEDevice::setMTU(sizeof(DataContainer) + 3);
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    BLEService* pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY);

    pCharacteristic->addDescriptor(new BLE2902());

    pService->start();
    pServer->getAdvertising()->start();
}

void setupMPU() {
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
            digitalWrite(BUILTIN_LED, LOW);
            delay(100);
            digitalWrite(BUILTIN_LED, HIGH);
            delay(100);
        }
    }
    Serial.println("MPU6050 connection successful");
}

void setup() {
    Serial.begin(115200);

    pinMode(BUILTIN_LED, OUTPUT);

    for (uint16_t pin : inputs) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

    setupMPU();
    setupBLE();

    xTaskCreatePinnedToCore(sensorCollectionTask, "SensorCollector", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(bluetoothSendTask, "BluetoothStreamer", 4096, NULL, 1, NULL, 1);
}

void loop() {}
