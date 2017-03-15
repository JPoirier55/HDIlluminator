/* 
 * File:   pin_defines.h
 * Author: JPoirier
 *
 * Created on March 15, 2017, 10:56 AM
 */

#ifndef PIN_DEFINES_H
#define	PIN_DEFINES_H

#ifdef	__cplusplus
extern "C" {
#endif

#define pb_pin PORTAbits.RA0
#define pwm_pin PORTAbits.RA1
#define iset_cntl PORTAbits.RA2
    
#define PB_PIN_DIRECTION TRISAbits.TRISA0 
#define PWM_PIN_DIRECTION TRISAbits.TRISA1
#define ISET_PIN_DIRECTION TRISAbits.TRISA2


#ifdef	__cplusplus
}
#endif

#endif	/* PIN_DEFINES_H */

