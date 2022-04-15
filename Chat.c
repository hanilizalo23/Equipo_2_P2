/*
 * \file 	Chat.c
**		This source file includes the functions to access the chat function
**		on the program.
**		
 *		
 *  Created on: 14/04/2022
 *      Author: Nelida Hernández
 */

#include "Chat.h"
#include "stdbool.h"
#include "fsl_uart.h"
#include "Bits.h"
#include "HC05.h"
#include "UART_PC.h"
#include <stdio.h>

#define NOTHING 0U
#define ONE_LENGHT 1U
#define my_sizeof(type) (char *)(&type+1)-(char*)(&type) //Own function for sizeof, cause we need it

static program_status_t g_status_chat[2];
static uint8_t g_data_chat[2];
static uint8_t g_t_connected[2];
static message_t g_chat_message[2];
static uint8_t g_first_in[2] = {true};

//Strings for messages
uint8_t Chat [] = "\r\nCHAT\r\n";
uint8_t Message [] = "\r\n       Mensaje: ";
uint8_t Allignment [] = "\r\n                    ";
uint8_t Allignment0 [] ="\r\n";
uint8_t Term1 [] = "Terminal 1:\r";
uint8_t Term2 [] = "Terminal 2:\r";
uint8_t Term1Disc [] = 	"\r\n-------Terminal 1 desconectada-------\r\n";
uint8_t Term2Disc [] = 	"\r\n-------Terminal 2 desconectada-------\r\n";
uint8_t TermWaiting [] = "\r\n       Esperando a la otra terminal\r\n";
uint8_t Term1Con [] = 	"\r\n-------Terminal 1 conectada----------\r\n";
uint8_t Term2Con [] = 	"\r\n-------Terminal 2 conectada----------\r\n";
uint8_t Press_ESC1 [] = "\r\nPresione ESC para salir\r\n";

//Functions pointers arrays for all the stages of each submenu
void (*p_chat_pc_funcs[3])() = {PC_Chat_Start,PC_Chat_Transmit,PC_Chat_Exit};
void (*p_chat_hc05_funcs[3])() = {HC05_Chat_Start,HC05_Chat_Transmit,HC05_Chat_Exit};

program_status_t Chat_Choose(terminal_t terminal, program_status_t actual_status, uint8_t *read_data)
{
	uint8_t submenu_stage;
	g_status_chat[terminal] = actual_status;
	submenu_stage = actual_status.stage - ONE_LENGHT;
	g_data_chat[terminal] = *read_data;
	if(PC == terminal)
	{
		p_chat_pc_funcs[submenu_stage]();
	}
	else
	{
		p_chat_hc05_funcs[submenu_stage]();
	}
	return(g_status_chat[terminal]);
}

//Chat
void PC_Chat_Start(void)
{
	//If the other terminal is connected, then start reading
	g_t_connected[PC] = true;
	if(g_t_connected[HC05])
	{
		g_status_chat[PC].stage = SUBMENU;
		UART_PC_write(Term2Con,my_sizeof(Term2Con) - ONE_LENGHT);
		UART_PC_write(Chat,my_sizeof(Chat) - ONE_LENGHT);
		g_status_chat[PC].continue_flow = false;
		g_chat_message[PC].length = NOTHING;
	}

	//If the other terminal is not connected, then show message
	else
	{
		//To only print the messages once
		if(g_first_in[PC])
		{
			UART_PC_write(Term2Disc,my_sizeof(Term2Disc) - ONE_LENGHT);
			UART_PC_write(TermWaiting,my_sizeof(TermWaiting) - ONE_LENGHT);
			UART_PC_write(Press_ESC1,my_sizeof(Press_ESC1) - ONE_LENGHT);
			g_first_in[PC] = false;
			g_status_chat[PC].continue_flow = true;
		}
		//If ESC is pressed, then exit
		if(ASCII_ESC == g_data_chat[PC])
		{
			PC_Chat_Exit();
		}
	}
}

void PC_Chat_Transmit(void)
{
	//If ESC, print in both terminals "Terminal 1:"
	//Go to next stage
	//Else store message
	if(ASCII_ENTER == g_data_chat[PC])
	{
		//Print messages in both terminals
		UART_PC_write(Allignment,my_sizeof(Allignment) - ONE_LENGHT);
		UART_PC_write(Term1,my_sizeof(Term1) - ONE_LENGHT);
		UART_PC_write(Allignment,my_sizeof(Allignment) - ONE_LENGHT);
		UART_PC_write(g_chat_message[PC].text,g_chat_message[PC].length);

		HC05_write(Allignment0,my_sizeof(Allignment0) - ONE_LENGHT);
		HC05_write(Term1,my_sizeof(Term1) - ONE_LENGHT);
		HC05_write(Allignment0,my_sizeof(Allignment0) - ONE_LENGHT);
		HC05_write(g_chat_message[PC].text,g_chat_message[PC].length);
		g_status_chat[PC].stage = SUBMENU_OUT;
		g_chat_message[PC].length = NOTHING;
		PC_Chat_Exit();
	}

	else
	{
		if(0 == g_chat_message[PC].length)
		{
			UART_PC_write(Message,my_sizeof(Message) - ONE_LENGHT);
		}
		g_chat_message[PC].text[g_chat_message[PC].length] = g_data_chat[PC];
		UART_PC_write(&g_chat_message[PC].text[g_chat_message[PC].length],ONE_LENGHT);
		g_chat_message[PC].length ++;
		PC_Chat_Exit();
	}
}

