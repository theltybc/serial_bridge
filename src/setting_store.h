#pragma once

#define SIZE_FIELD 30

struct Setting_wifi {
  int ap;
  char ssid[SIZE_FIELD];
  char pass[SIZE_FIELD];
};

int setting_save(int ap, const char* ssid, const char* pass);
int setting_read(void);

int setting_get_ap(void);
const char* setting_get_ssid(void);
const char* setting_get_pass(void);
