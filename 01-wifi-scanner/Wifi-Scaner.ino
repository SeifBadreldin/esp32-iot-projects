#include <WiFi.h>  // ESP32 Wi-Fi library

// ===============================
// Struct to store Wi-Fi network info
// ===============================
struct WiFiNetwork {
  String ssid;          // Network name
  int32_t rssi;         // Signal strength in dBm
  int32_t channel;      // Wi-Fi channel number
  wifi_auth_mode_t encryption; // Encryption type
};

// ===============================
// Function: Convert RSSI to ASCII bar
// Visual indicator of signal strength
// ===============================
String rssiBar(int32_t rssi) {
  int bars = map(rssi, -100, -50, 0, 10); // Map RSSI (-100 to -50 dBm) to 0–10 bars
  bars = constrain(bars, 0, 10);          // Ensure bars are within 0–10
  String bar = "";
  for (int i = 0; i < bars; i++) bar += "|";
  return bar;
}

// ===============================
// Function: Print a single network
// Displays SSID, RSSI, ASCII bar, channel, encryption
// ===============================
void printNetwork(int index, WiFiNetwork net) {
  String enc;
  switch(net.encryption) {
    case WIFI_AUTH_OPEN: enc = "OPEN"; break;
    case WIFI_AUTH_WEP: enc = "WEP"; break;
    case WIFI_AUTH_WPA_PSK: enc = "WPA"; break;
    case WIFI_AUTH_WPA2_PSK: enc = "WPA2"; break;
    case WIFI_AUTH_WPA_WPA2_PSK: enc = "WPA/WPA2"; break;
    case WIFI_AUTH_WPA2_ENTERPRISE: enc = "WPA2 Enterprise"; break;
    default: enc = "UNKNOWN"; break;
  }

  Serial.print("[");
  Serial.print(index);
  Serial.print("] SSID: ");
  Serial.print(net.ssid);
  Serial.print(" | RSSI: ");
  Serial.print(net.rssi);
  Serial.print(" dBm ");
  Serial.print(rssiBar(net.rssi)); // ASCII bar
  Serial.print(" | CH: ");
  Serial.print(net.channel);
  Serial.print(" | ENC: ");
  Serial.println(enc);
}

// ===============================
// Function: Scan Wi-Fi networks, sort, and filter
// ===============================
void scanNetworks() {
  // Print timestamp for scan
  unsigned long t = millis();
  int h = (t / 3600000) % 24;
  int m = (t / 60000) % 60;
  int s = (t / 1000) % 60;
  Serial.printf("\n--- Wi-Fi Scan at %02d:%02d:%02d ---\n", h, m, s);

  int n = WiFi.scanNetworks();      // Perform Wi-Fi scan
  Serial.print("Number of networks found: ");
  Serial.println(n);

  if (n == 0) {
    Serial.println("No networks found");
    return;
  }

  // Store all networks in an array
  WiFiNetwork networks[n];
  for (int i = 0; i < n; i++) {
    networks[i].ssid = WiFi.SSID(i);
    networks[i].rssi = WiFi.RSSI(i);
    networks[i].channel = WiFi.channel(i);
    networks[i].encryption = WiFi.encryptionType(i);
  }

  // ===============================
  // Sort networks by RSSI (strongest first)
  // ===============================
  for (int i = 0; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
      if (networks[j].rssi > networks[i].rssi) {
        WiFiNetwork temp = networks[i];
        networks[i] = networks[j];
        networks[j] = temp;
      }
    }
  }

  // ===============================
  // Print all networks (sorted)
  // ===============================
  Serial.println("\n--- All Networks (sorted by RSSI) ---");
  for (int i = 0; i < n; i++) printNetwork(i, networks[i]);

  // ===============================
  // Print only open networks
  // ===============================
  Serial.println("\n--- Open Networks Only ---");
  bool foundOpen = false;
  for (int i = 0; i < n; i++) {
    if (networks[i].encryption == WIFI_AUTH_OPEN) {
      printNetwork(i, networks[i]);
      foundOpen = true;
    }
  }
  if (!foundOpen) Serial.println("No open networks found.");

  // ===============================
  // Print networks on channels 1, 6, 11
  // ===============================
  Serial.println("\n--- Networks on Channels 1, 6, 11 ---");
  bool foundChannel = false;
  for (int i = 0; i < n; i++) {
    if (networks[i].channel == 1 || networks[i].channel == 6 || networks[i].channel == 11) {
      printNetwork(i, networks[i]);
      foundChannel = true;
    }
  }
  if (!foundChannel) Serial.println("No networks found on channels 1, 6, 11.");
}

// ===============================
// Setup: initialize Serial & Wi-Fi
// ===============================
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 Wi-Fi Scanner starting...");

  WiFi.mode(WIFI_STA);     // Set ESP32 to Station mode
  WiFi.disconnect();       // Disconnect any previous connection
  delay(100);
  Serial.println("ESP32 is now in Station mode (ready to scan).");
}

// ===============================
// Loop: scan networks every 10 seconds
// ===============================
void loop() {
  scanNetworks();          // Perform scan
  delay(10000);            // Wait 10 seconds before next scan
}
