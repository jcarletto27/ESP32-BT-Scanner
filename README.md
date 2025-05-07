# ESP32 Bluetooth Scanner Library

A simple Arduino library for ESP32 to scan for nearby Bluetooth Classic devices and retrieve their names and MAC addresses.

## Features

* Initializes the ESP32's Bluetooth Classic module.
* Scans for discoverable Bluetooth Classic (not BLE) devices.
* Returns a list of found devices, including their name and MAC address.
* Simple, blocking scan operation.
* Example sketch provided for continuous scanning.

## Requirements

* ESP32 board
* Arduino IDE with ESP32 core installed.
* This library relies on the `BluetoothSerial.h` library, which is part of the ESP32 Arduino core.

## Installation

1.  **Download:** Download the library files (or clone the repository).
2.  **Locate Arduino Libraries Folder:**
    * On Windows: `Documents\Arduino\libraries`
    * On macOS: `~/Documents/Arduino/libraries`
    * On Linux: `~/Arduino/libraries`
3.  **Install:**
    * Create a new folder named `ESP32BluetoothScanner` inside your Arduino `libraries` folder.
    * Copy the following files into the `ESP32BluetoothScanner` folder:
        * `ESP32BluetoothScanner.h`
        * `ESP32BluetoothScanner.cpp`
        * `keywords.txt`
    * Create an `examples` subfolder inside `ESP32BluetoothScanner`.
    * Inside `examples`, create another subfolder named `BasicScan`.
    * Copy the `BasicScan.ino` example sketch into `ESP32BluetoothScanner/examples/BasicScan/`.
4.  **Restart Arduino IDE:** If the Arduino IDE was open, restart it to recognize the new library.

## How to Use

1.  **Include the Library:**
    ```cpp
    #include <ESP32BluetoothScanner.h>
    ```
2.  **Create an Instance:**
    ```cpp
    ESP32BluetoothScanner btScanner;
    ```
3.  **Initialize in `setup()`:**
    It's crucial to initialize the Bluetooth module. You can optionally provide a name for your ESP32 device as it appears to other Bluetooth devices.
    ```cpp
    void setup() {
      Serial.begin(115200);
      // ... other setup code ...

      if (btScanner.begin("MyESP32Scanner")) {
        Serial.println("Bluetooth initialized successfully.");
      } else {
        Serial.println("Failed to initialize Bluetooth scanner. Halting.");
        while(true);
      }
    }
    ```
4.  **Scan for Devices in `loop()` (or as needed):**
    The `scanClassicDevices()` method performs the scan. It takes the scan duration in seconds as an argument. This is a **blocking call**.
    ```cpp
    void loop() {
      // ... other loop code ...

      Serial.println("Starting scan...");
      // Scan for 10 seconds
      std::vector<BTDevice> discoveredDevices = btScanner.scanClassicDevices(10);

      if (discoveredDevices.empty()) {
        Serial.println("No devices found.");
      } else {
        Serial.print("Found ");
        Serial.print(discoveredDevices.size());
        Serial.println(" device(s):");
        for (size_t i = 0; i < discoveredDevices.size(); i++) {
          Serial.print("  Name: ");
          Serial.print(discoveredDevices[i].name);
          Serial.print(" | Address: ");
          Serial.println(discoveredDevices[i].address);
        }
      }
      delay(5000); // Wait before next scan
    }
    ```
5.  **Device Information:**
    The `scanClassicDevices()` method returns a `std::vector<BTDevice>`. Each `BTDevice` struct contains:
    * `String name`: The advertised name of the Bluetooth device.
    * `String address`: The MAC address of the Bluetooth device.

Refer to the `BasicScan.ino` example sketch (available via `File > Examples > ESP32BluetoothScanner > BasicScan` in the Arduino IDE) for a complete continuous scanning implementation.

## Library Structure

* `ESP32BluetoothScanner.h`: Header file defining the `ESP32BluetoothScanner` class and `BTDevice` struct.
* `ESP32BluetoothScanner.cpp`: Source file with the implementation of the class methods.
* `keywords.txt`: For syntax highlighting in the Arduino IDE.
* `examples/`: Contains example sketches.
    * `BasicScan/BasicScan.ino`: Demonstrates continuous scanning for Bluetooth Classic devices.

## Important Notes

* **Bluetooth Classic Only:** This library currently scans for Bluetooth Classic devices, not Bluetooth Low Energy (BLE) devices.
* **Blocking Scan:** The `scanClassicDevices()` method is blocking. This means your ESP32's program execution will pause at that line until the scan duration is complete. If you need to perform other tasks concurrently with scanning, you would need to implement more advanced non-blocking techniques (e.g., using FreeRTOS tasks on the ESP32).
* **Memory:** Be mindful of memory usage, especially if you expect to find a very large number of devices or store their details for extended periods. The `std::vector` will grow as devices are found.
* **ESP32 Resources:** Bluetooth functionality consumes ESP32 resources (RAM, processing).

## Troubleshooting

* **"Failed to initialize Bluetooth scanner":**
    * Ensure your ESP32 board is correctly configured in the Arduino IDE.
    * This can sometimes happen if other parts of your code are conflicting with Bluetooth resources or if there's an issue with the ESP32's Bluetooth hardware/firmware.
* **Crashes/Reboots (e.g., Heap Errors):**
    * The library was updated to fix a common heap corruption issue related to how `BTScanResults` are handled by the underlying ESP32 BluetoothSerial library. Ensure you are using the latest version of this library.
    * Memory issues can still occur if your application is very memory intensive.

## Contributing

Contributions, issues, and feature requests are welcome. Please open an issue or submit a pull request if you have improvements.
