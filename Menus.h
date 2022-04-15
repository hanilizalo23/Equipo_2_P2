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

#define ASCII_NULL		(0U)
#define ASCII_0 		(48U)
#define ASCII_9			(57U)
#define ASCII_A			(65U)
#define ASCII_a_min		(97U)
#define ASCII_F			(70U)
#define ASCII_f_min		(102U)
#define ASCII_X			(88U)
#define ASCII_x_min		(120U)
#define ASCII_ENTER		(13U)
#define ASCII_ESC		(27U)
#define ASCII_COLON		(58U)
#define ASCII_SLASH		(47U)

typedef enum
{
	PC,
	HC05
}terminal_t;

typedef enum
{
	MAIN_MENU,
	CHOOSE_OPT,
	SUBMENU,
	SUBMENU_OUT
}program_stages_t;

typedef enum
{
	NONE,
	SET_TIME,
	SET_DATE,
	READ_TIME,
	READ_DATE,
	WRITE_MEM,
	READ_MEM,
	CHAT,
	LED_MATRIX
}program_submenus_t;

typedef struct
{
	uint8_t t1_flag;
	uint8_t t2_flag;
}flow_flags_t;

typedef struct
{
	program_stages_t stage;
	program_submenus_t submenu;
	uint8_t continue_flow;
}program_status_t;

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
