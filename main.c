/*
 * File:   main.c
 * Author: JPoirier
 *
 * Created on March 8, 2017, 2:11 PM
 */


#include <xc.h>
#include <stdio.h>
#include <stdbool.h>

#include "pic10f320_config.h"

#define pwm_pin  PORTAbits.RA0
#define pb_pin1 PORTAbits.RA1

#define CONFIG_AS_ANALOG        1    // ANSEL bit value to set pin as analog pin
#define CONFIG_AS_DIGITAL       0    // ANSEL bit value to set pin as digital pin
#define INPUT_PIN_CNTL          1    //Tristate(TRIS) control value for input pin
#define OUTPUT_PIN_CNTL         0    //Tristate(TRIS) control value for output pin

#define PWM_PIN_DIRECTION TRISAbits.TRISA0 
#define PB_PIN_DIRECTION1 TRISAbits.TRISA1

#define BUTTON_DEADPERIOD 10
#define OSC_FREQ_BITS 0x01
/*
 * Duty cycle equation:
 * ratio = (PWM1DCH:PWM1DCL)/4(PR2+1)
 * 
 * PWM period equation:
 * [PR2+1]*4*Tosc*(TMR2 prescaler)
 * 
 * with Tosc = 1/Fosc = 1/250k
 * 
 */
#define TIMER_PRESCALER 0x03 // prescale of 64
#define PERIOD_SCALER 0x65 // PWM period = .1045
#define BRIGHT 0x0F //duty cycle of 14.7%
#define BRIGHTER 0x1F // duty cycle of 30.4%
#define BRIGHTEST 0x3F // duty cycle of 61.7%
#define SLEEP_TIMER 10 // Go to sleep after 10 minutes

unsigned int  four_msec_cnt;
unsigned char tenth_sec_cnt;
unsigned char sec_cnt;
unsigned char min_cnt;
unsigned char button_deadperiod;
unsigned char debounced_switch;
unsigned char sleep_count;
bit button_released;
bit light_on;


void main(void) {
    
    ANSELAbits.ANSA0 = CONFIG_AS_DIGITAL;
    ANSELAbits.ANSA1 = CONFIG_AS_DIGITAL;
    
    PWM_PIN_DIRECTION = OUTPUT_PIN_CNTL;
    PB_PIN_DIRECTION1 = OUTPUT_PIN_CNTL;
    
    pwm_pin = 1;
    OSCCONbits.IRCF = OSC_FREQ_BITS; //set clock to 250kHz
    PR2 = PERIOD_SCALER; //PWM period value
    
    PWM1DCH &= 0x00; //clear
    PWM1DCL &= 0x00; //clear

    PIR1bits.TMR2IF = 0; //clear
    T2CONbits.T2CKPS = TIMER_PRESCALER; // pre-scale of 64
    T2CONbits.TMR2ON = 1; //set, enable timer2
    
    PWM1CONbits.PWM1EN = 1;
    PWM1CONbits.PWM1OE = 1;
    
    tenth_sec_cnt = 25;     // 25 x 4 msecs in a 0.1 seconds
    sec_cnt = 10;           // 10 x 0.1 seconds in a second
    min_cnt = 60;
    
    button_deadperiod = 0;
    
    debounced_switch = 0xff; // start with it loaded with switching being open
    button_released = 1;
    sleep_count = 0;
    
    
    while(1){
        //  Code for every 100 msecs
        if (--tenth_sec_cnt == 0)
        {
            tenth_sec_cnt = 25;
            CLRWDT(); // kick the dog every 100 milliseconds

            // check if we have set the deadperiod check to count
            if (button_deadperiod != 0)
                    button_deadperiod--;

            // Code for every second
            if (--sec_cnt == 0)
            {
                sec_cnt = 10;
                // Code for every minute
                if (--min_cnt == 0)
                {
                    min_cnt = 60;
                }
            }
        }
        debounced_switch = (debounced_switch << 1) | (0x01 & pb_pin1);
        
        if(light_on){
            if(button_deadperiod == 0){
                if((debounced_switch == 0x0) && button_released){
                    if(PWM1DCH == BRIGHT){
                        PWM1DCH = BRIGHTER;     //turn on brighter
                    }else if(PWM1DCH == BRIGHTER){
                        PWM1DCH = BRIGHTEST;    //turn on brightest
                    }else{
                        light_on = false;
                    }
                    button_deadperiod = BUTTON_DEADPERIOD;
                    button_released = false;
                    
                    sleep_count = 0;            //reset sleep counter
                }else if(debounced_switch == 0xFF){
                    button_released = true;
                }
            }
        }else{
            if(button_deadperiod == 0){
                if((debounced_switch == 0x0) && button_released){
                    PWM1DCH = BRIGHT;           //turn on light, starting with bright
                    light_on = true;
                    button_deadperiod = BUTTON_DEADPERIOD;
                    button_released = false;
                   
                    sleep_count = 0;            //reset sleep counter
                }else if(debounced_switch == 0xFF){
                    button_released = true;
                    SLEEP();
                    
                    PWM1DCH = BRIGHT;
                }
            }
        }
    } 
    return;
}
