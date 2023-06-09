# Slave Code
This folder contains the Arduino code for the slave board in the Temperature Control System with Multiple DHT11 Sensors. The slave board is responsible for collecting temperature data from DHT11 sensors and sending it to the master board over Wi-Fi.

## Usage
1. Connect the slave board to power and upload the code to the board using the Arduino IDE.
2. Adjust the Wi-Fi credentials in the code to match your network settings.
3. Connect a DHT11 sensor to the slave board and configure the appropriate pin in the code.
4. Open the serial monitor to view the temperature readings.

## Code Explanation
- The code initializes the DHT sensor and Wi-Fi connection.
- It waits for requests from the master board to send temperature data.
- Upon receiving a request, the slave board reads the temperature from the DHT11 sensor and calculates the average temperature for the last few readings.
- The average temperature is sent to the master board over Wi-Fi.
- The code repeats this process at regular intervals.

## Flowchart

```mermaid
graph TD
  A[Start] --> B[Initialize Serial Communication]
  B --> C[Connect to Wi-Fi]
  C --> D[Wait for Wi-Fi Connection]
  D --> E[Print Slave IP Address]
  E --> F[Initialize DHT Sensor]
  F --> G[Start Wi-Fi Server]
  G --> H[Print Initialization Message]
  H --> I[Loop Start]
  I --> J[Read Temperature]
  J --> K[Check Reading Success]
  K -- Yes --> L[Send Temperature to Master]
  L --> M[Update Last Send Time]
  M --> I
  K -- No --> N[Print Error Message]
  N --> I
```

Note: Make sure to update the IP address of the master board in the code to match your network configuration.
