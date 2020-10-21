/*
 * fs_timer.c
 *
 *  Created on: 18 Sep 2020
 *      Author: linus
 */

#include "fs_timer.h"

void start_timer(struct timer_t * t, uint32_t * tick){
	if (t->active == 0){
		t->end = *tick + t->value;
		t->active = 1;
	}
}

uint8_t check_timer(struct timer_t * t, uint32_t * tick){
	if ((*tick > t->end) && (t->active == 1)) {
		t->active = 0;
		return 1;
	}
	return 0;
}
