#ifndef ESP32_BLUETOOTH_SCANNER_H
#define ESP32_BLUETOOTH_SCANNER_H

#include "Arduino.h"
#include "BluetoothSerial.h" // For Bluetooth Classic
#include <vector>           // For std::vector

// Structure to hold discovered Bluetooth device information
struct BTDevice {
    String name;    // Device name
    String address; // MAC address
    // int32_t rssi; // Optional: Received Signal Strength Indicator
};

class ESP32BluetoothScanner {
public:
    // Constructor
    ESP32BluetoothScanner();

    // Destructor
    ~ESP32BluetoothScanner();

    // Initializes the Bluetooth Classic module on the ESP32.
    // Must be called before any scanning operations.
    // deviceName: The name your ESP32 will use (optional, but good practice).
    // Returns true if initialization was successful, false otherwise.
    bool begin(String deviceName = "ESP32_Scanner");

    // Scans for nearby Bluetooth Classic devices.
    // This is a blocking call; the function will not return until the scan is complete or timeout.
    // scanTimeSec: The duration of the scan in seconds (e.g., 5-15 seconds).
    // Returns a std::vector of BTDevice structs, each representing a discovered device.
    std::vector<BTDevice> scanClassicDevices(uint8_t scanTimeSec = 10);

    // De-initializes the Bluetooth module.
    // Call this to free resources if Bluetooth is no longer needed.
    void end();

private:
    BluetoothSerial ESP_BT; // Instance of the ESP32 BluetoothSerial class
    bool initialized;       // Flag to track initialization status
};

#endif // ESP32_BLUETOOTH_SCANNER_H
