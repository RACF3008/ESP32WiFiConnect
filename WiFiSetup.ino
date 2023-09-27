/*============================================================
File:                 WiFiPassWebServer.ino
Authors:              Rodrigo Alejandro Cruz Fagiani (20073)
Creation Date:        -2023
Title:                WiFi Password Configuration Test

Description:
Allow users to connect ESP32 to internet by introducing WiFi credentials
in a Web Server UI.

Backup Links:
- https://www.youtube.com/watch?v=pL3dhGtmcMY&t=741s&ab_channel=KrisKasprzak

============================================================*/

//====================================================================
// LIBRARIES
//====================================================================
// Arduino Libraries
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

// Header Files
#include "index.h"

//====================================================================
// CONSTANTS
//====================================================================
// ### Test Constants
// #define USE_INTRANET
// #define LOCAL_SSID "ARRIS-CF84"
// #define LOCAL_PASS "70DFF7A1CF84"
// #define LOCAL_SSID "FEMSAMAN"
// #define LOCAL_PASS "3m80C3N2021"
// #define LOCAL_SSID "rodrigo_hotspot"
// #define LOCAL_PASS "Fideo123"

// WebServer WiFi Credentials
#define AP_SSID "PetFeeder"
#define AP_PASS ""

//====================================================================
// PROGRAM VARIABLES
//====================================================================
// XML variable to send web server
char XML[2048];
char buf[32];
char wifi_net[32] = "";                                // WiFi network name
char wifi_pas[32] = "";                                // WiFi password name

String wifi_network = "";
String wifi_password = "";

bool server_status = false;                             // Check if server is on or off

int connection_timeOut = 0;
int connection_status = 0;                              // 0 - Not started, 1 - Trying, 2 - Connected, 3 - Failed
int XML_flag_old = 0;
int XML_flag = 0;                                       // used to print XML once

// Web server creation variables
IPAddress Actual_IP;
IPAddress PageIP(10, 0, 0, 1);
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip;
WebServer server(80);

//====================================================================
// SETUP
//====================================================================
void setup() {
  // Begin serial communication to test
  Serial.begin(115200);
  delay(500);
  Serial.println("Starting System...");

  // Turn off watch-dog timers to avoid interference with server
  disableCore0WDT();
  disableCore1WDT();

  // Start SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("An error occurred while mounting SPIFFS.");
        return;
    }

  // Initialize server for first connection
  initServer();
}

//====================================================================
// MAIN LOOP
//====================================================================
void loop() {
  checkWiFiConnection();

  // Cuando el servidor local está encendido...
  if (server_status) {
    server.handleClient();
  }
}

//====================================================================
// FUNCTION PROTOTYPES
//====================================================================
//Función para enviar el HTML
void SendWebsite() {
  Serial.println("\nSENDING WEB PAGE");
  server.send(200, "text/html", PAGE_MAIN);
}

void SendXML() {
  // Attach XML header
  strcpy(XML, "<?xml version = '1.0'?>\n<Data>\n");

  // Attach connection status
  if (connection_status == 1) {
    strcat(XML, "<C>Connecting...</C>\n");
    XML_flag = 1;
  } else if (connection_status == 2) {
    strcat(XML, "<C>Connected successfully</C>\n");
    XML_flag = 2;
  } else if (connection_status == 3) {
    strcat(XML, "<C>Connection failed</C>\n");
    XML_flag = 3;
  } else {
    strcat(XML, "<C>0</C>\n");
    XML_flag = 0;
  }

  // Attach XML ending
  strcat(XML, "</Data>\n");
  
  // Print new XML
  if (XML_flag != XML_flag_old) {
    Serial.println("\nXML UPDATE SENT");
    Serial.println(XML);
    XML_flag_old = XML_flag;
  }
  server.send(200, "text/xml", XML);
}

void printWifiStatus() {
  Serial.println("\nCONNECTION INFORMATION");

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void WiFiNetwork() {
  // Set connection  counter to 0 and get the network name
  connection_timeOut = 0;
  wifi_network =  server.arg("VALUE");

  // Print WiFi network name
  Serial.println("\nWIFI CREDENTIALS RECEIVED");
  Serial.print("Wifi Network: ");Serial.println(wifi_network);

  // Send something back to client to maintain communication
  server.send(200, "text/plain", "");
}

void WiFiPassword() {
  // Get the WiFi password from server
  wifi_password =  server.arg("VALUE");

  // Print WiFi password
  Serial.print("Wifi Password: ");Serial.println(wifi_password);

  // Try connecting to Internet
  connection_status = 1;
  WiFi.begin(wifi_network, wifi_password);
  Serial.println("Begining WiFi connection");

  // Send something back to client to maintain communication
  server.send(200, "text/plain", "");
}

void closeServer() {
  // Indicate connection successfull and close WebServer and softAP
  WiFi.softAPdisconnect(true);
  server.stop();
  server_status = false;
  Serial.print("\nWEBSERVER AND SOFTAP CLOSED");
}

void checkWiFiConnection() {
  // If ESP32 Disconnected
  if (WiFi.status() != WL_CONNECTED && connection_status == 2) {
    if (connection_status != 1) {
      Serial.println("\nLOST CONNECTION, RECONNECTING...");
      connection_status = 1;
      connection_timeOut = 0;
    }
  }

  // Try to connect
  if (connection_timeOut >= 0 && connection_timeOut <= 40 && connection_status == 1) {
    delay(250);
    connection_timeOut++;
  } 
  
  // If more than 10 seconds have passed...
  else if (connection_timeOut > 40){
    if (connection_status != 3) {
      Serial.println("\nCONNECTION TIMEOUT!");
      wifi_network = "";
      wifi_password = "";
      // Initialize server to allow user to connect and indicate error
      if (server_status == false) {
        initServer();
      }
      connection_status = 3;
    }
  }

  // When WiFi reconnects...
  if (WiFi.status() == WL_CONNECTED && connection_status != 2) {
    // Print connection info
    Actual_IP = WiFi.localIP();
    printWifiStatus();
    connection_status = 2;
    if (wifi_network == "" && wifi_password == "") {
      closeServer();
    }
  }
}

void initServer() {
  delay(2000);
  Serial.println("\nSTARTING WEBSERVER AND SOFTAP");

  // If directly connecting to ESP32, not connected to internet
  WiFi.softAP(AP_SSID, AP_PASS);
  delay(100);
  WiFi.softAPConfig(PageIP, gateway, subnet);
  delay(100);
  Actual_IP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(Actual_IP);

  // Conection to internal functions for server
  server.on("/", SendWebsite);
  server.on("/xml", SendXML);
  server.on("/WiFiNetwork", WiFiNetwork);
  server.on("/WiFiPassword", WiFiPassword);
  server.on("/Connected", closeServer);
  
  // Initializing server
  server.begin();
  server_status = true;
}