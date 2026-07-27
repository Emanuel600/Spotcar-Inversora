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

#define STAR_POINT_TIME 100
#define WASHER_TIME 200
#define COPPER_POINTER_TIME 400
#define COAL_TIME 450

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
	CURRENT_MENU,
	ADJUST_I_MENU,
	ADJUST_T_MENU
} Menu_State;

typedef enum Operation_Mode {
	OP_NONE,		// Menu
	OP_ESTRELA,
	OP_ARRUELA,
	OP_COBRE,
	OP_CARVAO
} Operation_Mode;

void Set_Trigger_Pin(uint16_t pin, GPIO_TypeDef* port);

void Set_Trigger_Time(volatile uint32_t* trigger_time);

void Read_Button_State(uint32_t ADC_reading);

void Menu_Logic_Handler();

void Menu_Update_Display();

void Increase_Current();

void Decrease_Current();

uint32_t Get_Target_Current();

uint32_t Get_Target_Time();

uint32_t Current_Get_Compare();

uint32_t Test_Trigger_Time(uint32_t time);

uint32_t Is_Trigger_Ready();

Operation_Mode Get_Op_Mode();

#endif /* INC_MENU_H_ */
