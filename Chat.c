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

#define NOTHING 0U
#define ONE_LENGHT 1U
#define my_sizeof(type) (char *)(&type+1)-(char*)(&type) //Own function for sizeof, cause we need it

static program_status_t g_status_chat[2];
static uint8_t g_data_chat[2];


//Strings for messages
uint8_t Chat [] = "\r\nCHAT\r\n";


//Functions pointers arrays for all the stages of each submenu
void (*p_chat_pc_funcs[3])() = {PC_Chat_Start,PC_Chat_Transmit,PC_Chat_Exit};
void (*p_chat_hc05_funcs[3])() = {HC05_Chat_Start,HC05_Chat_Transmit,HC05_Chat_Exit};

program_status_t Chat_Choose(terminal_t terminal, program_status_t actual_status, uint8_t *read_data)
{

}

void PC_Chat_Transmit(void)
{

}

void PC_Chat_Exit(void)
{

}

//Bluetooth
void HC05_Chat_Start(void)
{

}

void HC05_Chat_Transmit(void)
{

}

void HC05_Chat_Exit(void)
{

}
