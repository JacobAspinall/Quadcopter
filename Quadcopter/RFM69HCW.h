/*
 * RFM69HCW.h
 *
 *  Created on: Jan 2, 2018
 *      Author: Jacob Aspinall
 */

#ifndef RFM69HCW_H_
#define RFM69HCW_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>

void RFM69HCW_init();
uint8_t write_register(uint8_t address, uint8_t data);
uint8_t read_register(uint8_t address);

#endif /* RFM69HCW_H_ */
