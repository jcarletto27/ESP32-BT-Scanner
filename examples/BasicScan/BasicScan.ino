#include <ESP32BluetoothScanner.h> // Include the library

// Create an instance of the scanner
ESP32BluetoothScanner btScanner;

// Define the scan interval in milliseconds
const unsigned long scanInterval = 15000; // Scan every 15 seconds
unsigned long previousMillis = 0;        // Stores the last time a scan was initiated

void setup() {
  Serial.begin(115200);
  // Wait for Serial port to connect (especially for native USB ESP32 boards)
  while (!Serial && millis() < 5000) { 
    delay(100);
  }

  Serial.println("\n\nESP32 Bluetooth Classic Continuous Device Scanner");
  Serial.println("-------------------------------------------------");

  // Initialize the Bluetooth scanner.
  // This needs to be done only once.
  if (btScanner.begin("ESP32_Continuous_Scanner")) {
    Serial.println("Bluetooth initialized successfully for continuous scanning.");
  } else {
    Serial.println("Critical Error: Failed to initialize Bluetooth scanner. Halting.");
    while (true) { // Halt execution if BT initialization fails
      delay(1000);
    }
  }
  Serial.println("-------------------------------------------------");
}

void loop() {
  // Get the current time
  unsigned long currentMillis = millis();

  // Check if it's time to start a new scan
  // This implements a non-blocking delay for the scan interval.
  if (currentMillis - previousMillis >= scanInterval) {
    // Save the last time a scan was initiated
    previousMillis = currentMillis;

    Serial.println("\nStarting new scan for Bluetooth Classic devices...");

    // Perform a scan for 10 seconds.
    // This is a blocking call, the ESP32 will be busy during this time.
    // Consider the impact on other tasks if your loop needs to be more responsive.
    std::vector<BTDevice> discoveredDevices = btScanner.scanClassicDevices(10); // Scan duration: 10 seconds

    if (discoveredDevices.empty()) {
      Serial.println("No Bluetooth Classic devices found in this scan cycle.");
    } else {
      Serial.print("Scan finished. Found ");
      Serial.print(discoveredDevices.size());
      Serial.println(" device(s):");
      Serial.println("------------------------------------");
      for (size_t i = 0; i < discoveredDevices.size(); i++) {
        Serial.print(i + 1);
        Serial.print(". Name: ");
        Serial.print(discoveredDevices[i].name);
        Serial.print(" | Address: ");
        Serial.println(discoveredDevices[i].address);
        // If you enabled RSSI in BTDevice struct and fetching:
        // Serial.print(" | RSSI: ");
        // Serial.println(discoveredDevices[i].rssi);
      }
      Serial.println("------------------------------------");
    }
    Serial.print("Next scan in approx. ");
    Serial.print(scanInterval / 1000);
    Serial.println(" seconds.");
  }

  // You can add other non-blocking tasks here if needed.
  // For example, checking sensor readings, updating a display, etc.
  // delay(10); // Small delay to yield to other tasks if any (optional)
}
