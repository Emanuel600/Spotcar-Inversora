/*
 * @file	menu.c
 *
 * @date	24 de mar. de 2026
 * @author	emanuel.staub
 *
 * @brief	
 */
#include <string.h>

#include "menu.h"
#include "main.h"
#include "HD44780.h"

static Button_State button_state;
static Menu_State menu_state = SELECT_MENU;

static pin_s Trigger;

static uint32_t current;
static uint32_t timer_value = 0;
static uint32_t update_flag = 1;

static const char* Decimal_to_String[] = {
        "000", "010", "020", "030", "040", "050", "060", "070", "080", "090", "100",
        "110", "120", "130", "140", "150", "160", "170", "180", "190", "200"
    };

void Read_Button_State(uint32_t ADC_reading){
	button_state = NO_BUTTON;
	if ((ADC_reading < 3700) & (ADC_reading > 2800)){
		button_state = BUTTON_1;
	} else if (ADC_reading > 2200){
		button_state = BUTTON_2;
	} else if (ADC_reading > 1600){
		button_state = BUTTON_3;
	} else if (ADC_reading > 1000){
		button_state = BUTTON_4;
	}
}

void Menu_Logic_Handler(){
	switch (menu_state){
	case SELECT_MENU:
		current = 1;
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
			menu_state = ADJUST_MENU;
			update_flag = 1;
			break;
		case BUTTON_3:
			Decrease_Current();
			menu_state = ADJUST_MENU;
			update_flag = 1;
			break;
		case BUTTON_4:
			menu_state = SELECT_MENU;
			update_flag = 1;
			break;
		default:
			break;
		}
		case ADJUST_MENU:
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
	button_state = NO_BUTTON;
}

void Menu_Update_Display(){
	if (update_flag){
		switch (menu_state){
		case SELECT_MENU:
			HD_Write_4_Lines("Estrela", "Arruela", "Ponteira de Cobre", "Carvao");
			break;
		case CURRENT_MENU:
			HD_Write_4_Lines("Escolha a Corrente", "I = 10  A", "                   -", "Cancelar");
			break;
		case ADJUST_MENU:
			HD_Set_Cursor(LINE_2 + 4);
			HD_Write(Decimal_to_String[current]);
			break;
		default:
			break;
		}
	}
}

inline void Increase_Current(){
	if(current < 20){
		current ++;
	}
}

inline void Decrease_Current(){
	if(current > 1){
		current --;
	}
}

inline uint32_t Get_Target_Current(){
	return current;
}

inline uint32_t Current_Get_Compare(){
	return current*15;
}

inline uint32_t Test_Trigger_Time(uint32_t time){
	return (time >= timer_value);
}

inline uint32_t Is_Trigger_Ready(){
	return ((menu_state != SELECT_MENU) & (HAL_GPIO_ReadPin(Trigger.port, Trigger.pin) == GPIO_PIN_SET));
}

inline void Set_Trigger_Pin(uint16_t pin, GPIO_TypeDef* port){
	Trigger.pin = pin;
	Trigger.port = port;
}

