/*
 * fs_timer.h
 *
 *  Created on: 18 Sep 2020
 *      Author: linus
 */

#ifndef INC_FS_TIMER_H_
#define INC_FS_TIMER_H_

#include "main.h"
#include "state_est_const.h"


#define MACH_INIT() \
  { \
    .value = MACH_TIMER, \
    .end = 0, \
	.active = 0, \
  }

#define FAIL_SAFE_INIT() \
  { \
    .value = FAIL_SAFE_TIMER, \
    .end = 0, \
	.active = 0, \
  }

#define FAIL_SAFE_MAIN_INIT() \
  { \
    .value = FAIL_SAFE_TIMER_MAIN, \
    .end = 0, \
	.active = 0, \
  }

typedef struct timer_t {

	// Hardware Configuration
	uint32_t value;
	uint32_t end;
	uint8_t active;
} TIMER;

void start_timer(struct timer_t * t, uint32_t * tick);
uint8_t check_timer(struct timer_t * t, uint32_t * tick);


#endif /* INC_FS_TIMER_H_ */
