/*
 * File:   main.c
 * Author: JPoirier
 *
 * Created on March 15, 2017, 10:45 AM
 */

#include <pic10f320.h>

#include "globals.h"
#include "pin_defines.h"

void interrupt isr(void)
{
    if (PIE1bits.TMR2IE && PIR1bits.TMR2IF)
    {
        PIR1bits.TMR2IF = 0;              // clear the interrupt
        debounced_switch = (debounced_switch << 1) | (0x01 & pb_pin); // bit shift every interrupt
    }
    if (IOCAFbits.IOCAF0){
        IOCAFbits.IOCAF0 = 0;
        sleep_timer = 0;
    }
    sleep_timer++;
}