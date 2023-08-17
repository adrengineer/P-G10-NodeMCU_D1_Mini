#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define WIFI_SSID "Mi Phone"
#define WIFI_PASSWORD "fb41f23ed441"

//#define WIFI_SSID "Union Audio Two"
//#define WIFI_PASSWORD "Union_Work"


#define COLLECTOR_ID "2PACX-1vSNQXkhgskXNBK4VIaigjM7S7rg4WQCzOX8B_L9jgq5Ae6BJqfBBLcTMsPBZzlu25RCmeBywIWcGOsp"

#define LOCATION "garden-bed"

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  
}

void logData() {
  float temp = 25.0;      // Simulated temperature value (replace with actual sensor reading)
  float humidity = 50.0;  // Simulated humidity value (replace with actual sensor reading)
  int moisture = 123;     // Simulated moisture value (replace with actual sensor reading)

  WiFiClientSecure client;
  if (client.connect("script.google.com", 443)) {
    String url = "/macros/s/" + String(COLLECTOR_ID);
    url += "/exec?tab=weather&location=";
    url += LOCATION;
    url += "&p1=" + String(temp);
    url += "&p2=" + String(humidity);
    url += "&p3=" + String(moisture);

    Serial.print("Sending URL: ");
    Serial.println(url); // Debugging: Print the URL being sent

    String request = "GET " + url + " HTTP/1.1\r\n" +
                     "Host: script.google.com\r\n" +
                     "Connection: close\r\n\r\n";

    client.print(request);

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        break;
      }
    }

    String response = client.readString(); // Read the complete response
    Serial.print("Response from server:\n");
    Serial.println(response); // Debugging: Print the response received from the server

    client.stop();

    Serial.println("Data logged successfully!"); // Serial confirmation
  } 
  else {
    Serial.println("Failed to connect to Google Sheets server."); // Serial confirmation
  }
}

void loop() {
  logData();
  delay(500); // Simulate 500 ms delay
}