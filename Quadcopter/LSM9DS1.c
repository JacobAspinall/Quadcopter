/*
 * LSM9DS1.c
 *
 *  Created on: Dec 31, 2017
 *      Author: Jacob Aspinall
 */
#include "LSM9DS1.h"

//Device slave addresses
#define ACCELGYRO       0x6B
#define MAG             0x1E

//Accelerometer/Gyro register addresses
#define CTRL_REG1_G     0x10
#define OUT_TEMP        0x15
#define WHO_AM_I        0x0F
#define OUT_X_G         0x18
#define OUT_Y_G         0x1A
#define OUT_Z_G         0x1C
#define OUT_X_XL        0x28
#define OUT_Y_XL        0x2A
#define OUT_Z_XL        0x2C

//Magnetometer register addresses
#define CTRL_REG3_M     0x22
#define OUT_X_M         0x28
#define OUT_Y_M         0x2A
#define OUT_Z_M         0x2C

static void write_register(uint8_t device_address, uint8_t sub_address, uint8_t num_bytes, uint8_t write_value);

void LSM9DS1_init(){

    //Enable clock on Port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    //Set PB2,PB3 to digital
    GPIO_PORTB_DEN_R |= 0xC;//1100
    //Enable alternate functions for PB2,Pb3
    GPIO_PORTB_AFSEL_R |= 0xC;
    //Set Alternate functions to I2C
    GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB2_I2C0SCL | GPIO_PCTL_PB3_I2C0SDA;
    //Set PB3 to open drain
    GPIO_PORTB_ODR_R |= 0x8;

    //Enable clock for I2C0
    SYSCTL_RCGCI2C_R |= SYSCTL_RCGCI2C_R0;
    //Set to master mode
    I2C0_MCR_R |= I2C_MCR_MFE;
    //Set speed to 100kHz
    I2C0_MTPR_R = 0x7;

    //Power up gyro and magnetometer
    write_register(ACCELGYRO, CTRL_REG1_G, 1, 0b10000000);
    write_register(MAG, CTRL_REG3_M, 1, 0b00000000);

}

uint8_t i2c_wait_done(){
    while(I2C0_MCS_R & 1);
    return (uint8_t)I2C0_MCS_R & 0xE;
}

static void write_register(uint8_t device_address, uint8_t sub_address, uint8_t num_bytes, uint8_t write_value){
    int8_t i;
    //Set slave address with write
    I2C0_MSA_R = (device_address << 1);
    //Set address for register
    I2C0_MDR_R = sub_address;
    //Transmit first byte
    I2C0_MCS_R = I2C_MCS_START | I2C_MCS_RUN;
    i2c_wait_done();
    I2C0_MDR_R = write_value;
    I2C0_MCS_R = I2C_MCS_RUN;
    i2c_wait_done();
    for(i = 1; i < num_bytes; i++){
        I2C0_MCS_R = I2C_MCS_RUN;
        I2C0_MDR_R = write_value >> 8 && 0x00FF;
        i2c_wait_done();
    }
    I2C0_MCS_R = I2C_MCS_STOP | I2C_MCS_RUN;
}

static int16_t read_register(uint8_t device_address, uint8_t sub_address, uint8_t num_bytes){
    int16_t register_value;
    int8_t i;
    //Set slave address with write
    I2C0_MSA_R = (device_address << 1);
    //Set address for register
    I2C0_MDR_R = sub_address;
    //Transmit first byte
    I2C0_MCS_R = I2C_MCS_START | I2C_MCS_RUN;
    i2c_wait_done();
    //Set slave address with read
    I2C0_MSA_R = (device_address << 1) | 1;
    I2C0_MCS_R = I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_ACK;
    i2c_wait_done();
    register_value = I2C0_MDR_R;
    for(i = 1; i < num_bytes; i++){
        I2C0_MCS_R = I2C_MCS_RUN | I2C_MCS_ACK;
        i2c_wait_done();
        register_value |= I2C0_MDR_R << i*8;
    }
    I2C0_MCS_R = I2C_MCS_STOP | I2C_MCS_RUN;
    return register_value;
}

uint8_t get_gyro_control(){
    return read_register(ACCELGYRO, CTRL_REG1_G,1);
}


int16_t get_temperature(){
    int16_t temperature_raw = read_register(ACCELGYRO, OUT_TEMP,2);
    return ((temperature_raw / 16.0) + 25) * 9.0/5.0 + 32;
}

uint8_t get_gyro_accel_address(){
    return read_register(ACCELGYRO, WHO_AM_I,1);
}

int16_t get_x_gyro(){
    return read_register(ACCELGYRO, OUT_X_G, 2);
}

int16_t get_y_gyro(){
    return read_register(ACCELGYRO, OUT_Y_G, 2);
}

int16_t get_z_gyro(){
    return read_register(ACCELGYRO, OUT_Z_G, 2);
}

int16_t get_x_accel(){
    return read_register(ACCELGYRO, OUT_X_XL, 2);
}

int16_t get_y_accel(){
    return read_register(ACCELGYRO, OUT_Y_XL, 2);
}

int16_t get_z_accel(){
    return read_register(ACCELGYRO, OUT_Z_XL, 2);
}

int16_t get_x_mag(){
    return read_register(MAG, OUT_X_M, 2);
}

int16_t get_y_mag(){
    return read_register(MAG, OUT_Y_M, 2);
}

int16_t get_z_mag(){
    return read_register(MAG, OUT_Z_M, 2);
}




