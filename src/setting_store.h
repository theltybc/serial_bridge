#pragma once

#include <stdint.h>


#define SIZE_FIELD 30

struct Setting_wifi {
  int ap;
  char ssid[SIZE_FIELD];
  char pass[SIZE_FIELD];
  char host[SIZE_FIELD];
  uint16_t port;
};

int setting_save(int ap, const char* ssid, const char* pass, const char *host, int16_t port);
int setting_read(void);

int setting_get_ap(void);
const char* setting_get_ssid(void);
const char* setting_get_pass(void);
const char* setting_get_host(void);
uint16_t setting_get_port(void);
int setting_available(void);
