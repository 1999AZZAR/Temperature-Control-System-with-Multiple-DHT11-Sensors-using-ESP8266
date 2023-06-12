# Temperature Control System with Multiple DHT11 Sensors using ESP8266
This repository contains code for a temperature control system implemented using Arduino and ESP8266 boards. The system utilizes multiple DHT11 sensors to measure temperature and applies PID control to maintain a desired setpoint temperature. The system consists of two components: the master board and the slave board. The master board performs temperature calculation and control, while the slave board collects temperature data from DHT11 sensors. The master and slave boards communicate with each other over Wi-Fi.

## Features
- Multiple DHT11 sensors connected to the slave board for temperature measurement
- PID control algorithm for precise temperature regulation
- Wi-Fi communication between master and slave boards
- Automatic adjustment of timing intervals based on the number of connected slaves

## Contents
- master_code.ino: Arduino code for the master board
- slave_code.ino: Arduino code for the slave board
- README.md: Instructions and explanations for using the code

## Requirements
- Arduino IDE
- ESP8266 board
- DHT library

## Usage
1. Connect the master and slave boards to the same Wi-Fi network.
2. Upload the respective code (master_code.ino for the master board and slave_code.ino for the slave board) to their respective boards.
3. Open the serial monitor for each board to view the temperature readings and control actions.

## Flowchart

```mermaid
graph TD
  subgraph Master Board
    A[Start] --> B[Initialize Serial Communication]
    B --> C[Connect to Wi-Fi]
    C --> D[Wait for Wi-Fi Connection]
    D --> E[Print Master IP Address]
    E --> F[Initialize DHT Sensor]
    F --> G[Set Setpoint Range and Initial Value]
    G --> H[Print Initialization Message]
    H --> I[Loop Start]
    I --> J[Read Potentiometer Value]
    J --> K[Map Potentiometer Value to Setpoint Range]
    K --> L[Request Temperature Data from Slave]
    L --> M[Update Last Data Request Time]
    M --> N[Handle Slave Communication]
    N --> O[Check If Client Connected]
    O -- Yes --> P[Process Received Message]
    P --> W[Compute Input Value]
    P --> Q[Reset Message]
    Q --> N
    O -- No --> R[Stop Client Connection]
    R --> S[Compute Master Temperature]
    S --> T[Read Temperature from DHT Sensor]
    T --> U[Check If Reading Failed]
    U -- Yes --> V[Return 0.0]
    U -- No --> W[Compute Input Value]
    W --> X[Compute PID Control Output]
    X --> Y[Set Output Value to PWM Pin or Actuator]
    Y --> Z[Print Temperature and Setpoint]
    Z --> AA[Adjust Timing Variables]
    AA --> I
  end
  
  subgraph Slave Board
    A1[Start] --> B1[Initialize Serial Communication]
    B1 --> C1[Connect to Wi-Fi]
    C1 --> D1[Wait for Wi-Fi Connection]
    D1 --> E1[Print Slave IP Address]
    E1 --> F1[Initialize DHT Sensor]
    F1 --> G1[Start Wi-Fi Server]
    G1 --> H1[Print Initialization Message]
    H1 --> I1[Loop Start]
    I1 --> J1[Read Temperature from DHT Sensor]
    J1 --> K1[Send Temperature Data to Master]
    K1 --> L1[Update Last Temperature Send Time]
  end

  subgraph Network
    Z1((Wi-Fi Network))
    Z1 -.-> C
    Z1 -.-> C1
  end
```

## Contribution
Contributions to the code and project are welcome. Feel free to submit issues or pull requests for any improvements or bug fixes.
