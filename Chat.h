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

#include "Menus.h"
#include "MK64F12.h"

#define MAX_TEXT_CHAT	(100U)


/*!
 	 \brief	 This function chooses which function to do according to the program stage.
 	 \param[in]  terminal to use, actual status (stage and submenu) and data read from terminal.
 	 \return void
 */
program_status_t Chat_Choose(terminal_t terminal,program_status_t actual_status, uint8_t *read_data);

#endif /* CHAT_H_ */
