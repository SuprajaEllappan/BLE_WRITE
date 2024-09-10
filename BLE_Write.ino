#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// UUIDs for the service and characteristics
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC1_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a1"
#define CHARACTERISTIC2_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a2"
#define CHARACTERISTIC3_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a3"

// Declare characteristic pointers
BLECharacteristic *pCharacteristic1;
BLECharacteristic *pCharacteristic2;
BLECharacteristic *pCharacteristic3;

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) override {
    String rxValue = pCharacteristic->getValue().c_str(); 

    if (rxValue.length() > 0) {
      Serial.println("*********");
      Serial.print("Received Value: ");

      if (pCharacteristic == pCharacteristic1) {
        // Handle string value
        Serial.println("String Value: " + rxValue);
      } else if (pCharacteristic == pCharacteristic2) {
        // Handle char value
        if (rxValue.length() > 0) {
          char receivedChar = rxValue[0];
          Serial.print("Char Value: ");
          Serial.println(receivedChar);
        } else {
          Serial.println("No char value received.");
        }
      } else if (pCharacteristic == pCharacteristic3) {
        // Handle int value (convert from string to int)
        int receivedInt = rxValue.toInt(); // Convert String to int
        Serial.print("Int Value: ");
        Serial.println(receivedInt);
      }

    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("MyESP32");

  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic1 = pService->createCharacteristic(
                                         CHARACTERISTIC1_UUID,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharacteristic1->setCallbacks(new MyCallbacks());

  pCharacteristic2 = pService->createCharacteristic(
                                         CHARACTERISTIC2_UUID,
                                         BLECharacteristic::PROPERTY_WRITE 
                                       );
  pCharacteristic2->setCallbacks(new MyCallbacks());

  pCharacteristic3 = pService->createCharacteristic(
                                         CHARACTERISTIC3_UUID,
                                         BLECharacteristic::PROPERTY_WRITE 
                                       );
  pCharacteristic3->setCallbacks(new MyCallbacks());

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("Characteristic defined! Now you can write to it from your phone!");
}

void loop() {
  // Main loop can be left empty as all data handling is done in the callback
}

