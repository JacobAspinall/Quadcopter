/*
 * main.c
 *
 *  Created on: Dec 31, 2017
 *      Author: Jacob Aspinall
 */

#include <stdio.h>
#include "LSM9DS1.h"
#include "RFM69HCW.h"



int main(void)
{

    //LSM9DS1_init();
    RFM69HCW_init();
    while(1){
        int t = read_register(0x00);
        printf("%d\n",t);
        t = write_register(0x00,0xF);
        printf("%d\n",t);
        t = read_register(0x00);
        printf("%d\n",t);
        t = read_register(0x00);
        printf("%d\n",t);
        int i;
        while(1);

    }
    return 0;
}
