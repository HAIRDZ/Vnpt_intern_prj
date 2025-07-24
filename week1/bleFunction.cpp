#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

BLECharacteristic* charReceive;    //BLEchar = value container co the dung dc boi ca server va client
BLECharacteristic* charSend;
bool isConnected      = false;
std::string inputData = "";

BLEServer* bleServer;
BLEService* bleService;
BLEAdvertising* bleAdvertising;

void setup() {
  Serial.begin(9600);

  BLEDevice::init("ESP32_BLE");    // BLEDevice::init(deviceName)
  bleServer = BLEDevice::createServer();    // A new instance of the server.

  bleService = bleServer->createService("12345678-1234-1234-1234567890ab");    // createService(const char* uuid)

  //createCharacteristic(BLEUUID(uuid), properties) khaibao chuc nang
  charReceive = bleService->createCharacteristic(
    "abcd1234-ab12-cd34-ef56-1234567890ab",
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE
  );

  charSend = bleService->createCharacteristic(
    "abcd1234-ab12-cd34-ef56-1234567890cd",
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE |
    BLECharacteristic::PROPERTY_NOTIFY
  );

  bleService->start();

  bleAdvertising = BLEDevice::getAdvertising();
  bleAdvertising->addServiceUUID("12345678-1234-1234-1234567890ab");
  bleAdvertising->setScanResponse(true);    //thong tin thiet bi khi dc s
  BLEDevice::startAdvertising();
}

void loop() {
  // Kiem tra client
  if (bleServer->getConnectedCount() > 0) {
    isConnected = true;
  } else {
    isConnected = false;
  }

  if (isConnected) {
    // set chrSend = Hi
    charSend->setValue("Hi");
    Serial.println("Da gui");
    delay(100);
  }

  std::string currentData = charReceive->getValue();    //Retrieve the current value of the characteristic.
  if (currentData != inputData) {
    inputData = currentData;
    Serial.print("Received: ");
    Serial.println(inputData.c_str());
  }
}
