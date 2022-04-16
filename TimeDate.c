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
static time_store_t g_real_time;
static time_store_t g_last_time;
static date_store_t g_real_date;
static uint8_t g_valid;
static terminal_t g_actual_terminal;

//Strings for messages
uint8_t SetT1 [] = "\r\nEscribir hora en hh:mm:ss\r\n";
uint8_t SetT2 [] = "\r\nLa hora ha sido cambiada...\r\n";
uint8_t SetD1 [] = "\r\nEscribir fecha en dd/mm/aa:\r\n";
uint8_t SetD2 [] = "\r\nLa fecha ha sido cambiada...\r\n";
uint8_t ReadT [] = "\r\nLa hora actual es:\r\n";
uint8_t ReadD [] = "\r\nLa fecha actual es:\r\n";
uint8_t I2C_in_use [] = "\r\nDispositivo en uso por otra terminal\r\n";
uint8_t I2C_Error [] = "\r\nERROR: No se pudo comunicar con el dispositivo I2C\r\n";
uint8_t Press_ESC [] = "\r\n\nPresione ESC para salir\r\n";
uint8_t Time_Error [] = "\r\nIngrese una hora valida: \r\n";
uint8_t Date_Error [] = "\r\nIngrese una fecha valida: \r\n";
uint8_t Adjust[] = "\n\n\n";
uint8_t Move_Cursor_FW [] = "\033[8D";
uint8_t Move_Cursor_UP [] = "\033[3A";
uint8_t Move_Cursor_DOWN [] = "\033[3B";

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

void SetTime_Read(terminal_t terminal)
{
	uint8_t real_number;
	//Only writes numbers and colons if they are on the corresponding place
	if((ASCII_9 >= g_data_time[terminal]) && (ASCII_0 <= g_data_time[terminal]))
	{
		Print_On_Terminal(terminal,&g_data_time[terminal],ONE_LENGHT);
		real_number = g_data_time[terminal] - ASCII_0;
		//Save on the variable for time
		g_temp_time[g_time_dig_counter] = real_number;
		//Increase counters for both reading and store
		g_digits_counter_time ++;
		g_time_dig_counter ++;
	}
	else if((ASCII_COLON == g_data_time[terminal]) && ((TWO_DIGITS == g_digits_counter_time) || (FIVE_DIGITS == g_digits_counter_time)))
	{
		Print_On_Terminal(terminal,&g_data_time[terminal],ONE_LENGHT);
		g_digits_counter_time++;
	}
	//Verify if the number of digits has reached the limit
	if(TIME_DIGITS == g_digits_counter_time)
	{
		g_status_td[terminal].stage = SUBMENU_OUT;
	}
}

void SetTime_Save(terminal_t terminal)
{
	//Only if ENTER is pressed, save the value
	if(ASCII_ENTER == g_data_time[terminal])
	{

		//Transform to time_store_t
		g_real_time = Array_to_time(g_temp_time);
		//Save to RTC
		g_valid = RTCLOCK_write_time(g_real_time);
		//Free the device
		g_rtc_in_use = false;
		//Validate the time
		if(!g_valid)
		{
			Print_On_Terminal(terminal,Time_Error,my_sizeof(Time_Error) - ONE_LENGHT);
			SetTime_Start(terminal);
		}
		else
		{
			Print_On_Terminal(terminal,SetT2,my_sizeof(SetT2) - ONE_LENGHT);
			//Once the process ends, the global variables must return to the original value
			g_status_td[terminal].stage = MAIN_MENU;
			g_status_td[terminal].submenu = NONE;
		}
	}
}

//Set Date
void SetDate_Start(terminal_t terminal)
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
		Print_On_Terminal(terminal,SetD1,my_sizeof(SetD1) - ONE_LENGHT);
		g_digits_counter_time = NOTHING;
		g_date_dig_counter = NOTHING;
		g_status_td[terminal].stage = SUBMENU;
		if(RTC_verify_communication())
		{
			Print_On_Terminal(terminal,I2C_Error,my_sizeof(I2C_Error) - ONE_LENGHT); //Not working well, goes recursive
			g_status_td[terminal].stage = SUBMENU_OUT;
		}
	}
}

void SetDate_Read(terminal_t terminal)
{

}

void SetDate_Save(terminal_t terminal)
{
	//Only if ENTER is pressed, save the value
	if(ASCII_ENTER == g_data_time[terminal])
	{

		//Transform to time_store_t
		g_real_date = Array_to_date(g_temp_date);
		//Save to RTC
		g_valid = RTCLOCK_write_date(g_real_date);
		//Free the device
		g_rtc_in_use = false;
		if(!g_valid)
		{
			Print_On_Terminal(terminal,Date_Error,my_sizeof(Time_Error) - ONE_LENGHT);
			SetDate_Start(terminal);
		}
		else
		{
			Print_On_Terminal(terminal,SetD2,my_sizeof(SetD2) - ONE_LENGHT);
			//Once the process ends, the global variables must return to the original value
			g_status_td[terminal].stage = MAIN_MENU;
			g_status_td[terminal].submenu = NONE;
		}

	}
}

