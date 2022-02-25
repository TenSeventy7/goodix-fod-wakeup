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

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/fb.h>
#include <linux/input.h>
#include "wakeup.h"

static void wakeup_fod()
{
	if (readfint(BLDEV) > 3 || readfint(PRDEV) > 220) {
		usleep(DELAY);
		return;
	}

	dbg(":: Wake-up the screen.\n");
	send_input(TSDEV, EV_KEY, KEY_WAKEUP, 1);
	send_input(TSDEV, EV_KEY, KEY_WAKEUP, 0);
	send_input(TSDEV, EV_SYN, SYN_REPORT, 0);

	for(;;) {
		if (readfint(BLDEV) <= 5)
			continue;

		dbg(":: Screen on.\n");
		dbg(":: Emulating touches\n");

		system("input touchscreen swipe 443 1972 443 1972 500");

		dbg(":: Done\n");
		return;
	}
}

static void wakeup_aod()
{
	if (readfint(BLDEV) > 3 || readfint(PRDEV) > 220) {
		usleep(DELAY);
		return;
	}

	dbg(":: Trigger ambient display\n");
	system("am broadcast -a com.android.systemui.doze.pulse --user 0");
}

int main()
{
	int fd, rd;
	struct input_event ev;
	size_t evsize = sizeof(struct input_event);

	dbg("SamsungFODWakeup %s\n", APP_VERSION);
	dbg("Tiny service for in-display fingerprint to do tap-to-wake-and-scan on Samsung devices\n");
	dbg("Copyright 2019, Nanda Oktavera\n");
	dbg("Extensions 2021, John Vincent\n");

	if (getuid() != 0){
		printf("Permission denied, please run as root. exiting\n");
		return 1;
	}

	dbg(":: Reading %s\n", EVDEV);
	fd = open(EVDEV, O_RDONLY | O_NONBLOCK);
	while((rd = read(fd, &ev, evsize))) {
		if (rd == -1) {
			usleep(DELAY);
			continue;
		}

		if (unlikely(ev.value == 1)) {
			switch (ev.code) {
				case INP_OFF:
					wakeup_fod();
					break;
				case INP_AOD:
					wakeup_aod();
					break;
				default:
					usleep(DELAY);
					break;
			}
		} else {
			usleep(DELAY);
		}
	}

	dbg(":: Closing %s\n", EVDEV);
	close(fd);
	return 0;
}
