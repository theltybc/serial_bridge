
#include <ESP8266WiFi.h>
#include "wifi.h"
#include "log.h"

static WiFiServer server(TCP_PORT);

static const char *host;
static uint16_t port;

void wifi_ap_up(const char *ssid, const char *pass, int ssid_hidden) {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass, 0, ssid_hidden);
  server.begin();
  server.setNoDelay(true);
}

int wifi_ap_has_connect(void) {
  return server.hasClient();
}

void wifi_sta_up(const char *ssid, const char *pass) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  WiFi.setAutoReconnect(1);
}
int wifi_sta_has_connect(void) {
  return WiFi.status() != WL_CONNECTED;
}

WiFiClient wifi_get_client(void) {
  return server.available();
}

void set_host_port(const char *h, uint16_t p) {
  if (h == NULL) {
    h = HOST;
  }
  if (p == 0) {
    p = TCP_PORT;
  }

  debug("set_host_port: %s:%u", host, port);
  host = h;
  port = p;
}

WiFiClient wifi_create_client(void) {
  WiFiClient client;
  client.connect(host, port);
  return client;
}