#include "ESP32BluetoothScanner.h"

// Constructor
ESP32BluetoothScanner::ESP32BluetoothScanner() : initialized(false) {
    // The ESP_BT member is automatically constructed.
}

// Destructor
ESP32BluetoothScanner::~ESP32BluetoothScanner() {
    end(); // Ensure Bluetooth resources are released
}

// Initializes Bluetooth Classic
bool ESP32BluetoothScanner::begin(String deviceName) {
    if (initialized) {
        Serial.println("Bluetooth already initialized.");
        return true;
    }

    // Start Bluetooth Classic.
    // The deviceName parameter sets the name of the ESP32 device itself.
    if (!ESP_BT.begin(deviceName)) {
        Serial.println("Error: Failed to initialize Bluetooth Classic module (esp_bt_controller or bluedroid failed).");
        initialized = false;
        return false;
    }

    Serial.println("Bluetooth Classic Initialized successfully.");
    Serial.print("ESP32 Device Name: ");
    Serial.println(deviceName);

    initialized = true;
    return true;
}

// Scans for Bluetooth Classic devices
std::vector<BTDevice> ESP32BluetoothScanner::scanClassicDevices(uint8_t scanTimeSec) {
    std::vector<BTDevice> devicesFound; // Vector to store found devices

    if (!initialized) {
        Serial.println("Error: Bluetooth module not initialized. Call begin() first.");
        return devicesFound; // Return empty vector
    }

    Serial.print("Starting Bluetooth Classic scan for ");
    Serial.print(scanTimeSec);
    Serial.println(" seconds...");

    // `discover()` is a blocking call. It performs the device discovery.
    // The timeout for discover() is in milliseconds.
    // It returns a pointer to a BTScanResults object.
    // IMPORTANT: This pointer is typically to an internal object within BluetoothSerial,
    // and SHOULD NOT be deleted by the caller. Its contents are managed by the BluetoothSerial library.
    BTScanResults* discoveredDevices = ESP_BT.discover(scanTimeSec * 1000);

    if (discoveredDevices) {
        int count = discoveredDevices->getCount();
        Serial.print("Scan complete. Found ");
        Serial.print(count);
        Serial.println(" device(s).");

        for (int i = 0; i < count; i++) {
            BTAdvertisedDevice* d = discoveredDevices->getDevice(i);
            if (d) { // Check if the device pointer is valid
                BTDevice device;

                // Get device name
                std::string name_str = d->getName();
                if (name_str.empty()) {
                    device.name = "Unknown";
                } else {
                    device.name = String(name_str.c_str());
                }

                // Get device MAC address
                device.address = d->getAddress().toString();

                // Optional: Get RSSI (Received Signal Strength Indicator)
                // if (d->haveRSSI()) {
                //    device.rssi = d->getRSSI();
                // } else {
                //    device.rssi = 0; // Or some indicator for N/A
                // }

                devicesFound.push_back(device);
            }
        }
        // DO NOT DELETE discoveredDevices HERE!
        // The BTScanResults object pointed to by discoveredDevices is managed
        // by the ESP_BT (BluetoothSerial) instance. Deleting it will cause a crash
        // because it's not memory that was allocated with `new` by this part of the code.
        // The BluetoothSerial library will clear/reuse these results on the next scan or its own cleanup.
        // delete discoveredDevices; // <<-- THIS WAS THE BUG, REMOVE IT
        // discoveredDevices = nullptr;

    } else {
        Serial.println("Scan failed or an error occurred during discovery (discoveredDevices is null).");
    }

    Serial.println("Bluetooth Classic scan finished.");
    return devicesFound;
}

// De-initializes Bluetooth
void ESP32BluetoothScanner::end() {
    if (initialized) {
        ESP_BT.end(); // Gracefully stop Bluetooth services
        initialized = false;
        Serial.println("Bluetooth Classic de-initialized.");
    }
}
