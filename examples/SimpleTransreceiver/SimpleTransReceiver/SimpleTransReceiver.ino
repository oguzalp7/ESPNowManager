#include <ESPNowManager.h>

ESPNowManager espNowManager;

// String peerAddr = "0a:0b:0d::0e:0f"; // Replace with the MAC address of the first ESP32
String peerAddr = "ff:ff:ff:ff:ff:ff";  // use ff:ff:ff:ff:ff:ff for broadcast

typedef struct {
    float value1;
    float value2;
    float value3;
} SampleData;

void setup() {
    Serial.begin(115200);
    espNowManager.begin();
    // espNowManager.debugMacAddress(peerAddr); // Debug the MAC address (prints the peer's MAC address)
    espNowManager.addPeer(peerAddr);
    espNowManager.printPeers();
}

void loop() {
    // send sample data to the peer 2 seconds interval
    SampleData dataToSend = {1.0, 2.0, 3.0};
    espNowManager.sendData(peerAddr, (uint8_t *)&dataToSend, sizeof(dataToSend));
    delay(2000);

    // grab the received data
    const uint8_t* receivedData = espNowManager.getReceivedData();
    SampleData* receivedStruct = (SampleData*)receivedData;
    Serial.print("Received Value 1: ");
    Serial.println(receivedStruct->value1);
    Serial.print("Received Value 2: ");
    Serial.println(receivedStruct->value2);
    Serial.print("Received Value 3: ");
    Serial.println(receivedStruct->value3);
}