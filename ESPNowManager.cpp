#include "ESPNowManager.h"

uint8_t ESPNowManager::receivedData[250]; // Adjust the size as needed

ESPNowManager::ESPNowManager() : peer_count(0) {}

// Initialize ESP-NOW
void ESPNowManager::begin() {
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_recv_cb(onDataRecv);
    esp_now_register_send_cb(onDataSent);
}

// Add a single peer
void ESPNowManager::addPeer(const String &peer_addr) {
    uint8_t *mac = macAddressFromStringToByteArray(peer_addr);
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, mac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
    } else {
        Serial.println("Peer added successfully");
        memcpy(peers[peer_count].peer_addr, mac, 6);
        peers[peer_count].channel = peerInfo.channel;
        peers[peer_count].encrypt = peerInfo.encrypt;
        peer_count++;
    }
}


// Add multiple peers from a JSON string
void ESPNowManager::addPeers(const String &macs) {
    DynamicJsonDocument macs_doc(1024);
    DeserializationError macsError = deserializeJson(macs_doc, macs);
    if (macsError) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(macsError.f_str());
        return;
    }
    JsonArray macArray = macs_doc.as<JsonArray>();
    for (JsonVariant v : macArray) {
        String macStr = v.as<String>();
        addPeer(macStr);
    }
}

// Remove a peer
void ESPNowManager::removePeer(const String &peer_addr) {
    uint8_t *mac = macAddressFromStringToByteArray(peer_addr);
    if (esp_now_del_peer(mac) != ESP_OK) {
        Serial.println("Failed to remove peer");
    } else {
        Serial.println("Peer removed successfully");
    }
}

// Send data to a peer
void ESPNowManager::sendData(const String &peer_addr, const void *data, size_t len) {
    uint8_t *mac = macAddressFromStringToByteArray(peer_addr);
    esp_err_t result = esp_now_send(mac, (const uint8_t *)data, len);
    if (result == ESP_OK) {
        Serial.println("Data sent successfully");
    } else {
        Serial.println("Error sending data");
    }
}

// Callback for data received
void ESPNowManager::onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
    Serial.print("Received data from: ");
    printMacAddress(recv_info->src_addr);
    // Serial.print("Data: ");
    // for (int i = 0; i < len; i++) {
    //     Serial.printf("%02X ", incomingData[i]);
    // }
    // Serial.println();
    memcpy(receivedData, incomingData, len);
}

void ESPNowManager::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
    // Serial.print("Last Packet Send Status: ");
    // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    if(status != ESP_NOW_SEND_SUCCESS){
      Serial.println("Message delivery failed.");
    }
}

// Print a MAC address
void ESPNowManager::printMacAddress(const uint8_t *mac_addr) {
    for (int i = 0; i < 6; i++) {
        Serial.printf("%02X", mac_addr[i]);
        if (i < 5) Serial.print(":");
    }
    Serial.println();
}

// Print a single peer's information
void ESPNowManager::printPeer(const peer_info_t &peer) {
    Serial.print("Peer MAC Address: ");
    printMacAddress(peer.peer_addr);
    Serial.print("Channel: ");
    Serial.println(peer.channel);
    Serial.print("Encrypt: ");
    Serial.println(peer.encrypt);
}

// Print all connected peers
void ESPNowManager::printPeers() {
    Serial.println("Connected peers:");
    for (int i = 0; i < peer_count; i++) {
        printPeer(peers[i]);
    }
}

// Convert MAC address from string to byte array
uint8_t* ESPNowManager::macAddressFromStringToByteArray(const String &macStr) {
    static uint8_t mac[6];
    sscanf(macStr.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    return mac;
}

// Debug a MAC address
void ESPNowManager::debugMacAddress(const String &macStr) {
    uint8_t *mac = macAddressFromStringToByteArray(macStr);
    Serial.print("Debug MAC Address: ");
    printMacAddress(mac);
}

// Get the last received data
const uint8_t* ESPNowManager::getReceivedData() const {
    return receivedData;
}