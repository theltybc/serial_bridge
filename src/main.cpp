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
void show_indication(struct Indication *ind);
void drop(HardwareSerial serial);

#define UART_SPEED 115200
#define UART_RS 2                //выход для переключения направления передачи
#define UART_RS_TRANSFER_STATE 1 //значение при передаче

#define PIN_SETTING_MODE 16

#define PIN_STATE_BLINK 50

// #define SSID "45 REGION"
// #define PASSHRASE "7529527529"
#define SSID "SSID"
#define PASSHRASE "12345678"

enum Mode {
  m_setting,
  m_sta,
  m_ap
};

struct Indication {
  int client;
  int data;
  timer_t _data_ind;
} ind;

int mode = m_setting;

void setup() {
  UART_PORT.begin(UART_SPEED);
  UART_PORT.setTimeout(100);
  UART_PORT.setRxBufferSize(256);
  debug_port_init();
  pinMode(UART_RS, OUTPUT);
  digitalWrite(UART_RS, !UART_RS_TRANSFER_STATE);

  pinMode(PIN_SETTING_MODE, INPUT_PULLDOWN_16);

  pinMode(PIN_STATE, OUTPUT);

  setting_read();

  if (!setting_available() || digitalRead(PIN_SETTING_MODE)) {
    init_setting_mode();
  } else {
    init_main_mod();
  }
}

void init_setting_mode(void) {
  debugf("SETTING MODE %i %i\n", digitalRead(PIN_SETTING_MODE), setting_get_ap());
  wifi_ap_up(SSID, PASSHRASE, 0);
  web_init();
}

void init_main_mod(void) {
  int ap = setting_get_ap();
  const char *ssid = setting_get_ssid();
  const char *pass = setting_get_pass();

  debugf("ap %i\n", ap);
  debugf("ssid %s\n", ssid);
  debugf("pass %s\n", pass);

  if (ap) {
    debugf("AP MODE\n");
    wifi_ap_up(ssid, pass, SSID_HIDDEN);

    mode = m_ap;
  } else {
    debugf("STA MODE\n");
    wifi_sta_up(ssid, pass);

    const char *host = setting_get_host();
    uint16_t port = setting_get_port();
    set_host_port(host, port);

    mode = m_sta;
  }
}

void loop() {
  static WiFiClient client;
  if (digitalRead(PIN_SETTING_MODE)) {
    init_setting_mode();
    mode = m_setting;
  }
  if (mode == m_setting) {
    web_handle();
  } else {
    if (client.connected()) {
      ind.client = 1;
      transfer(client, UART_PORT);
    } else {
      client.stop();
      drop(UART_PORT);
      if (mode == m_ap) {
        debugf("wifi_get_client\n");
        client = wifi_get_client();
      } else {
        debugf("wifi_create_client\n");
        client = wifi_create_client();
      }
    }
    show_indication(&ind);
  }
}

void drop(HardwareSerial serial) {
  char buffer[250];
  serial.read(buffer, sizeof(buffer));
}

void transfer(WiFiClient client, HardwareSerial serial) {
  uint8_t buffer[256];

  int res = client.read(buffer, sizeof(buffer));
  if (res > 0) {
    digitalWrite(UART_RS, UART_RS_TRANSFER_STATE);
    while (res > 0) {
      ind.data = 1;
      serial.write(buffer, res);
      res = client.read(buffer, sizeof(buffer));
      ind.data = 1;
    }
    digitalWrite(UART_RS, !UART_RS_TRANSFER_STATE);
  }

  res = serial.read((char *)buffer, sizeof(buffer));
  while (res > 0) {
    ind.data = 1;
    client.write(buffer, res);
    res = serial.read((char *)buffer, sizeof(buffer));
  }
}

void show_indication(struct Indication *ind) {
  if (ind->client) {
    if (ind->_data_ind) {
      if (ind->_data_ind < millis()) {
        if (digitalRead(PIN_STATE) && ind->data == 0) {
          ind->_data_ind = 0;
        } else {
          ind->_data_ind = millis() + PIN_STATE_BLINK;
        }
        digitalWrite(PIN_STATE, !digitalRead(PIN_STATE));
        ind->data = 0;
      }
    } else if (ind->data) {
      ind->_data_ind = millis() + PIN_STATE_BLINK;
      digitalWrite(PIN_STATE, 0);
    } else {
      digitalWrite(PIN_STATE, 1);
    }
    ind->client = 0;
  } else {
    digitalWrite(PIN_STATE, 0);
  }
}