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

// Custom onDataSent callback
void customOnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Data sent to: ");
    ESPNowManager::printMacAddress(mac_addr);
    Serial.print("Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

// Custom onDataRecv callback
void customOnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
    Serial.print("Data received from: ");
    ESPNowManager::printMacAddress(recv_info->src_addr);
    Serial.print("Data length: ");
    Serial.println(len);

    if (len == sizeof(SampleData)) {
        memcpy(&receivedData, data, sizeof(SampleData));
        Serial.print("Received Value 1: ");
        Serial.println(receivedData.value1);
        Serial.print("Received Value 2: ");
        Serial.println(receivedData.value2);
        Serial.print("Received Value 3: ");
        Serial.println(receivedData.value3);
    } else {
        Serial.println("Received data size mismatch");
    }
}

void setup() {
    Serial.begin(115200);
    espNowManager->begin();
    espNowManager->setOnDataSent(customOnDataSent);
    espNowManager->setOnDataRecv(customOnDataRecv);

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
}