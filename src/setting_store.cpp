#include <EEPROM.h>
#include <string.h>

#include "setting_store.h"

struct Setting_wifi setting_wifi;

#define ADDRESS 0

int setting_save(int ap, const char *user, const char *pass) {
  int result = 0;
  size_t user_l = strlen(user) + 1;
  size_t pass_l = strlen(pass) + 1;
  if (user_l > SIZE_FIELD || pass_l > SIZE_FIELD) {
    result = -1;
    return result;
  }

  if (setting_wifi.ap == ap &&
      strncmp(setting_wifi.ssid, user, SIZE_FIELD) == 0 &&
      strncmp(setting_wifi.pass, pass, SIZE_FIELD) == 0) {
    return result;
  }

  setting_wifi.ap = ap;
  strncpy(setting_wifi.ssid, user, user_l);
  strncpy(setting_wifi.pass, pass, pass_l);

  EEPROM.begin(sizeof(Setting_wifi));
  EEPROM.put(ADDRESS, setting_wifi);
  result = !EEPROM.commit();
  EEPROM.end();
  return result;
}

int setting_read(void) {
  EEPROM.begin(sizeof(Setting_wifi));
  EEPROM.get(ADDRESS, setting_wifi);
  EEPROM.end();
  return 0;
}

int setting_get_ap(void) {
  return setting_wifi.ap;
}

const char *setting_get_ssid(void) {
  return setting_wifi.ssid;
}

const char *setting_get_pass(void) {
  return setting_wifi.pass;
}