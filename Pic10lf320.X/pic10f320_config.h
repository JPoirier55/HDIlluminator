/* 
 * File:   pi10lf320_config.h
 * Author: JPoirier
 *
 * Created on March 8, 2017, 2:20 PM
 */

#ifndef PI10LF320_CONFIG_H
#define	PI10LF320_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

// PIC10F320 Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = INTOSC        // Oscillator Selection bits (EC: CLKIN function enabled)
#pragma config BOREN = NSLEEP   // Brown-out Reset Enable (Brown-out Reset enabled while running and disabled in Sleep)
#pragma config WDTE = NSLEEP    // Watchdog Timer Enable (WDT enabled while running and disabled in Sleep)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)
#pragma config LPBOR = ON       // Brown-out Reset Selection bits (BOR enabled)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#ifdef	__cplusplus
}
#endif

#endif	/* PIC10LF320_CONFIG_H */