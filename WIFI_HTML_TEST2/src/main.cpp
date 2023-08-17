#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "Mi Phone";
const char* password = "fb41f23ed441";

// Create an instance of the server
ESP8266WebServer server(80);

// Sample data log (replace with your actual data log)
String dataLog = "Timestamp,Value1,Value2\n"
                 "2023-07-24 12:00:00,23.5,45.6\n"
                 "2023-07-24 12:01:00,24.0,46.3\n"
                 "2023-07-24 12:02:00,23.8,45.9\n";



// Generate the web page
String getWebPage() {
  String html = "<!DOCTYPE html><html><body>";
  html += "<h1>Data Logger Web Interface</h1>";
  html += "<p>Data Log:</p>";
  html += "<pre>" + dataLog + "</pre>";
  html += "<br>";
  html += "<a href=\"/download\" download=\"data_log.csv\"><button>Download CSV</button></a>";
  html += "</body></html>";
  return html;
}

// Handle root URL
void handleRoot() {
  server.send(200, "text/html", getWebPage());
}


// Handle download request
void handleDownload() {
  server.sendHeader("Content-Disposition", "attachment; filename=data_log.csv");
  server.sendHeader("Cache-Control", "no-cache");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(200, "text/csv", dataLog);
}



void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot); // Handle root URL
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

