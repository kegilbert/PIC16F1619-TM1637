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


#define START_TIME_S 12
#define TIME_S_TO_DISPLAY_FORMAT(time) ((time / 60) * 100) + (time % 60)

void RC4_ISR(void);
    
static tm1637Pins_t *display;
static volatile uint32_t time = START_TIME_S;

void RC4_ISR(void) {
    IO_RA1_SetLow();
    time = START_TIME_S;
    tm1637SetBrightness(display, 3);
    tm1637DisplayDecimal(display, TIME_S_TO_DISPLAY_FORMAT(time), 1);
}

void main(void) {
    SYSTEM_Initialize();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
    IO_RA1_SetLow();
    IO_RA2_SetLow();
    IO_RA5_SetLow();
    
    display = tm1637Init(
        (tPin){_PORTA_RA2_MASK, _PORTA_RA2_POSN, &PORTA},
        (tPin){_PORTA_RA5_MASK, _PORTA_RA5_POSN, &PORTA}
    );
    
    tm1637SetBrightness(display, 3);
    tm1637DisplayDecimal(display, TIME_S_TO_DISPLAY_FORMAT(time), 1);
 
    IOCCF4_SetInterruptHandler(RC4_ISR);
    
    while (1) {
        __delay_ms(1000);
        time -= 1;
        tm1637DisplayDecimal(display, TIME_S_TO_DISPLAY_FORMAT(time), 1);
        while(time == 0) {
            IO_RA1_SetHigh();
            tm1637SetBrightness(display, 0);
            __delay_ms(500);
            tm1637SetBrightness(display, 3);
            __delay_ms(500);
        }
    }
}
