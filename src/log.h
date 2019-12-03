#pragma once

#ifdef DEBUG_ESP_PORT
#define debug(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
#else
#define debug(...)
#endif

