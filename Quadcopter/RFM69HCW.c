/*
 * RFM69HCW.c
 *
 *  Created on: Jan 2, 2018
 *      Author: Jacob Aspinall
 */

#include "RFM69HCW.h"
#include <stdint.h>


void RFM69HCW_init(){
    //Enable clock on Port A
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;
    //Enable clock to SSI0
    SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R0;
    //Set PA2,3,4,5 to digital
    GPIO_PORTA_DEN_R |= 0xBC;//10111100
    //Enable alternate functions for PA2,,4,5
    GPIO_PORTA_AFSEL_R |= 0x34;//00110100
    //Set alternate functions to SSI
    GPIO_PORTA_PCTL_R = GPIO_PCTL_PA2_SSI0CLK | GPIO_PCTL_PA4_SSI0RX | GPIO_PCTL_PA5_SSI0TX;
    //Set PA3 as output
    GPIO_PORTA_DIR_R |= 0x8;
    //GPIO_PORTA_DATA_R |= 0x8;
    GPIO_PORTA_DIR_R |= (0x80);
    GPIO_PORTA_DATA_R &= ~(0x80);


    //Disable SSI0 to make configuration changes and set to master mode
    SSI0_CR1_R &= ~(0x6);
    //SPI, 8 data bits
    SSI0_CR0_R |= 0x007;
    //clock prescale divisor of 16
    SSI0_CPSR_R = 16;
    //re-enable SSI0
    SSI0_CR1_R |= 0x2;
}

// write = 1, read = 0
static uint8_t access_register(uint8_t address, uint8_t mode, uint8_t data){

    GPIO_PORTA_DATA_R &= ~(0x8);
    SSI0_DR_R = ((mode << 7) | address);
    while(SSI0_SR_R & 0x10);
    uint32_t contents = SSI0_DR_R;
    SSI0_DR_R = data;
    while(SSI0_SR_R & 0x10);
    contents = SSI0_DR_R;
    GPIO_PORTA_DATA_R |= 0x8;

    return contents;
}

 uint8_t write_register(uint8_t address, uint8_t data){
    return access_register(address, 1, data);
}

 uint8_t read_register(uint8_t address){
    return access_register(address, 0, 0);
}
