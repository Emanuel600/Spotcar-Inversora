/*
 * @file	menu.h
 *
 * @date	24 de mar. de 2026
 * @author	emanuel.staub
 *
 * @brief	
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_

#include "main.h"

#define STAR_POINT_TIME 1
#define WASHER_TIME 3
#define COPPER_POINTER_TIME 5
#define COAL_TIME 10

typedef enum Button_State {
	NO_BUTTON,
	BUTTON_1,
	BUTTON_2,
	BUTTON_3,
	BUTTON_4,
	TRIGGER
} Button_State;

typedef enum Menu_State {
	SELECT_MENU,
	CURRENT_MENU
} Menu_State;

void Read_Button_State(uint32_t ADC_reading);

void Menu_Logic_Handler();

void Menu_Update_Display();

void Increase_Current();

void Decrease_Current();

uint32_t Is_Trigger_Ready();

uint32_t Current_Get_Compare();

uint32_t Test_Trigger_Time(uint32_t time);

#endif /* INC_MENU_H_ */