void PC_Chat_Exit(void)
{
	//Only if ESC is pressed, exit
	if(ASCII_ESC == g_data_chat[PC])
	{
		//Once the process ends, the global variables must return to the original value
		g_status_chat[PC].stage = MAIN_MENU;
		g_status_chat[PC].submenu = NONE;
		g_t_connected[PC] = false;

		if(g_t_connected[HC05])
		{
			//print "Terminal 1 se desconectó"
			HC05_write(Term1Disc,my_sizeof(Term1Disc) - ONE_LENGHT);
		}
		g_first_in[PC] = true;
	}

	else
	{
		//Go back to reading
		g_status_chat[PC].stage = SUBMENU;
	}
}

//Bluetooth
void HC05_Chat_Start(void)
{
	//If the other terminal is connected, then start reading
	g_t_connected[HC05] = true;
	if(g_t_connected[PC])
	{
		g_status_chat[HC05].stage = SUBMENU;
		HC05_write(Term1Con,my_sizeof(Term1Con) - ONE_LENGHT);
		HC05_write(Chat,my_sizeof(Chat) - ONE_LENGHT);
		g_status_chat[HC05].continue_flow = false;
		g_chat_message[HC05].length = NOTHING;
	}

	//If the other terminal is not connected, then show message
	else
	{
		//To only print the messages once
		if(g_first_in[HC05])
		{
			HC05_write(Term1Disc,my_sizeof(Term1Disc) - ONE_LENGHT);
			UART_PC_write(TermWaiting,my_sizeof(TermWaiting) - ONE_LENGHT);
			UART_PC_write(Press_ESC1,my_sizeof(Press_ESC1) - ONE_LENGHT);
			g_first_in[HC05] = false;
			g_status_chat[HC05].continue_flow = true;
		}

		//If ESC is pressed, then exit
		if(ASCII_ESC == g_data_chat[HC05])
		{
			HC05_Chat_Exit();
		}
	}
}

void HC05_Chat_Transmit(void)
{
	//If ESC, print in both terminals "Terminal 1:"
	//Go to next stage
	//Else store message
	if(ASCII_ENTER == g_data_chat[HC05])
	{
		//Print messages in both terminals
		HC05_write(Allignment,my_sizeof(Allignment) - ONE_LENGHT);
		HC05_write(Term2,my_sizeof(Term2) - ONE_LENGHT);
		HC05_write(Allignment,my_sizeof(Allignment) - ONE_LENGHT);
		HC05_write(g_chat_message[HC05].text,g_chat_message[HC05].length);

		UART_PC_write(Allignment0,my_sizeof(Allignment0) - ONE_LENGHT);
		UART_PC_write(Term2,my_sizeof(Term2) - ONE_LENGHT);
		UART_PC_write(Allignment0,my_sizeof(Allignment0) - ONE_LENGHT);
		UART_PC_write(g_chat_message[HC05].text,g_chat_message[HC05].length);
		g_status_chat[HC05].stage = SUBMENU_OUT;
		g_chat_message[HC05].length = NOTHING;
		PC_Chat_Exit();
	}

	else
	{
		if(0 == g_chat_message[HC05].length)
		{
			HC05_write(Message,my_sizeof(Message) - ONE_LENGHT);
		}
		g_chat_message[HC05].text[g_chat_message[HC05].length] = g_data_chat[HC05];
		HC05_write(&g_chat_message[HC05].text[g_chat_message[HC05].length],ONE_LENGHT);
		g_chat_message[HC05].length ++;
		PC_Chat_Exit();
	}
}

void HC05_Chat_Exit(void)
{
	//Only if ESC is pressed, exit
	if(ASCII_ESC == g_data_chat[HC05])
	{
		//Once the process ends, the global variables must return to the original value
		g_status_chat[HC05].stage = MAIN_MENU;
		g_status_chat[HC05].submenu = NONE;
		g_t_connected[HC05] = false;
		if(g_t_connected[PC])
		{
			//Print message of terminal disconnected
			UART_PC_write(Term2Disc,my_sizeof(Term2Disc) - ONE_LENGHT);
		}
		g_first_in[HC05] = true;
	}

	else
	{
		//Go back to reading
		g_status_chat[HC05].stage = SUBMENU;
	}
}
