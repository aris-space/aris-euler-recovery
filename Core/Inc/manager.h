/*
 * manager.h
 *
 *  Created on: 14 Sep 2020
 *      Author: linus
 */

#ifndef INC_MANAGER_H_
#define INC_MANAGER_H_

#include "main.h"
#include "state_est_config.h"

#define BARO 3
#define SHT 100
#define IMU 0
#define ACC 0
#define ADC_INT 20
#define LOG_INT 1000/STATE_ESTIMATION_FREQUENCY

// interval of state estimation call
#define STATE_EST_INT 1000/STATE_ESTIMATION_FREQUENCY

// BARO TASK
#define BARO_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = MS_TEMPERATURE_REQ, \
	.interval = BARO, \
  }

// SHT TASK
#define SHT_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = 0, \
	.interval = SHT, \
  }

// IMU TASK
#define IMU_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = 0, \
	.interval = IMU, \
  }

// ACCEL TASK
#define ACCEL_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = 0, \
	.interval = ACC, \
  }

// ADC TASK
#define ADC_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = 0, \
	.interval = ADC_INT, \
  }

// LOGGING TASK
#define LOG_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = 0, \
	.interval = LOG_INT, \
  }

// STATE ESTIMATION TASK
#define STATE_EST_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = 0, \
	.interval = STATE_EST_INT, \
  }

// LED TASKS
#define RDY_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = 0, \
	.interval = 333, \
  }
#define STAT_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = 0, \
	.interval = 1000, \
  }
#define SAVE_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = 0, \
	.interval = 500, \
  }
#define PRGM_TASK_INIT() \
  { \
	.last_call = 0, \
	.stage = 0, \
	.interval = 100, \
  }

enum event_t {
  NOE = 0,
  HAWKS = 1,
  TENDER = 2,
  TENDER_DISABLE = 3,
};

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
