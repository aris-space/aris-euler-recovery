/*
 * H3L.h
 *
 *  Created on: 17 Jun 2020
 *      Author: linus
 */

#ifndef H3L_H_
#define H3L_H_

int H3L_INIT();
void H3L_read(double* buffer);
void H3L_read_raw(uint8_t* buf);
void H3L_convert(uint8_t* buf, double* out);
void H3L_write(uint8_t REG, uint8_t val);



#endif /* H3L_H_ */
