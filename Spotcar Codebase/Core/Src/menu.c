/*
 * @file	menu.c
 *
 * @date	24 de mar. de 2026
 * @author	emanuel.staub
 *
 * @brief	
 */

#include "menu.h"
#include "main.h"
#include "HD44780.h"

static Button_State button_state;
static Menu_State menu_state = SELECT_MENU;

static uint32_t current;
static uint32_t timer_value = 0;
static uint32_t update_flag = 1;

void Read_Button_State(uint32_t ADC_reading){
	if(ADC_reading > 3700){ // No button pressed
		button_state = NO_BUTTON;
	} else if (ADC_reading > 2700){
		button_state = BUTTON_1;
	} else if (ADC_reading > 2200){
		button_state = BUTTON_2;
	} else if (ADC_reading > 1600){
		button_state = BUTTON_3;
	} else if (ADC_reading > 800){
		button_state = BUTTON_4;
	} else {
		button_state = TRIGGER;
	}
}

void Menu_Logic_Handler(){
	switch (menu_state){
	case SELECT_MENU:
		current = 10;
		switch (button_state){
		case BUTTON_1:
			timer_value = STAR_POINT_TIME;
			menu_state = CURRENT_MENU;
			update_flag = 1;
			break;
		case BUTTON_2:
			timer_value = WASHER_TIME;
			menu_state = CURRENT_MENU;
			update_flag = 1;
			break;
		case BUTTON_3:
			timer_value = COPPER_POINTER_TIME;
			menu_state = CURRENT_MENU;
			update_flag = 1;
			break;
		case BUTTON_4:
			timer_value = COAL_TIME;
			menu_state = CURRENT_MENU;
			update_flag = 1;
			break;
		default:
			break;
		}
		break;
	case CURRENT_MENU:
		switch (button_state){
		case BUTTON_2:
			Increase_Current();
			update_flag = 1;
			break;
		case BUTTON_3:
			Decrease_Current();
			update_flag = 1;
			break;
		case BUTTON_4:
			menu_state = SELECT_MENU;
			update_flag = 1;
			break;
		default:
			break;
		}
	}
}

void Menu_Update_Display(){
	if (update_flag){
		switch (menu_state){
		case SELECT_MENU:
			HD_Write_4_Lines("Estrela", "Arruela", "Ponteira de Cobre", "Carvao");
			break;
		case CURRENT_MENU:
			HD_Write_4_Lines("Escolha a Corrente", "I =                +", "                   -", "Cancelar");
		}
	}
}

void Increase_Current(){
	if(current < 200){
		current += 10;
	}
}

void Decrease_Current(){
	if(current > 10){
		current -= 10;
	}
}

uint32_t Current_Get_Compare(){
	return current*3;
}

uint32_t Test_Trigger_Time(uint32_t time){
	return (time >= timer_value);
}

uint32_t Is_Trigger_Ready(){
	return (menu_state != SELECT_MENU) & (button_state == TRIGGER);
}

