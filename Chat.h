/*
 * \file 	Chat.h
**		This source file includes the functions to access the chat function
**		on the program.
**		
 *		
 *  Created on: 14/04/2022
 *      Author: Nelida Hernández
 */

#ifndef CHAT_H_
#define CHAT_H_

#include "MK64F12.h"

#define MAX_TEXT_CHAT	(100U)

//Variable to store messages
typedef struct
{
	uint8_t text[MAX_TEXT_CHAT];
	uint8_t length;
}message_t;

/*!
 	 \brief	 This function chooses which function to do according to the program stage.
 	 \param[in]  terminal to use, actual status (stage and submenu) and data read from terminal.
 	 \return void
 */
program_status_t Chat_Choose(terminal_t terminal,program_status_t actual_status, uint8_t *read_data);
/*!
 	 \brief	 This group of functions are the stages of the submenu "Chat" for the UART0.
 	 \param[in]  terminal to use.
 	 \return void
 */
void PC_Chat_Start(void);
void PC_Chat_Transmit(void);
void PC_Chat_Exit(void);
/*!
 	 \brief	 This group of functions are the stages of the submenu "Chat" for the UART4.
 	 \param[in]  terminal to use.
 	 \return void
 */
void HC05_Chat_Start(void);
void HC05_Chat_Transmit(void);
void HC05_Chat_Exit(void);
#endif /* CHAT_H_ */
