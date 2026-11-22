#include "WiFi.h"

#define LED0 23   // LED 1 connected to GPIO 23
#define LED1 22   // LED 2 connected to GPIO 22
#define LED2 19   // LED 3 connected to GPIO 19

// Local Wi-Fi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Start the web server on port 80 (HTTP default)
WiFiServer server(80);

// HTML page content
String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>ESP32 LED Control</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  body { font-family: Arial; text-align: center; background-color: #f4f4f4; }
  h1, h2 { color: #333; }
  button {
    width: 140px; height: 45px; margin: 8px;
    font-size: 16px; border-radius: 10px; border: none; cursor: pointer;
  }
  .on { background-color: #4CAF50; color: white; }
  .off { background-color: #f44336; color: white; }
  .auto { background-color: #2196F3; color: white; }
</style>
</head>
<body>
  <h1>ESP32 DevKitC LED Control</h1>
  <h2>Control 3 LEDs from Web Page</h2>
  <form>
    <button class="on" name="LED0" value="ON" type="submit">LED0 ON</button>
    <button class="off" name="LED0" value="OFF" type="submit">LED0 OFF</button><br><br>
    <button class="on" name="LED1" value="ON" type="submit">LED1 ON</button>
    <button class="off" name="LED1" value="OFF" type="submit">LED1 OFF</button><br><br>
    <button class="on" name="LED2" value="ON" type="submit">LED2 ON</button>
    <button class="off" name="LED2" value="OFF" type="submit">LED2 OFF</button><br><br>
    <button class="off" name="ALL" value="OFF" type="submit">ALL OFF</button><br><br>
    <button class="auto" name="AUTO" value="ON" type="submit">TRAFFIC LIGHT</button>
  </form>
</body>
</html>
)rawliteral";

// Function to connect to Wi-Fi
void Connect_WiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// Setup function
void setup() {
  Serial.begin(115200);

  // Configure LED pins as outputs
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED0, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  // Connect to Wi-Fi and start server
  Connect_WiFi();
  server.begin();
  Serial.println("Web server started!");
}

// Function for traffic light sequence
void trafficLight() {
  Serial.println("Starting Traffic Light Sequence...");
  for (int i = 0; i < 3; i++) { // run sequence 3 times
    digitalWrite(LED0, HIGH); // Red
    Serial.println("Red ON");
    delay(2000);
    digitalWrite(LED0, LOW);

    digitalWrite(LED1, HIGH); // Yellow
    Serial.println("Yellow ON");
    delay(1000);
    digitalWrite(LED1, LOW);

    digitalWrite(LED2, HIGH); // Green
    Serial.println("Green ON");
    delay(2000);
    digitalWrite(LED2, LOW);
  }
  Serial.println("Traffic Light Sequence Complete.");
}

// Main loop
void loop() {
  WiFiClient client = server.available(); // Wait for a client to connect
  if (!client) return;

  Serial.println("New client connected.");
  String request = client.readStringUntil('\r');
  client.flush();

  // Check for LED control requests
  if (request.indexOf("LED0=ON") != -1) { digitalWrite(LED0, HIGH); Serial.println("LED0 ON"); }
  if (request.indexOf("LED0=OFF") != -1) { digitalWrite(LED0, LOW); Serial.println("LED0 OFF"); }

  if (request.indexOf("LED1=ON") != -1) { digitalWrite(LED1, HIGH); Serial.println("LED1 ON"); }
  if (request.indexOf("LED1=OFF") != -1) { digitalWrite(LED1, LOW); Serial.println("LED1 OFF"); }

  if (request.indexOf("LED2=ON") != -1) { digitalWrite(LED2, HIGH); Serial.println("LED2 ON"); }
  if (request.indexOf("LED2=OFF") != -1) { digitalWrite(LED2, LOW); Serial.println("LED2 OFF"); }

  // OFF ALL LEDs
  if (request.indexOf("ALL=OFF") != -1) {
    digitalWrite(LED0, LOW);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    Serial.println("ALL LEDs OFF");
  }

  // Automatic traffic light mode
  if (request.indexOf("AUTO=ON") != -1) {
    trafficLight();
  }

  // Send HTML page to client
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.print(html);
  client.println();

  delay(1);
  client.stop();
  Serial.println("Client disconnected.");
}
