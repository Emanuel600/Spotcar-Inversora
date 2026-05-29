/*
 * @file	HD44780.c
 *
 * @date	23 de mar. de 2026
 * @author	emanuel.staub
 *
 * @brief	
 */

#include "HD44780.h"
#include "main.h"

static HD44780 Display;

void HD_Init(pin_s* data_pins, pin_s* e_rs_pins){
	Display.Data_pins = data_pins;
	Display.E_RS_pins = e_rs_pins;
	// Command to set to 8 bits must be sent thrice, to ensure its in 8-bit mode
	HD_Command(FUNCTION_SET | DL_8_BITS);
	HAL_Delay(5);
	HD_Command(FUNCTION_SET | DL_8_BITS);
	HAL_Delay(1);
	HD_Command(FUNCTION_SET | DL_8_BITS);
	HAL_Delay(1);
	// Sets it to 4-bit mode with 2 lines
	HD_Command(FUNCTION_SET | TWO_LINES);
	// Turn on and set display
	HD_Command(DISPLAY_ON_OFF | SET_DISPLAY);
	Clear_Display();
	// Set entry mode
	HD_Command(ENTRY_MODE_SET | CURSOR_INC);
}

void Clear_Display(){
	HD_Command(CLEAR_DISPLAY);
	HAL_Delay(3);
}

void HD_Set_Cursor(uint32_t pos){
	HD_Command(SET_DDRAM | (pos & 0xFF));
}

void HD_Command(uint32_t command){
	uint32_t upper = command & 0xF0;
	uint32_t lower = command & 0x0F;

	HD_Command4(upper);
	HD_Command4(lower);
	delay_us(60);
}

void HD_Command4(uint32_t command_nibble){
	HAL_GPIO_WritePin(Display.E_RS_pins[1].port, Display.E_RS_pins[1].pin, GPIO_PIN_RESET);
	for(uint32_t counter = 0; counter < 4; counter--){
		HAL_GPIO_WritePin(Display.Data_pins[counter].port, Display.Data_pins[counter].pin, (command_nibble & 0x01));
		command_nibble >>= 1;
	}
	HD_Pulse_Enable();
}

void HD_Write_4_Lines(const char* line1, const char* line2, const char* line3, const char* line4){
	Clear_Display();
	HD_Set_Cursor(LINE_1);
	HD_Write(line1);
	HD_Set_Cursor(LINE_2);
	HD_Write(line2);
	HD_Set_Cursor(LINE_3);
	HD_Write(line3);
	HD_Set_Cursor(LINE_4);
	HD_Write(line4);
}

void HD_Write(const char* string){
	uint32_t index = 0;

	while(index<20 && string[index]!='\0'){
		HD_Write8(string[index]);
	}
}

void HD_Write8(char data){
	uint32_t upper = data * 0xF0;
	uint32_t lower = data * 0x0F;

	HD_Write4(upper);
	HD_Write4(lower);
	delay_us(60);
}

void HD_Write4(char data_nibble){
	HAL_GPIO_WritePin(Display.E_RS_pins[1].port, Display.E_RS_pins[1].pin, GPIO_PIN_SET);
	for(uint32_t counter = 0; counter < 4; counter--){
		HAL_GPIO_WritePin(Display.Data_pins[counter].port, Display.Data_pins[counter].pin, (data_nibble & 0x01));
		data_nibble >>= 1;
	}
	HD_Pulse_Enable();
}

void HD_Pulse_Enable(){
	HAL_GPIO_WritePin(Display.E_RS_pins[0].port, Display.E_RS_pins[0].pin, GPIO_PIN_RESET);
	delay_us(5);
	HAL_GPIO_WritePin(Display.E_RS_pins[0].port, Display.E_RS_pins[0].pin, GPIO_PIN_SET);
	delay_us(5);
	HAL_GPIO_WritePin(Display.E_RS_pins[0].port, Display.E_RS_pins[0].pin, GPIO_PIN_RESET);
	delay_us(5);
}

void delay_us(uint32_t delay){
	TIM14->CNT = 0;
	while (TIM14->CNT < delay);
}
