/*
 * LSM9DS1.h
 *
 *  Created on: Dec 31, 2017
 *      Author: jacob_000
 */

#ifndef LSM9DS1_H_
#define LSM9DS1_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>

void LSM9DS1_init();
int16_t get_x_gyro();
int16_t get_y_gyro();
int16_t get_z_gyro();
int16_t get_x_accel();
int16_t get_y_accel();
int16_t get_z_accel();
int16_t get_x_mag();
int16_t get_y_mag();
int16_t get_z_mag();
int16_t get_temperature();



#endif /* LSM9DS1_H_ */
