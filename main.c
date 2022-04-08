/*
 * File:   main.c
 * Author: kgilbert
 *
 * Created on January 10, 2022, 6:51 PM
 */


#include <stdint.h>
#include <xc.h>
#include "custom_pin.h"
#include "mcc_generated_files/mcc.h"
#include "TM1637Display.h"

void main(void) {
    SYSTEM_Initialize();
    
    IO_RA2_SetLow();
    IO_RA5_SetLow();
    
    __delay_ms(1000);
    
    tm1637Pins_t *display = tm1637Init(
        (tPin){_PORTA_RA2_MASK, _PORTA_RA2_POSN, &PORTA},
        (tPin){_PORTA_RA5_MASK, _PORTA_RA5_POSN, &PORTA}
    );
    
    tm1637SetBrightness(display, 3);
    tm1637DisplayDecimal(display, 1234, 1);
    
    while (1) {
        __delay_ms(50);
    }
}
