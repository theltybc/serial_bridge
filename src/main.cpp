#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "log.h"

void wifi_connect_up(void);
void pass(void);

#define UART_PORT Serial
#define UART_SPEED 115200
#define UART_RS 2 //выход для переключения направления передачи

// #define SSID "45 REGION"
// #define PASSHRASE "7529527529"
#define SSID "SSID"
#define PASSHRASE "PASSHRASE"

#define HOST "192.168.4.1"
#define TCP_PORT 2000

void setup() {
  UART_PORT.begin(UART_SPEED);
  UART_PORT.setTimeout(100);
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

static WiFiServer server(TCP_PORT);

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
    WiFiClient client = server.available();
    while (client.connected()) {
      while (client.available() > 0) {
        UART_PORT.write(client.read());
      }
      while (UART_PORT.available() > 0) {
        client.write(UART_PORT.read());
      }
    }
    client.stop();
  }
}

#else // MODE_SERVER

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
  if (client.connect(HOST, TCP_PORT)) {
    client.setNoDelay(true);
    while (client.connected()) {
      while (client.available() > 0) {
        UART_PORT.write(client.read());
      }
      while (UART_PORT.available() > 0) {
        client.write(UART_PORT.read());
      }
    }
    client.stop();
  }
}

#endif // MODE_SERVER
