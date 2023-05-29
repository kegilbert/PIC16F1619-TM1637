/*
 * File:   TM1637Display.c
 * Author: kgilbert
 *
 * Created on January 14, 2022, 3:37 PM
 */

#include <stdint.h>
#include <xc.h>
#include "custom_pin.h"
#include "mcc_generated_files/mcc.h"
#include "TM1637Display_bak.h"

static void start(tPin DIO, tPin CLK);
static void stop(tPin DIO, tPin CLK);
static void write_byte(tPin DIO, tPin CLK, uint8_t b);

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
const uint8_t digitToSegment[] = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
  };

static const uint8_t minusSegments = 0b01000000;
/**
 * START
 * CMD_START
 * STOP
 * START
 * SELECT POS
 * STOP
 * START
 * DATA
 * STOP
 */


static void start(tPin DIO, tPin CLK) {
    write_reg(DIO, 0);
    write_reg(CLK, 1);
    __delay_ms(BITDELAY);
    write_reg(CLK, 0);
}

static void stop(tPin DIO, tPin CLK) {
    write_reg(DIO, 0);
    __delay_ms(BITDELAY);
    write_reg(CLK, 1);
    __delay_ms(BITDELAY);
    write_reg(DIO, 1);
    __delay_ms(BITDELAY);
}


static void write_byte(/*tTM1637Display *display*/tPin DIO, tPin CLK, uint8_t b) {
  uint8_t data = b;

  // 8 Data Bits
  for(uint8_t i = 0; i < 8; i++) {
    // CLK low
    //write_reg(CLK, 0);
      write_reg(CLK, 0);
    __delay_ms(BITDELAY);;

	// Set data bit
    write_reg(DIO, data & 0x01);
    __delay_ms(BITDELAY);;

	// CLK high
    write_reg(CLK, 1);
    __delay_ms(BITDELAY);;
    data = data >> 1;
  }

  // Wait for acknowledge
  // CLK to zero
  write_reg(CLK, 0);
  write_reg(DIO, 1);
  __delay_ms(BITDELAY);;

  // CLK to high
  write_reg(CLK, 1);
  __delay_ms(BITDELAY);
  write_reg(DIO, 0);

  __delay_ms(BITDELAY);;
  write_reg(CLK, 0);
  __delay_ms(BITDELAY);;
}

void tm1637_set_segments(tPin DIO, tPin CLK, uint8_t *data, uint8_t length, uint8_t pos) {
    // Write COMM1
	start(DIO, CLK);
	write_byte(DIO, CLK, TM1637_CMD_START);
	stop(DIO, CLK);

	// Write COMM2 + first digit address
	start(DIO, CLK);
	write_byte(DIO, CLK, TM1637_CMD_SELPOS + (pos & 0x03));

	// Write the data bytes
	for (uint8_t k=0; k < length; k++)
	  write_byte(DIO, CLK, data[k]);

	stop(DIO, CLK);

    uint8_t brightness = 0xFF;
    bool on = true;
    uint8_t m_brightness = (brightness & 0x7) | (on ? 0x08 : 0x00);
	// Write COMM3 + brightness
	start(DIO, CLK);
	write_byte(DIO, CLK, TM1637_CMD_DATA + (m_brightness & 0x0f));
	stop(DIO, CLK);
}
