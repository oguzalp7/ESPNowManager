
# ESPNowManager

ESPNowManager is a library for managing ESP-NOW communication on ESP32 devices. It allows you to easily add, remove, and manage peers, as well as send and receive data.

## Features

- Initialize ESP-NOW
- Add and remove peers
- Send and receive data
- Print connected peers
- Debug MAC addresses

## Installation

1. Clone or download this repository to your local machine.
2. Copy the `ESPNowManager` folder to your Arduino libraries directory.

## Usage

### Include the Library

Include the `ESPNowManager` library in your Arduino sketch:

```cpp
#include <ESPNowManager.h>
```

### Initialize ESPNowManager

Create an instance of `ESPNowManager` and initialize it:

```cpp
ESPNowManager espNowManager;

void setup() {
    Serial.begin(115200);
    espNowManager.begin();
}
```

### Add a Peer

Add a single peer by providing its MAC address:

```cpp
espNowManager.addPeer("AA:BB:CC:DD:EE:FF");
```

### Add Multiple Peers

Add multiple peers from a JSON string:

```cpp
String macs = "[\"AA:BB:CC:DD:EE:FF\", \"11:22:33:44:55:66\"]";
espNowManager.addPeers(macs);
```

### Remove a Peer

Remove a peer by providing its MAC address:

```cpp
espNowManager.removePeer("AA:BB:CC:DD:EE:FF");
```

### Send Data

Send data to a peer:

```cpp
const char *data = "Hello, ESP-NOW!";
espNowManager.sendData("AA:BB:CC:DD:EE:FF", data, strlen(data));
```

### Print Connected Peers

Print all connected peers to the Serial monitor:

```cpp
espNowManager.printPeers();
```

### Debug MAC Address

Debug a MAC address by printing it to the Serial monitor:

```cpp
espNowManager.debugMacAddress("AA:BB:CC:DD:EE:FF");
```

### Get Received Data

Get the last received data:

```cpp
const uint8_t* receivedData = espNowManager.getReceivedData();
```

## License

This project is licensed under the MIT License.
