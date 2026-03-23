/*
 * @file	Hd44780.h
 *
 * @date	23 de mar. de 2026
 * @author	emanuel.staub
 *
 * @brief	
 */

#ifndef INC_HD44780_H_
#define INC_HD44780_H_

#include "main.h"

#define CLEAR_DISPLAY		0x01
#define RETURN_HOME			0x02
#define ENTRY_MODE_SET		0x04
#define DISPLAY_ON_OFF		0x08
#define CURSOR_DISP_SHIFT	0x10
#define FUNCTION_SET		0x20
#define SET_CGRAM			0x40
#define SET_DDRAM			0x80

#define CURSOR_INC			0x02
#define SHIFT_DISPLAY		0x01

#define SET_DISPLAY			0x04
#define SET_CURSOR			0x08
#define SET_BLINKING		0x01

#define DISPLAY_SHIFT		0x08
#define MOVE_RIGHT			0x04

#define DL_8_BITS			0x10
#define TWO_LINES			0x08
#define FONT_5X10			0x04

#define LINE_1				0x00
#define LINE_2				0x40
#define LINE_3				0x14
#define LINE_4				0x54


typedef struct pin_s {
	GPIO_TypeDef* port;
	uint16_t pin;
} pin_s;

typedef struct HD44780 {
	pin_s* Data_pins; // Only 4 bits
	pin_s* E_RS_pins; // Enable/RS pins
} HD44780;

void delay_us(uint32_t delay);

void HD_Init(pin_s* data_pins, pin_s* e_rs_pins);

void HD_Set_Cursor(uint32_t pos);

void HD_Write(const char* data);

void HD_Write8(char data);

void HD_Write4(char data_nibble);

void HD_Command(uint32_t command);

void HD_Command4(uint32_t command_nibble);

void HD_Pulse_Enable();

void Clear_Display();

#endif /* INC_HD44780_H_ */
