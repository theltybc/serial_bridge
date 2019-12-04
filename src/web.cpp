
#include <ESP8266WebServer.h>
#include "web.h"
#include "setting_store.h"
#include "page.h"

static void handler_page_request(void);
static void handler_settings_post(void);

static ESP8266WebServer server(80);

void web_init(void) {
  server.on("/", HTTP_GET, handler_page_request);
  server.on("/wifi", HTTP_POST, handler_settings_post);
  server.begin();
}

static void handler_page_request(void) {
  server.send(200, "text/html", page, sizeof(page));
  server.sendHeader("Connection", "close");
}

static void handler_settings_post(void) {
  int result = 0;
  if (server.hasArg("ssid") && server.hasArg("pass")) {
    result = setting_save(server.hasArg("ap"),
                       server.arg("ssid").c_str(),
                       server.arg("pass").c_str());
  } else {
    result = -1;
  }
  server.send(200, "text/plain", (result == 0 ? "OK" : "FAIL"));
  server.sendHeader("Connection", "close");
}

void web_handle(void) {
  server.handleClient();
}
