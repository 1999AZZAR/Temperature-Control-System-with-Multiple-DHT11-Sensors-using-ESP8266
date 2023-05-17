#include <DHT.h>
#include <ESP8266WiFi.h>

// Wi-Fi credentials
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// DHT sensor pin on master
const int DHT_PIN = D1; // Replace with actual pin number

// Setpoint temperature potentiometer pin on master
const int POT_PIN = A0; // Replace with actual pin number

// Setpoint temperature range
const double MIN_SETPOINT = 20.0; // Minimum desired temperature
const double MAX_SETPOINT = 30.0; // Maximum desired temperature

// Variables
double setpoint = 0.0;
double input = 0.0;
double output = 0.0;

// DHT sensor
DHT dht(DHT_PIN, DHT11);

// Wi-Fi client
WiFiClient client;

// Wi-Fi server port
const int SERVER_PORT = 80;

// Define message separator
const char separator = ',';

// Timing variables
unsigned long lastSlaveDataRequestTime = 0;
unsigned long slaveDataRequestInterval = 280; // Interval between data requests to the slave
unsigned long lastTemperatureCalculationTime = 0;
unsigned long temperatureCalculationInterval = 500; // Interval between temperature calculations on the master

// Slave temperature variables
double slaveTemperatureSum = 0.0;
int numSlaves = 0;

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  // Wait for Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Print the IP address of the master
  Serial.print("Master IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize DHT sensor on master
  dht.begin();

  // Setpoint range and initial value
  setpoint = MIN_SETPOINT;

  Serial.println("Master ESP8266 initialized.");
}

void loop() {
  // Read the potentiometer value
  int potValue = analogRead(POT_PIN);
  setpoint = map(potValue, 0, 1023, MIN_SETPOINT, MAX_SETPOINT);

  // Request temperature data from the slave
  if (millis() - lastSlaveDataRequestTime >= slaveDataRequestInterval) {
    requestSlaveData();
    lastSlaveDataRequestTime = millis();
  }

  // Handle communication with the slave
  handleSlaveCommunication();

  // Read temperature data from the slave(s) if available
  if (numSlaves > 0) {
    input = (slaveTemperatureSum + getMasterTemperature()) / (numSlaves + 1);
  } else {
    input = getMasterTemperature();
  }

  // Compute the PID control output
  output = computePID();

  // Set the output value to the PWM pin or actuator on the master board
  analogWrite(D6, output);

  // Print temperature and setpoint for debugging
  Serial.print("Temperature: ");
  Serial.print(input);
  Serial.print(" °C\t");
  Serial.print("Setpoint: ");
  Serial.print(setpoint);
  Serial.println(" °C");

  // Adjust timing variables based on the number of connected slaves
  adjustTiming();
}

// Function to send request to the slave board for temperature data
void requestSlaveData() {
  if (client.connect("SlaveIP", SERVER_PORT)) { // Replace with the actual IP address of the slave board
    client.print("request");
    client.stop();
  }
}

// Function to handle communication with the slave board
void handleSlaveCommunication() {
  WiFiClient client = server.available();

  if (client) {
    if (client.connected()) {
      String message = "";

      while (client.available()) {
        char c = client.read();
        if (c == separator) {
          // Process received message
          processSlaveMessage(message);
          message = "";
        } else {
          message += c;
        }
      }
    }

    client.stop();
  }
}

// Function to process the message received from the slave board
void processSlaveMessage(String message) {
  double slaveTemperature = message.toDouble();
  slaveTemperatureSum += slaveTemperature;
  numSlaves++;
}

// Function to compute the PID control output
double computePID() {
  // Implementation of PID control algorithm
  // Replace with your own implementation
  // using input, setpoint, and any necessary coefficients

  // Dummy implementation for testing
  return input;
}

// Function to get the master temperature from the DHT sensor
double getMasterTemperature() {
  // Read temperature from the DHT sensor on the master
  float temperature = dht.readTemperature();

  // If the reading failed, return 0.0
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return 0.0;
  }

  return temperature;
}

// Function to adjust timing variables based on the number of connected slaves
void adjustTiming() {
  // Calculate the new slave data request interval
  unsigned long newSlaveDataRequestInterval = map(numSlaves, 0, 10, 280, 500);
  slaveDataRequestInterval = min(newSlaveDataRequestInterval, 500); // Cap the interval at 500 ms

  // Calculate the new temperature calculation interval
  unsigned long newTemperatureCalculationInterval = map(numSlaves, 0, 10, 500, 1000);
  temperatureCalculationInterval = min(newTemperatureCalculationInterval, 1000); // Cap the interval at 1000 ms
}
