
#include <xc.h>
#include "custom_pin.h"
#include "TM1637Display.h"

#define _XTAL_FREQ (4000000)

void _tm1637Start(tm1637Pins_t *display);
void _tm1637Stop(tm1637Pins_t *display);
void _tm1637ReadResult(tm1637Pins_t *display);
void _tm1637WriteByte(tm1637Pins_t *display, unsigned char b);

const char segmentMap[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, // 0-7
    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, // 8-9, A-F
    0x00
};


tm1637Pins_t * tm1637Init(tPin DIO, tPin CLK) {
    static tm1637Pins_t display;
    display.DIO = DIO;
    display.CLK = CLK;
    
    tm1637SetBrightness(&display, 8);
    
    return &display;
}

void tm1637DisplayDecimal(tm1637Pins_t *display, int v, int displaySeparator) {
    unsigned char digitArr[4];
    for (int i = 0; i < 4; ++i) {
        digitArr[i] = segmentMap[v % 10];
        if (i == 2 && displaySeparator) {
            digitArr[i] |= 1 << 7;
        }
        v /= 10;
    }

    _tm1637Start(display);
    _tm1637WriteByte(display, 0x40);
    _tm1637ReadResult(display);
    _tm1637Stop(display);

    _tm1637Start(display);
    _tm1637WriteByte(display, 0xc0);
    _tm1637ReadResult(display);

    for (int i = 0; i < 4; ++i) {
        _tm1637WriteByte(display, digitArr[3 - i]);
        _tm1637ReadResult(display);
    }

    _tm1637Stop(display);
}

// Valid brightness values: 0 - 8.
// 0 = display off.
void tm1637SetBrightness(tm1637Pins_t *display, char brightness) {
    // Brightness command:
    // 1000 0XXX = display off
    // 1000 1BBB = display on, brightness 0-7
    // X = don't care
    // B = brightness
    _tm1637Start(display);
    _tm1637WriteByte(display, 0x87 + brightness);
    _tm1637ReadResult(display);
    _tm1637Stop(display);
}

void _tm1637Start(tm1637Pins_t *display) {
    write_reg(display->CLK, 1);
    write_reg(display->DIO, 1);
    __delay_us(2);
    write_reg(display->DIO, 0);
}

void _tm1637Stop(tm1637Pins_t *display) {
    write_reg(display->CLK, 0);
    __delay_us(2);
    write_reg(display->DIO, 0);
    __delay_us(2);
    write_reg(display->CLK, 1);
    __delay_us(2);
    write_reg(display->DIO, 1);
}

void _tm1637ReadResult(tm1637Pins_t *display) {
    write_reg(display->CLK, 0);
    __delay_us(5);
    // while (dio); // We're cheating here and not actually reading back the response.
    write_reg(display->CLK, 1);
    __delay_us(2);
    write_reg(display->CLK, 0);
}

void _tm1637WriteByte(tm1637Pins_t *display, unsigned char b) {
    for (int i = 0; i < 8; ++i) {
        write_reg(display->CLK, 0);
        if (b & 0x01) {
            write_reg(display->DIO, 1);
        }
        else {
            write_reg(display->DIO, 0);
        }
        __delay_us(3);
        b >>= 1;
        write_reg(display->CLK, 1);
        __delay_us(3);
    }
}
