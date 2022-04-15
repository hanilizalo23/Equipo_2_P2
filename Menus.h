/*
 * \file 	Menus.h
 *		This source file includes the functions to access to each menu/submenu
**		of the program
 *		
 *  Created on: 14/04/2022
 *      Author: Nelida Hernández
 */

#ifndef MENUS_H_
#define MENUS_H_

#include "MK64F12.h"


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function configures both UART terminals, the RTC and the memory.
 	 \param[in]  void
 	 \return void
 */
uint8_t ConfigureAll(void);
/*!
 	 \brief	 This function read both terminals and verify that the reading is valid
 	 	 	 and the program flow must continue.
 	 \param[in]  void
 	 \return true if the flow can continue, false to stop the program and continue reading
 */
flow_flags_t ReadTerminals(void);
/*!
 	 \brief	 This function choose what to do depending on the stage of the program of the
 	 	 	 specified terminal.
 	 \param[in]  terminal to use
 	 \return void
 */
void ChooseStage(terminal_t terminal);
/*!
 	 \brief	 This function choose what to do depending on the submenu of the program of the
 	 	 	 specified terminal.
 	 \param[in]  terminal to use
 	 \return void
 */
void ChooseSubmenu(terminal_t terminal);
/*!
 	 \brief	 This function displays the start menu on the selected terminal.
 	 \param[in]  terminal to use
 	 \return void
 */
void Start_Menu(terminal_t terminal);
/*!
 	 \brief	 This function calls the function that chooses what to do for the submenus that
 	 	 	 use time or date.
 	 \param[in]  terminal to use
 	 \return void
 */
void TimeDate_Menu(terminal_t terminal);
/*!
 	 \brief	 This function calls the function that chooses what to do for the submenus that
 	 	 	 use memory.
 	 \param[in]  terminal to use
 	 \return void
 */
void Memory_Menu(terminal_t terminal);
/*!
 	 \brief	 This function calls the function that chooses what to do for the submenu chat.
 	 \param[in]  terminal to use
 	 \return void
 */
void Chat_Menu(terminal_t terminal);
/*!
 	 \brief	 This function prints on the selected terminal the given data.
 	 \param[in]  terminal to use
 	 \return void
 */
void Print_On_Terminal(terminal_t terminal, uint8_t *data, size_t length);
#endif /* MENUS_H_ */
