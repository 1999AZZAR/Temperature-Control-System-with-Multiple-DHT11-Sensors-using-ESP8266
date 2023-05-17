#include <DHT.h>
#include <ESP8266WiFi.h>

// Wi-Fi credentials
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// DHT sensor pin on slave
const int DHT_PIN = D1; // Replace with actual pin number

// Wi-Fi server port
const int SERVER_PORT = 80;

// DHT sensor
DHT dht(DHT_PIN, DHT11);

// Define message separator
const char separator = ',';

// Timing variables
unsigned long lastTemperatureSendTime = 0;
const unsigned long temperatureSendInterval = 50; // Interval between temperature readings on the slave

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

  // Print the IP address of the slave
  Serial.print("Slave IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize DHT sensor on slave
  dht.begin();

  // Start the Wi-Fi server
  server.begin();

  Serial.println("Slave ESP8266 initialized.");
}

void loop() {
  // Read temperature from the DHT sensor on the slave
  float temperature = dht.readTemperature();

  // If the reading failed, skip this loop iteration
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Send average temperature to the master
  if (millis() - lastTemperatureSendTime >= temperatureSendInterval) {
    sendTemperature(temperature);
    lastTemperatureSendTime = millis();
  }
}

// Function to send the average temperature to the master
void sendTemperature(float temperature) {
  WiFiClient client;

  if (client.connect("MasterIP", SERVER_PORT)) { // Replace with the actual IP address of the master board
    client.print(temperature);
    client.print(separator);
    client.stop();
  }
}
