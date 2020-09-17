/*
 * SD.h
 *
 *  Created on: Sep 14, 2020
 *      Author: linus
 */

#ifndef INC_SD_H_
#define INC_SD_H_

#include "main.h"


uint16_t bufsize (char * buf);
void bufclear(char * buffer);
uint8_t find_file(void);
uint8_t init_sd(uint16_t * file_count, uint16_t * log_count);
uint8_t init_file(char * FILE_NAME, char * LOG_NAME);
void read_from_SD(char * FILE_NAME, char * buffer);
void write_to_SD(char * FILE_NAME, char * buffer);
void log_to_SD(char * FILE_NAME, char * buffer);






#endif /* INC_SD_H_ */
