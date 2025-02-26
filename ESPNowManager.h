#ifndef ESP_NOW_MANAGER_H
#define ESP_NOW_MANAGER_H

#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>

class ESPNowManager {
public:
    ESPNowManager(); // Constructor
    void begin(); // Initialize ESP-NOW
    void addPeer(const String &peer_addr); // Add a single peer
    void addPeers(const String &macs); // Add multiple peers from a JSON string
    void removePeer(const String &peer_addr); // Remove a peer
    void printPeers(); // Print all connected peers
    void sendData(const String &peer_addr, const void *data, size_t len); // Send data to a peer
    static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status); // Callback for data sent
    static void onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len); // Callback for data received
    void debugMacAddress(const String &macStr); // Debug a MAC address
    const uint8_t* getReceivedData() const; // Get the last received data

private:
    struct peer_info_t {
        uint8_t peer_addr[6];
        uint8_t channel;
        uint8_t encrypt;
    };

    peer_info_t peers[20]; // Adjust the size as needed
    int peer_count;
    static uint8_t receivedData[250]; // Adjust the size as needed

    uint8_t* macAddressFromStringToByteArray(const String &macStr); // Convert MAC address from string to byte array
    static void printMacAddress(const uint8_t *mac_addr); // Print a MAC address
    void printPeer(const peer_info_t &peer); // Print a single peer's information
};

#endif // ESP_NOW_MANAGER_H