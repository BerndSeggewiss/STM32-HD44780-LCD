/*
 * lcd.h
 *
 *  Created on: Jun 23, 2025
 *      Author: Win11 Pro
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>

typedef struct
{
	uint8_t slot;
	char bitmap[8];
} LCD_CustomCharDef;

// Max. number of custom chars
#define LCD_MAX_CUSTOMCHARS 8

// Used Pins
#define LCD_RW GPIO_PIN_5	// PA5
#define LCD_RS GPIO_PIN_6	// PA6
#define LCD_EN GPIO_PIN_7	// PA7
#define D4_Pin GPIO_PIN_8	// PA8
#define D5_Pin GPIO_PIN_9	// PA9
#define D6_Pin GPIO_PIN_10	// PA10
#define D7_Pin GPIO_PIN_11	// PA11

// Custom-Char slot definitions
#define LCD_CGRAM_SLOT_PLUSMINUS	0
#define LCD_CGRAM_SLOT_ARROW		1
#define LCD_CGRAM_SLOT_CLUSTERDOT	2
#define LCD_CGRAM_SLOT_ALPHA		3
#define LCD_CGRAM_SLOT_BETA			4
#define LCD_CGRAM_SLOT_MU			5
#define LCD_CGRAM_SLOT_OHM			6
#define LCD_CGRAM_SLOT_DEG			7

extern char CharPlusMinus[8];
extern char CharArrow[8];
extern char CharClusterDot[8];
extern char CharAlpha[8];
extern char CharBeta[8];
extern char CharMu[8];
extern char CharOhm[8];
extern char CharDeg[8];

#define LCD_CMD_8BIT_MODE      0x3   // Initialisierungsschritt: 8-Bit-Modus (nur Nibble!)
#define LCD_CMD_4BIT_MODE      0x2   // Initialisierungsschritt: Umschalten auf 4-Bit-Modus (nur Nibble!)
#define LCD_FUNCTION_SET       0x28  // 4-Bit, 2 Zeilen, 5x8 Dots
#define LCD_DISPLAY_OFF        0x08  // Display aus
#define LCD_DISPLAY_ON         0x0C  // Display an, Cursor aus, Blinken aus
#define LCD_CLEAR              0x01  // Display löschen
#define LCD_HOME               0x02  // Cursor Home
#define LCD_ENTRY_MODE         0x06  // Cursor nach rechts, kein Display-Shift
#define LCD_SET_CGRAM_ADDR     0x40  // Setze CGRAM-Adresse (für Custom Characters)
#define LCD_SET_DDRAM_ADDR1    0x80  // Setze Cursor an Anfang erste Zeile
#define LCD_SET_DDRAM_ADDR2    0xC0  // Setze Cursor an Anfang zweite Zeile

unsigned char LCD_waitBusy(void);
void enablePuls(void);
void LCD_WriteNibble(uint8_t);
void LCD_WriteCmd(uint8_t);
void LCD_WriteData(uint8_t);
void LCD_putChar(char c);
void LCD_putString(char *s);
void LCD_CursorShift(uint8_t, uint8_t);
void LCD_Clear(void);
void LCD_CursorHome(void);
void LCD_createChar(uint8_t, char*);
void LCD_print(const char *fmt, ...);
void LCD_createDefaultCustomChars(void);

void LCD_LoadCustomChars(void);

void LCD_RegisterCustomChar(uint8_t slot, char* bitmap);

void LCD_Init(void);

#endif /* LCD_H_ */
