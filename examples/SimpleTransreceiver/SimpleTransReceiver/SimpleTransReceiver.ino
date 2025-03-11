#include "ESPNowManager.h"

ESPNowManager* espNowManager = ESPNowManager::getInstance();

String peerAddr = "ff:ff:ff:ff:ff:ff";  // Broadcast address

typedef struct {
  float value1;
  float value2;
  float value3;
} SampleData;

SampleData dataToSend;
SampleData receivedData;

void setup() {
    Serial.begin(115200);
    espNowManager->begin();

    // Add a peer (broadcast address)
    espNowManager->addPeer(peerAddr);
    espNowManager->printPeers();
}

void loop() {
    // Send random values to the peer every 2 seconds
    dataToSend.value1 = random(0, 100) / 10.0;
    dataToSend.value2 = random(0, 100) / 10.0;
    dataToSend.value3 = random(0, 100) / 10.0;
    espNowManager->sendData(peerAddr, &dataToSend, sizeof(dataToSend));
    delay(2000);

    // Grab the received data
    const uint8_t* receivedDataPtr = espNowManager->getReceivedData();
    memcpy(&receivedData, receivedDataPtr, sizeof(SampleData));
    Serial.print("Received Value 1: ");
    Serial.println(receivedData.value1);
    Serial.print("Received Value 2: ");
    Serial.println(receivedData.value2);
    Serial.print("Received Value 3: ");
    Serial.println(receivedData.value3);
}