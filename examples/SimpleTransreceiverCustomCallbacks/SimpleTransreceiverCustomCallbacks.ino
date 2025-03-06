#include "ESPNowManager.h"

ESPNowManager espNowManager;

String peerAddr = "ff:ff:ff:ff:ff:ff";  // use ff:ff:ff:ff:ff:ff for broadcast

typedef struct {
  float value1;
  float value2;
  float value3;
} SampleData;

// Custom onDataSent callback
void customOnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Custom onDataSent - Last Packet Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Custom onDataRecv callback
void customOnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
    
    Serial.print("Data: ");
    for (int i = 0; i < len; i++) {
        Serial.printf("%02X ", data[i]);
    }
    Serial.println();

    SampleData* receivedStruct = (SampleData*)data;
    Serial.print("Received Value 1: ");
    Serial.println(receivedStruct->value1);
    Serial.print("Received Value 2: ");
    Serial.println(receivedStruct->value2);
    Serial.print("Received Value 3: ");
    Serial.println(receivedStruct->value3);
}



void setup() {
    Serial.begin(115200);
    espNowManager.begin();
    espNowManager.setOnDataSent(customOnDataSent);    // 
    espNowManager.setOnDataRecv(customOnDataRecv);    // register custom receive callback.

    // Add a peer (example MAC address)
    espNowManager.addPeer(peerAddr);
    espNowManager.printPeers();
}

void loop() {
    // Main loop does nothing, data is handled in the callbacks

    // send sample data to the peer 2 seconds interval
    SampleData dataToSend = {4.0, 5.0, 6.0};
    espNowManager.sendData(peerAddr, (uint8_t *)&dataToSend, sizeof(dataToSend));
    delay(2000);

    // use builtin receive callback
    const uint8_t* receivedData = espNowManager.getReceivedData();
    SampleData* receivedStruct = (SampleData*)receivedData;
    Serial.print("Received Value 1: ");
    Serial.println(receivedStruct->value1);
    Serial.print("Received Value 2: ");
    Serial.println(receivedStruct->value2);
    Serial.print("Received Value 3: ");
    Serial.println(receivedStruct->value3);

}