/*
 * \file 	TimeDate.c
 *		This source file includes the functions needed to set and read hour and date
**		from both terminals.
 *		
 *  Created on: 14/04/2022
 *      Author: Mauricio Peralta
 */

#include "TimeDate.h"
#include "MK64F12.h"
#include "stdbool.h"
#include "fsl_uart.h"
#include "Bits.h"
#include "HC05.h"
#include "UART_PC.h"
#include "MCP7940M.h"
#include "I2C.h"
#include <stdio.h>

#define NOTHING 0U
#define ONE_LENGHT 1U
#define TWO_DIGITS 2U
#define FIVE_DIGITS 5U
#define my_sizeof(type) (char *)(&type+1)-(char*)(&type) //Own function for my_my_sizeof, cause we need it

static program_status_t g_status_td[2];
static uint8_t g_data_time[2];
static uint8_t g_digits_counter_time;
static uint8_t g_time_dig_counter;
static uint8_t g_date_dig_counter;
static uint8_t g_temp_time[TIME_TOTAL_DIGS];
static uint8_t g_temp_date[TIME_TOTAL_DIGS];

uint8_t SetT1 [] = "\r\nEscribir hora en hh:mm:ss\r\n";
uint8_t SetT2 [] = "\r\nLa hora ha sido cambiada...\r\n";
uint8_t SetD1 [] = "\r\nEscribir fecha en dd/mm/aa:\r\n";
uint8_t SetD2 [] = "\r\nLa fecha ha sido cambiada...\r\n";
uint8_t ReadT [] = "\r\nLa hora actual es:\r\n";
uint8_t ReadD [] = "\r\nLa fecha actual es:\r\n";

//Flags for "I2C device in use"
static uint8_t g_rtc_in_use = false;

//Functions pointers arrays for all the stages of each submenu
void (*p_settime_pc_funcs[3])(terminal_t terminal) = {SetTime_Start,SetTime_Read,SetTime_Save};
void (*p_setdate_pc_funcs[3])(terminal_t terminal) = {SetDate_Start,SetDate_Read,SetDate_Save};
void (*p_readtime_pc_funcs[3])(terminal_t terminal) = {ReadTime_Start,ReadTime_Write,ReadTime_Exit};
void (*p_readdate_pc_funcs[3])(terminal_t terminal) = {ReadDate_Start,ReadDate_Write,ReadDate_Exit};

program_status_t TimeDate_Choose(terminal_t terminal, program_status_t actual_status, uint8_t *read_data)
{
	uint8_t submenu_stage;
	g_status_td[terminal] = actual_status;
	submenu_stage = actual_status.stage - ONE_LENGHT;
	g_data_time[terminal] = *read_data;
	switch(actual_status.submenu)
	{
	case SET_TIME:
		p_settime_pc_funcs[submenu_stage](terminal);
		break;
	case SET_DATE:
		p_setdate_pc_funcs[submenu_stage](terminal);
		break;
	case READ_TIME:
		p_readtime_pc_funcs[submenu_stage](terminal);
		break;
	case READ_DATE:
		p_readdate_pc_funcs[submenu_stage](terminal);
		break;
	default:
		break;
	}
	return(g_status_td[terminal]);
}

//Set Time
void SetTime_Start(terminal_t terminal)
{
	if(g_rtc_in_use)
	{
		Print_On_Terminal(terminal,I2C_in_use,my_sizeof(I2C_in_use) - ONE_LENGHT);
		g_status_td[terminal].stage = MAIN_MENU;
		g_status_td[terminal].submenu = NONE;
	}
	else
	{
		g_rtc_in_use = true;
		Print_On_Terminal(terminal,SetT1,my_sizeof(SetT1) - ONE_LENGHT);
		//Restart counters
		g_digits_counter_time = NOTHING;
		g_time_dig_counter = NOTHING;
		//Change to next stage
		g_status_td[terminal].stage = SUBMENU;
		if(RTC_verify_communication())
		{
			Print_On_Terminal(terminal,I2C_Error,my_sizeof(I2C_Error) - ONE_LENGHT);
			g_status_td[terminal].stage = SUBMENU_OUT;
		}
	}
}




}
