#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "Union Audio Two"; // Your WiFi name or SSID
const char* password = "Union_Work"; // Your WiFi password

const char* host = "script.google.com"; // Google Apps Script host
const int httpPort = 443; // HTTPS port

WiFiClientSecure client;
String GAS_ID = "AKfycbydUAEyJbhHwW3uYuWpWYweq2MM-rfVp3SxgzGcN9CzzFXtr4gvsZcSvqvTxSWKGgrVZA"; // Spreadsheet script ID

void setup() {
  Serial.begin(115200);
  delay(500);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Successfully connected to: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  client.setInsecure();
}

void sendData() {
  Serial.println("==========");
  Serial.print("Connecting to: ");
  Serial.println(host);

  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }

  String url = "/macros/s/" + GAS_ID + "/exec?temperature=11&humidity=22";
  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("Request sent");

  // Read and display response headers
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received");
      break;
    }
    Serial.println("Response Header: " + line);
  }

  // Read and display response body
  Serial.println("Response Body:");
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }

  client.stop();

  if (client.connected()) {
    Serial.println("Connection still open");
  } else {
    Serial.println("Connection closed");
  }

  Serial.println("==========");
  Serial.println();
}

void loop() {
  sendData();
  delay(5000); // Delay for 5 seconds
}
