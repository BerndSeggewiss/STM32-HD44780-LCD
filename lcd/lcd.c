/*
 * lcd.c
 *
 *  Created on: Jun 23, 2025
 *      Author: Bernd Seggewiß
 */

#include "main.h"
#include <string.h>
#include "lcd.h"
#include <stdarg.h>
#include <stdio.h>


char DefCharPlusMinus[8] = {0x04, 0x04, 0x1F, 0x04, 0x04, 0x00, 0x1F, 0x00};		// 0
char DefCharArrow[8] = {0x08, 0x0C, 0x0E, 0x0F, 0x0E, 0x0C, 0x08, 0x00};			// 1
char DefCharAlpha[8] = { 0x00, 0x0E, 0x01, 0x0F, 0x11, 0x11, 0x0F, 0x00 };			// 2
char DefCharBeta[8] = { 0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x16, 0x10 };			// 3
char DefCharMu[8]    = { 0x00, 0x11, 0x11, 0x11, 0x13, 0x0D, 0x01, 0x00 };			// 4
char DefCharOhm[8]   = { 0x00, 0x0E, 0x11, 0x11, 0x11, 0x0E, 0x0A, 0x11 }; 			// 5
char DefCharDeg[8]   = { 0x06, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00 };			// 6
char DefCharClusterDot[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18 };		// 7


static LCD_CustomCharDef lcd_custom_chars[LCD_MAX_CUSTOMCHARS];

void LCD_print(const char *fmt, ...)
{
	char buffer[32];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	for (const char *p = buffer; *p; p++)
	{
		if (*p == '.')
		{
			LCD_putChar(7);
		}
		else
		{
			LCD_putChar(*p);
		}
	}
}

void LCD_createChar(uint8_t adress, char *charmap)
{
	adress &= 0x07;
	LCD_WriteCmd(LCD_SET_CGRAM_ADDR | (adress * 8));
	for (uint8_t i = 0; i < 8; i++)
	{
		LCD_WriteData(charmap[i]);
		//HAL_Delay(1);
	}
	LCD_WriteCmd(LCD_SET_DDRAM_ADDR1);
}

unsigned char LCD_waitBusy(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	//GPIO_InitStruct.Pin = D4_Pin | D5_Pin  |  D6_Pin | D7_Pin;
	GPIO_InitStruct.Pin = D7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA, LCD_RW, GPIO_PIN_RESET);	// RW = 0 (Write)
	HAL_GPIO_WritePin(GPIOA, LCD_RS, GPIO_PIN_RESET); 	// RS = 0 (Instruction)
	HAL_GPIO_WritePin(GPIOA, LCD_RW, GPIO_PIN_SET);		// RW = 1 (Read)
	uint8_t busy = 0;
	uint32_t timeout = 10000;
	do
	{
		HAL_GPIO_WritePin(GPIOA, LCD_EN, GPIO_PIN_SET);
		busy = HAL_GPIO_ReadPin(GPIOA, D7_Pin);
		HAL_GPIO_WritePin(GPIOA, LCD_EN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, LCD_EN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, LCD_EN, GPIO_PIN_RESET);
		if (!(--timeout)) break;
	}
	while (busy);
	HAL_GPIO_WritePin(GPIOA, LCD_RW, GPIO_PIN_RESET);	// RW = 0 (Write)
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	return busy;
}

void enablePuls(void)
{
	HAL_GPIO_WritePin(GPIOA, LCD_EN, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, LCD_EN, GPIO_PIN_RESET);
}

void LCD_WriteNibble(uint8_t nibble)
{
	const uint16_t pins[4] = { D4_Pin, D5_Pin, D6_Pin, D7_Pin };
	for (int i = 0; i < 4; i++)
		HAL_GPIO_WritePin(GPIOA, pins[i], (nibble & (1 << i) ) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	enablePuls();
}

void LCD_WriteCmd(uint8_t c)
{
	uint8_t highNibble = c >> 4;
	uint8_t lowNibble  = c & 0x0F;
	HAL_GPIO_WritePin(GPIOA, LCD_RS, GPIO_PIN_RESET);
	LCD_WriteNibble(highNibble);
	LCD_WriteNibble(lowNibble);
}

void LCD_WriteData(uint8_t data)
{
	 uint8_t highNibble = data >> 4;
	 uint8_t lowNibble  = data & 0x0F;
	 HAL_GPIO_WritePin(GPIOA, LCD_RS, GPIO_PIN_SET);
	 LCD_WriteNibble(highNibble);
	 LCD_WriteNibble(lowNibble);
}

void LCD_putChar(char c)
{
	LCD_WriteData((uint8_t)c);
}

void LCD_putString(char *s)
{
	while(*s)
		LCD_WriteData(*s++);
}

void LCD_CursorShift(uint8_t row, uint8_t col)
{
    if (col > 15)
        return;
    uint8_t address;
    if (row == 0)
        address = LCD_SET_DDRAM_ADDR1 | col;
    else if (row == 1)
        address = LCD_SET_DDRAM_ADDR2 | col;
    else
        return;
    LCD_WriteCmd(address);
}

void LCD_Clear(void)
{
	LCD_WriteCmd(LCD_CLEAR);
}

void LCD_CursorHome(void)
{
	LCD_WriteCmd(LCD_HOME);
}

void LCD_createDefaultCustomChars(void)
{
	LCD_RegisterCustomChar(0, DefCharPlusMinus);
	LCD_RegisterCustomChar(1, DefCharArrow);
	LCD_RegisterCustomChar(2, DefCharAlpha);
	LCD_RegisterCustomChar(3, DefCharBeta);
	LCD_RegisterCustomChar(4, DefCharMu);
	LCD_RegisterCustomChar(5, DefCharOhm);
	LCD_RegisterCustomChar(6, DefCharDeg);
	LCD_RegisterCustomChar(7, DefCharClusterDot);
	LCD_LoadCustomChars();
}


void LCD_RegisterCustomChar(uint8_t slot, char* bitmap)
{
	if (slot >= LCD_MAX_CUSTOMCHARS)
		return;
	lcd_custom_chars[slot].slot = slot;
	memcpy(lcd_custom_chars[slot].bitmap, bitmap, 8);
}

void LCD_LoadCustomChars(void)
{
	for (uint8_t i = 0; i < LCD_MAX_CUSTOMCHARS; ++i)
	{
		LCD_createChar(lcd_custom_chars[i].slot, lcd_custom_chars[i].bitmap);
	}
}

void LCD_Init(void)
{
	HAL_Delay(50);
	LCD_WriteNibble(LCD_CMD_8BIT_MODE);
	HAL_Delay(5);
	LCD_WriteNibble(LCD_CMD_8BIT_MODE);
	HAL_Delay(1);
	LCD_WriteNibble(LCD_CMD_8BIT_MODE);
	HAL_Delay(1);
	LCD_WriteNibble(LCD_CMD_4BIT_MODE);
	HAL_Delay(1);
	LCD_WriteCmd(LCD_FUNCTION_SET);
	HAL_Delay(5);
	LCD_WriteCmd(LCD_DISPLAY_OFF);
	HAL_Delay(5);
	LCD_WriteCmd(LCD_CLEAR);
	LCD_waitBusy();
	LCD_WriteCmd(LCD_ENTRY_MODE);
	LCD_WriteCmd(LCD_DISPLAY_ON);
	LCD_createDefaultCustomChars();
}






