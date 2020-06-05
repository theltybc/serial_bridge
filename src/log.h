#pragma once

#if !defined(DEBUG_PORT) && defined(DEBUG_BUILD)
#error "Define debug mode bot not defined debug port"
#endif

#if defined(DEBUG_PORT) && defined(DEBUG_BUILD) && !defined(SILENCE_MODE)
#define debugln(...) DEBUG_PORT.println(__VA_ARGS__)
#define debug(...) DEBUG_PORT.print(__VA_ARGS__)
#define debugf(...) DEBUG_PORT.printf(__VA_ARGS__)
#else
#define debugln(...)
#define debug(...)
#define debugf(...)
#endif

#if defined(DEBUG_PORT) && !defined(SILENCE_MODE)
#define logln(...) DEBUG_PORT.println(__VA_ARGS__)
#define log(...) DEBUG_PORT.print(__VA_ARGS__)
#define logf(...) DEBUG_PORT.printf(__VA_ARGS__)
#else
#define logln(...)
#define log(...)
#define logf(...)
#endif

#if defined(DEBUG_PORT) && !defined(SILENCE_MODE)
#define debug_port_init() DEBUG_PORT.begin(115200);
#else
#define debug_port_init()
#endif

#define error(...)    \
  do {                \
    log("ERROR: ");   \
    log(__VA_ARGS__); \
  } while (0);
