/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Samsung FOD Wakeup
 * Tiny service for in-display fingerprint to do tap-to-wake-and-scan
 * for Samsung devices with a compatible kernel.
 *
 * Copyright (c) 2019 Nanda Oktavera
 * Extensions 2021 John Vincent
 * Released under the terms of 3-clause BSD License
 *
 */

// #define DEBUG

#ifndef SECTS_WAKEUP_H
#define SECTS_WAKEUP_H

#ifdef __GNUC__
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#else
#define likely(x)       (x)
#define unlikely(x)     (x)
#endif

#define EVDEV   "/dev/input/event8" // sec_touchpad input event path
#define TSDEV   "/dev/input/event4" // sec_touchscreen input event path
#define PRDEV   "/sys/devices/virtual/sensors/proximity_sensor/raw_data" // proximity sensor raw data
#define BLDEV   "/sys/class/backlight/panel/actual_brightness"
#define INP_OFF 455 // when area is touched while screen is off for longpress
#define INP_AOD 454 // when area is touched while screen is off for singletap
#define DELAY   10000

#ifdef DEBUG
#define dbg printf
#else
#define dbg(fmt,...) do {} while (0)
#endif

int send_input(char *input, uint16_t type, uint16_t code, uint16_t value);
int readfint(char *file);

#define APP_VERSION "1.4"
#endif
