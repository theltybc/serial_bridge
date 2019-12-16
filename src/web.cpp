
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
  if (server.hasArg("ssid") && server.hasArg("pass") && server.hasArg("mode") ) {
    result = setting_save(server.arg("mode") == "1",
                       server.arg("ssid").c_str(),
                       server.arg("pass").c_str(),
                       server.hasArg("host") ? server.arg("host").c_str() : "",
                       server.hasArg("port") ? server.arg("port").toInt() : 0
                       );
  } else {
    result = -1;
  }
  server.send(200, "text/plain", (result == 0 ? "OK" : "FAIL"));
  server.sendHeader("Connection", "close");
}

void web_handle(void) {
  server.handleClient();
}
