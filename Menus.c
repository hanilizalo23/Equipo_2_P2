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

}

void ChooseSubmenu(terminal_t terminal)
{

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
