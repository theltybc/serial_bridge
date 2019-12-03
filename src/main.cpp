#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "log.h"

void wifi_connect_up(void);
void pass(void);

#define UART_PORT Serial
#define UART_SPEED 115200
#define UART_RS 2 //выход для переключения направления передачи

void setup() {
  UART_PORT.begin(UART_SPEED);
  pinMode(UART_RS, OUTPUT);

#ifdef MODE_SERVER
  Serial.println("\n\t\t\tMODE_SERVER");
#else
  Serial.println("\n\t\t\tMODE_CLIENT");
#endif
}

void loop() {
  wifi_connect_up();
  pass();
}

#ifdef MODE_SERVER

#define SSID "SSID"
#define PASSHRASE "PASSHRASE"

#define TCP_PORT 2000
static WiFiServer server(TCP_PORT);
WiFiClient client;

void wifi_connect_up(void) {
  if (server.status() != CLOSED) {
    return;
  }
  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, PASSHRASE);
  server.begin();
  server.setNoDelay(true);
}

void pass(void) {
  while (server.hasClient()) {
    client = server.available();
    while (client.connected()) {
      while (client.available() > 0) {
        Serial.write(client.read());
      }
      while (Serial.available() > 0) {
        client.write(Serial.read());
      }
    }
    client.stop();
  }
}

#else // MODE_SERVER

// #define SSID "45 REGION"
// #define PASSHRASE "7529527529"
#define SSID "SSID"
#define PASSHRASE "PASSHRASE"

#define HOST "192.168.4.1"
#define PORT 2000

void wifi_connect_up(void) {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSHRASE); //Connect to wifi

  while (WiFi.status() != WL_CONNECTED) {
    debug(".");
    delay(500);
  }
}

void pass(void) {
  WiFiClient client;

  if (client.connect(HOST, PORT)) {
    while (client.connected()) {
      while (client.available() > 0) {
        Serial.write(client.read());
      }
      while (Serial.available() > 0) {
        client.write(Serial.read());
      }
    }
    client.stop();
  }
}

#endif // MODE_SERVER
