/*
 * manager.h
 *
 *  Created on: 14 Sep 2020
 *      Author: linus
 */

#ifndef INC_MANAGER_H_
#define INC_MANAGER_H_

#include "main.h"

#define BARO 3
#define SHT 100
#define LOGGING 100
#define IMU 0

// BARO TASK
#define BARO_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = 0, \
	.duration = BARO, \
  }

// SHT TASK
#define SHT_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = 0, \
	.duration = SHT, \
  }

// IMU TASK
#define IMU_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = 0, \
	.duration = IMU, \
  }

// LED TASKS
#define RDY_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = 0, \
	.duration = 333, \
  }
#define STAT_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = 0, \
	.duration = 1000, \
  }
#define SAVE_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = 0, \
	.duration = 500, \
  }
#define PRGM_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = 0, \
	.duration = 100, \
  }

typedef struct {
	uint32_t last_call;
	uint32_t interval;
	uint8_t stage;
	uint32_t duration;
} task_t;


extern void schedulerinit(void);
extern void scheduler(void);
extern uint32_t getNextExecution(task_t * task);

#endif /* INC_MANAGER_H_ */
