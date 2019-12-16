#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <string.h>
#include "log.h"
#include "web.h"
#include "setting_store.h"
#include "wifi.h"

void transfer(WiFiClient client, HardwareSerial serial);
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
#define PASSHRASE "12345678"

enum Mode {
  setting,
  sta,
  ap
};
int mode = setting;

void setup() {
  UART_PORT.begin(UART_SPEED);
  UART_PORT.setTimeout(100);
  pinMode(UART_RS, OUTPUT);
  digitalWrite(UART_RS, !UART_RS_TRANSFER_STATE);

  pinMode(PIN_SETTING_MODE, INPUT_PULLDOWN_16);

  setting_read();

  if (!setting_available() || digitalRead(PIN_SETTING_MODE)) {
    debug("SETTING MODE %i %i\n", digitalRead(PIN_SETTING_MODE), setting_get_ap());
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
    wifi_ap_up(ssid, pass, SSID_HIDDEN);

    mode = ap;
  } else {
    debug("STA MODE\n");
    wifi_sta_up(ssid, pass);

    const char *host = setting_get_host();
    uint16_t port = setting_get_port();
    set_host_port(host, port);

    mode = sta;
  }
}

void loop() {
  if (mode == setting) {
    web_handle();
  } else {
    WiFiClient client;
    if (mode == sta){
      client = wifi_create_client();
    }else {
      client = wifi_get_client();
    }

    transfer(client, UART_PORT);
  }
}

void transfer(WiFiClient client, HardwareSerial serial) {
  client.setNoDelay(true);
  while (client.connected()) {
    while (client.available() > 0) {
      digitalWrite(UART_RS, UART_RS_TRANSFER_STATE);
      serial.write(client.read());
    }
    digitalWrite(UART_RS, !UART_RS_TRANSFER_STATE);
    while (serial.available() > 0) {
      client.write(serial.read());
    }
  }
  client.stop();
}
