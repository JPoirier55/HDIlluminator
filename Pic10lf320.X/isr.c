
#include <pic10lf320.h>





#include "globals.h"
#include "pin_defines.h"

void interrupt isr(void)
{


    // TIMER 2 ISR ==============================================================

    if (PIE1bits.TMR2IE && PIR1bits.TMR2IF)
    {
        PIR1bits.TMR2IF = 0;              // clear the interrupt
        debounced_switch = (debounced_switch << 1) | (0x01 & pb_pin);
        // assume we are quick enough to increment counter in main
    }
    // END TIMER 2 ISR  ==========================================================

}