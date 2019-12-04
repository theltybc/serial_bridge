#pragma once


#define HOST "192.168.4.1"
#define TCP_PORT 2000

#define TIMEOUT_CONNECT 10000ul

void wifi_ap_up(const char *ssid, const char *pass, int ssid_hidden);
int wifi_ap_has_connect(void);

void wifi_sta_up(const char *ssid, const char *pass);
int wifi_sta_has_connect(void);


WiFiClient wifi_get_client(void);
WiFiClient wifi_create_client(void);
