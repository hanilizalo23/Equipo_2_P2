/*
 * \file 	Menus.c
 *		This source file includes the functions to access to each menu/submenu
**		of the program
 *		
 *  Created on: 14/04/2022
 *      Author: Nelida Hernández
 */

#include "Menus.h"
#include "MK64F12.h"
#include "stdbool.h"
#include "fsl_uart.h"
#include "Chat.h"
#include <stdio.h>

#define NOTHING 0U
#define ONE_LENGHT 1U
#define my_sizeof(type) (char *)(&type+1)-(char*)(&type) //Own function for sizeof, cause we need it

static program_status_t g_actual_status[2] = {{MAIN_MENU, NONE,false},{MAIN_MENU, NONE,false}};
static uint8_t read_data[2];

//Strings for messages
uint8_t MainMenu[] = "\r\n1) Establecer Hora\r\n2) Establecer Fecha\r\n3) Leer hora\r\n4) Leer fecha\r\n5) Escribir mensaje en memoria I2C\r\n6) Leer mensaje en memoria I2C\r\n7) Chat\r\n8) Mostrar mensaje en matriz de LEDS\r\n";

uint8_t ConfigureAll(void)
{

}

flow_flags_t ReadTerminals(void)
{

}

void ChooseStage(terminal_t terminal)
{
	switch(g_actual_status[terminal].stage)
	{
	case MAIN_MENU:
		Start_Menu(terminal);
		break;
	case CHOOSE_OPT:
		//If the ASCII code is between 48 and 57 (numbers), convert to the real number
		if((ASCII_0 <= read_data[terminal]) && (ASCII_9 >= read_data[terminal]))
		{
			Print_On_Terminal(terminal,&read_data[terminal],1);
			g_actual_status[terminal].submenu = read_data[terminal] - ASCII_0;
		}
	case SUBMENU:
	case SUBMENU_OUT:
		ChooseSubmenu(terminal);
		break;
	default:
		break;
	}
}

void ChooseSubmenu(terminal_t terminal)
{
	switch(g_actual_status[terminal].submenu){
	case SET_TIME:
	case SET_DATE:
	case READ_TIME:
	case READ_DATE:
		TimeDate_Menu(terminal);
		break;
	case WRITE_MEM:
	case READ_MEM:
		Memory_Menu(terminal);
		break;
	case CHAT:
		Chat_Menu(terminal);
		break;
	case LED_MATRIX:
		//Led_Matrix(terminal); //We didn't reach to do this part, just a few things, basically, this just don't work
		break;
	default:
		break;
	}
}

void Start_Menu(terminal_t terminal)
{
	if(PC == terminal)
	{
		UART_PC_write(MainMenu,my_sizeof(MainMenu) - ONE_LENGHT);
	}
	else
	{
		HC05_write(MainMenu,my_sizeof(MainMenu) - ONE_LENGHT);
	}
	g_actual_status[terminal].stage = CHOOSE_OPT;
}

void TimeDate_Menu(terminal_t terminal)
{
	g_actual_status[terminal] = TimeDate_Choose(terminal, g_actual_status[terminal],&read_data[terminal]);
}

void Memory_Menu(terminal_t terminal)
{
	g_actual_status[terminal] = Memory_Choose(terminal,g_actual_status[terminal],&read_data[terminal]);
}

void Chat_Menu(terminal_t terminal)
{
	g_actual_status[terminal] = Chat_Choose(terminal,g_actual_status[terminal],&read_data[terminal]);
}

void Print_On_Terminal(terminal_t terminal, uint8_t *data, size_t length)
{
	if(PC == terminal)
	{
		UART_PC_write(data,length);
	}
	else
	{
		HC05_write(data,length);
	}
}
