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
static Menu_State menu_state = TOP_SELECT_MENU;
static Operation_Mode op_mode = OP_NONE;
static pin_s Trigger;

static uint32_t current;
static uint32_t update_flag = 1;
static uint32_t menu_scroll = 0; // 0 for TOP, 1 for BOTTOM

static uint32_t* timer_ptr;

static uint8_t Selected_Pulse_Lenght[5] = {
		[OP_ESTRELA] = 0,
		[OP_ARRUELA] = 0,
		[OP_COBRE]   = 0,
		[OP_CARVAO]  = 0
};

static const char* Current_to_String[] = {
	  // 30%,   40%,   50%,   60%,   70%,   80%,   90%,   100%
	  // 150,   165,   180,   195,   210,   225,   240,   255,   270,   285,   300
        "030", "040", "050", "060", "070", "080", "090", "100", "110", "120", "130"
    };

static const char* Pulse_to_String[][5] = {
		[OP_ESTRELA] = {"080", "120", "160", "200", "240"},
		[OP_ARRUELA] = {"300", "350", "375", "400", "450"},
		[OP_COBRE]   = {"400", "420", "440", "460", "480"},
		[OP_CARVAO]  = {"420", "440", "460", "480", "500"}
    };

static const char* Operation_Modes[] = {
		[OP_NONE]    = "",
		[OP_ESTRELA] = "Estrela",
		[OP_ARRUELA] = "Arruela",
		[OP_COBRE]   = "Cobre",
		[OP_CARVAO]  = "Carvao",
		[OP_PARAFIX] = "Parafix"
};

static const uint32_t Operation_Times[][5] = {
		[OP_NONE]    = {0, 0, 0, 0, 0},
		[OP_ESTRELA] = {80, 120, 160, 200, 240},
		[OP_ARRUELA] = {300, 350, 375, 400, 450},
		[OP_COBRE]   = {400, 420, 440, 460, 480},
		[OP_CARVAO]  = {420, 440, 460, 480, 500}
};

void Read_Button_State(uint32_t ADC_reading){
	button_state = NO_BUTTON;
	if (ADC_reading > 3700){
		button_state = NO_BUTTON;
	} else if (ADC_reading > 2800){
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
	case TOP_SELECT_MENU:
		current = 0;
		switch (button_state){
		case BUTTON_1:
			op_mode = OP_ESTRELA;
			menu_state = CURRENT_MENU;
			*timer_ptr = Operation_Times[OP_ESTRELA][0];
			update_flag = 1;
			break;
		case BUTTON_2:
			op_mode = OP_ARRUELA;
			menu_state = CURRENT_MENU;
			*timer_ptr = Operation_Times[OP_ARRUELA][0];
			update_flag = 1;
			break;
		case BUTTON_3:
			op_mode = OP_COBRE;
			menu_state = CURRENT_MENU;
			*timer_ptr = Operation_Times[OP_COBRE][0];
			update_flag = 1;
			break;
		case BUTTON_4:
			menu_scroll = 1;
			menu_state = BOTTOM_SELECT_MENU;
			update_flag = 1;
			break;
		default:
			update_flag = 0;
			break;
			}
		break;
	case BOTTOM_SELECT_MENU:
		current = 0;
		switch (button_state){
		case BUTTON_1:
			menu_scroll = 0;
			menu_state = TOP_SELECT_MENU;
			update_flag = 1;
			break;
		case BUTTON_2:
			op_mode = OP_CARVAO;
			menu_state = CURRENT_MENU;
			*timer_ptr = Operation_Times[OP_CARVAO][0];
			update_flag = 1;
			break;
		case BUTTON_3:
			op_mode = OP_PARAFIX;
			menu_state = PARAFIX_MENU;
			update_flag = 1;
			break;
		case BUTTON_4:
			//
		default:
			update_flag = 0;
			break;
		}
		break;
	case CURRENT_MENU:
		switch (button_state){
		case BUTTON_1:
			if ((op_mode == OP_COBRE) | (op_mode == OP_CARVAO)){
				update_flag = 0;
			} else{
				menu_state = ADJUST_T_MENU;
				update_flag = 1;
			}
			break;
		case BUTTON_2:
			Increase_Current();
			menu_state = ADJUST_I_MENU;
			update_flag = 1;
			break;
		case BUTTON_3:
			Decrease_Current();
			menu_state = ADJUST_I_MENU;
			update_flag = 1;
			break;
		case BUTTON_4:
			menu_state = TOP_SELECT_MENU;
			update_flag = 1;
			break;
		default:
			update_flag = 0;
			break;
		}
		break;
		case ADJUST_I_MENU:
				switch (button_state){
				case BUTTON_1:
					menu_state = ADJUST_T_MENU;
					update_flag = 1;
					break;
				case BUTTON_2:
					Increase_Current();
					update_flag = 1;
					break;
				case BUTTON_3:
					Decrease_Current();
					update_flag = 1;
					break;
				case BUTTON_4:
					menu_state = TOP_SELECT_MENU;
					update_flag = 1;
					break;
				default:
					update_flag = 0;
					break;
				}
				break;
		case ADJUST_T_MENU:
				uint32_t i = Selected_Pulse_Lenght[op_mode];
				switch(button_state){
				case BUTTON_1:
					menu_state = ADJUST_I_MENU;
					update_flag = 1;
					break;
				case BUTTON_2:
					i = (i<4) ? i+1 : i;
					update_flag = 1;
					break;
				case BUTTON_3:
					i = (i>0) ? i-1 : i;
					update_flag = 1;
					break;
				case BUTTON_4:
					menu_state = TOP_SELECT_MENU;
					update_flag = 1;
					break;
				default:
					update_flag = 0;
					break;
				}
				Selected_Pulse_Lenght[op_mode] = i;
				*timer_ptr = Operation_Times[op_mode][Selected_Pulse_Lenght[op_mode]];
				break;
		case PARAFIX_MENU:
			switch(button_state){
			case BUTTON_4:
				menu_state   = TOP_SELECT_MENU;
				update_flag  = 1;
				break;
			default:
				update_flag = 0;
				break;
			}
	}
	button_state = NO_BUTTON;
}

