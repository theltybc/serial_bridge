#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <string.h>
#include "log.h"
#include "web.h"
#include "setting_store.h"
#include "wifi.h"

void transfer(WiFiClient client);
void init_setting_mode(void);
void init_main_mod(void);

#define UART_PORT Serial
#define UART_SPEED 115200
#define UART_RS 2                //выход для переключения направления передачи
#define UART_RS_TRANSFER_STATE 1 //значение при передаче

#define PIN_SETTING_MODE 16

// #define SSID "45 REGION"
// #define PASSHRASE "7529527529"
#define SSID "SSID"
#define PASSHRASE "PASSHRASE"

WiFiClient (*get_client)(void);

int setting_mode = 1;

void setup() {
  UART_PORT.begin(UART_SPEED);
  UART_PORT.setTimeout(100);
  pinMode(UART_RS, OUTPUT);
  digitalWrite(UART_RS, !UART_RS_TRANSFER_STATE);

  pinMode(PIN_SETTING_MODE, INPUT_PULLUP);

  setting_read();
  if (!setting_available() || digitalRead(PIN_SETTING_MODE) != 1) {
    debug("SETTING MODE\n");
    init_setting_mode();
  } else {
    init_main_mod();
  }
}

void init_setting_mode(void) {
  wifi_ap_up(SSID, PASSHRASE, 0);
  web_init();
}

void init_main_mod(void) {
  int ap = setting_get_ap();
  const char *ssid = setting_get_ssid();
  const char *pass = setting_get_pass();
  debug("ap %i\n", ap);
  debug("ssid %s\n", ssid);
  debug("pass %s\n", pass);
  if (ap) {
    debug("AP MODE\n");
    wifi_ap_up(ssid, pass, 1);
    get_client = wifi_get_client;
  } else {
    debug("STA MODE\n");
    wifi_sta_up(ssid, pass);
    get_client = wifi_create_client;
  }
  setting_mode = 0;
}

void loop() {
  if (setting_mode) {
    web_handle();
  } else {
    WiFiClient client = get_client();
    transfer(client);
  }
}

void transfer(WiFiClient client) {
  client.setNoDelay(true);
  while (client.connected()) {
    while (client.available() > 0) {
      digitalWrite(UART_RS, UART_RS_TRANSFER_STATE);
      UART_PORT.write(client.read());
    }
    digitalWrite(UART_RS, !UART_RS_TRANSFER_STATE);
    while (UART_PORT.available() > 0) {
      client.write(UART_PORT.read());
    }
  }
  client.stop();
}
