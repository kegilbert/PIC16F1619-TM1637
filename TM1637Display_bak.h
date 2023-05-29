/*
 * File:   TM1637Display.c
 * Author: kgilbert
 *
 * Created on January 14, 2022, 3:37 PM
 */

#ifndef TM1637DISPLAY
#define TM1637DISPLAY

#define TM1637_CMD_START    0x40
#define TM1637_CMD_SELPOS   0xC0
#define TM1637_CMD_DATA     0x80

#define BITDELAY 1

typedef struct {
  unsigned CLK;
  unsigned DIO;
} tTM1637Display;

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

void tm1637_set_segments(tPin DIO, tPin CLK, uint8_t *data, uint8_t length, uint8_t pos);

#endif