void Menu_Update_Display(){
	if (update_flag){
		switch (menu_state){
		case TOP_SELECT_MENU:
			HD_Write_4_Lines("Estrela", "Arruela", "Ponteira de Cobre", "\x01");
			break;
		case BOTTOM_SELECT_MENU:
			HD_Write_4_Lines("\x02", "Carvao", "Parafix", "");
			break;
		case CURRENT_MENU:
			if((op_mode == OP_COBRE) | (op_mode == OP_CARVAO)){
				HD_Write_4_Lines(Operation_Modes[op_mode], "\x7E P = 030 \%       +", "", "Cancelar");
			} else{
				HD_Write_4_Lines(Operation_Modes[op_mode], "\x7E P = 030 \%       +", "  t = 075 ms      -", "Cancelar");
				HD_Set_Cursor(LINE_3 + 6);
				HD_Write(Pulse_to_String[op_mode][Selected_Pulse_Lenght[op_mode]]);
			}
			break;
		case ADJUST_I_MENU:
			HD_Set_Cursor(LINE_2);
			HD_Write("\x7E");
			HD_Set_Cursor(LINE_3);
			HD_Write(" ");
			HD_Set_Cursor(LINE_2 + 6);
			HD_Write(Current_to_String[current]);
			break;
		case ADJUST_T_MENU:
			HD_Set_Cursor(LINE_3);
			HD_Write("\x7E");
			HD_Set_Cursor(LINE_2);
			HD_Write(" ");
			HD_Set_Cursor(LINE_3 + 6);
			HD_Write(Pulse_to_String[op_mode][Selected_Pulse_Lenght[op_mode]]);
			break;
		case PARAFIX_MENU:
			HD_Write_4_Lines(Operation_Modes[op_mode], "", "", "Cancelar");
			break;
		default:
			break;
		}
	}
}

void Create_Custom_Chars(){
	HD_Char up_arrow = {
		0B00000, //
		0B00100, //   *
		0B01110, //  ***
		0B11111, // *****
		0B00000, //
		0B00000, //
		0B00000, //
		0B00000  //
	};

	HD_Char down_arrow = {
		0B00000, //
		0B00000, //
		0B00000, //
		0B00000, //
		0B11111, // *****
		0B01110, //  ***
		0B00100, //   *
		0B00000  //
	};

	HD_Create_Custom_Character(2, up_arrow);
	HD_Create_Custom_Character(1, down_arrow);
}

inline void Increase_Current(){
	if(current < 7){
		current ++;
	}
}

inline void Decrease_Current(){
	if(current > 0){
		current --;
	}
}

inline uint32_t Get_Target_Current(){
	return current;
}

inline uint32_t Current_Get_Compare(){
	if (menu_state != PARAFIX_MENU){
		return (150 + (current*15));
	} else{
		return 100;
	}
}


inline uint32_t Is_Trigger_Ready(){
	return ((!((menu_state == TOP_SELECT_MENU) | (menu_state == BOTTOM_SELECT_MENU))) & (HAL_GPIO_ReadPin(Trigger.port, Trigger.pin) == GPIO_PIN_RESET));
}

void Hold_Until_Trigger_Release(){
	while (HAL_GPIO_ReadPin(Trigger.port, Trigger.pin) == GPIO_PIN_RESET){
		__NOP();
	}
	return;
}
inline void Set_Trigger_Pin(uint16_t pin, GPIO_TypeDef* port){
	Trigger.pin = pin;
	Trigger.port = port;
}

inline void Set_Trigger_Time(volatile uint32_t* trigger_time){
	timer_ptr = trigger_time;
}

inline Operation_Mode Get_Op_Mode(){
	return op_mode;
}
