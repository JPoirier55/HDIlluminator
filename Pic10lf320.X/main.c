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
#include "globals.h"
#include "pin_defines.h"

#define CONFIG_AS_ANALOG        1    // ANSEL bit value to set pin as analog pin
#define CONFIG_AS_DIGITAL       0    // ANSEL bit value to set pin as digital pin
#define INPUT_PIN_CNTL          1    //Tristate(TRIS) control value for input pin
#define OUTPUT_PIN_CNTL         0    //Tristate(TRIS) control value for output pin

#define OSC_FREQ_BITS 0x01 // 250k
/*
 * Duty cycle equation:
 * ratio = (PWM1DCH:PWM1DCL)/4(PR2+1)
 * 
 * PWM period equation:
 * [PR2+1]*4*Tosc*(TMR2 prescaler)
 * 
 * with Tosc = 1/Fosc = 1/250k
 * 
 * ISR called every 4 ms 
 * = PR2 * Fosc *  4xclk
 * = 250 * 1/250kHz * 4
 * 
 */
#define TIMER_PRESCALER 0x03 // prescale of 64
#define PERIOD_SCALER 0xF9 // 249-1, gives period of 
#define BRIGHT 0x0F // duty cycle of 6%
#define BRIGHTER 0x1F // duty cycle of 12.4%
#define BRIGHTEST 0x3F // duty cycle of 25.%

bit button_released;
bit light_on;

void setup(){
    // Configure all analog select pins as digital
    ANSELAbits.ANSA0 = CONFIG_AS_DIGITAL;
    ANSELAbits.ANSA1 = CONFIG_AS_DIGITAL;
    ANSELAbits.ANSA1 = CONFIG_AS_DIGITAL;
    
    PB_PIN_DIRECTION = INPUT_PIN_CNTL; // Set push button to input
    
    PWM_PIN_DIRECTION = OUTPUT_PIN_CNTL; // Set pwm to output
    ISET_PIN_DIRECTION = OUTPUT_PIN_CNTL; // Set iset to output if used
   
    iset_cntl = 0; // Possibly used for led control, driven 0 
    pwm_pin = 1; // Set pwm pin on
    
    OSCCONbits.IRCF = OSC_FREQ_BITS; //set clock to 2MHz
    
    PR2 = PERIOD_SCALER; //PWM period value
    
    PWM2DCH &= 0x00; //clear
    PWM2DCL &= 0x00; //clear

    PIR1bits.TMR2IF = 0; //clear
    T2CONbits.T2CKPS = TIMER_PRESCALER; // pre-scale of 64
    T2CONbits.TMR2ON = 1; //set, enable timer2
    
    PWM2CONbits.PWM2EN = 1; // enable pwm2 on RA1
    PWM2CONbits.PWM2OE = 1; // enable pwm2 pin
    
    PIE1bits.TMR2IE = 1; // enable timer2 interrupt
    INTCONbits.PEIE = 1; // peripheral interrupt enable
    INTCONbits.GIE = 1; // global interrupt enable
    
    debounced_switch = 0xFF; // start with it loaded with switching being open
    button_released = true;
    light_on = false;
    
}

void main(void) {
    
    setup();
    
    while(1){
        CLRWDT(); // kick the dog every cycle ~20-25 instr, ~16us  
        if(light_on){
            if((debounced_switch == 0x00) && button_released){
                if(PWM2DCH == BRIGHT){
                    PWM2DCH = BRIGHTER;     //turn on brighter
                }else if(PWM2DCH == BRIGHTER){
                    PWM2DCH = BRIGHTEST;    //turn on brightest
                }else{
                    PWM2DCH = 0x00;
                    light_on = false;
                }
                button_released = false;
            }else if(debounced_switch == 0xFF){
                button_released = true;
            }
        }else{
            if((debounced_switch == 0x00) && button_released){
                PWM2DCH = BRIGHT;           //turn on light, starting with bright
                light_on = true;
                button_released = false;
            }else if(debounced_switch == 0xFF){
                button_released = true;
                SLEEP(); // sleep if no lights on and no button has been pushed after debouncing
            }
        }
    } 
    return;
}