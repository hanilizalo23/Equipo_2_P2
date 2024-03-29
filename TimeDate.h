/*
 * \file 	TimeDate.h
 *		This source file includes the functions needed to set and read hour and date
**		from both terminals.
 *		
 *  Created on: 14/04/2022
 *      Author: Mauricio Peralta
 */

#ifndef TIMEDATE_H_
#define TIMEDATE_H_

#include "MK64F12.h"
#include "Menus.h"

//Total of digits for both time and date
#define TIME_DIGITS			(8U)

/*!
 	 \brief	 This function chooses which function to do according to the submenu and program stage.
 	 \param[in]  terminal to use, actual status (stage and submenu) and data read from terminal.
 	 \return void
 */
program_status_t TimeDate_Choose(terminal_t terminal, program_status_t actual_status, uint8_t *read_data);
/*!
 	 \brief	 This group of functions are the stages of the submenu "Set time".
 	 \param[in]  terminal to use.
 	 \return void
 */
void SetTime_Start(terminal_t terminal);
void SetTime_Read(terminal_t terminal);
void SetTime_Save(terminal_t terminal);
/*!
 	 \brief	 This group of functions are the stages of the submenu "Set date".
 	 \param[in]  terminal to use.
 	 \return void
 */
void SetDate_Start(terminal_t terminal);
void SetDate_Read(terminal_t terminal);
void SetDate_Save(terminal_t terminal);
/*!
 	 \brief	 This group of functions are the stages of the submenu "Read time".
 	 \param[in]  terminal to use.
 	 \return void
 */
void ReadTime_Start(terminal_t terminal);
void ReadTime_Write(terminal_t terminal);
void ReadTime_Exit(terminal_t terminal);
/*!
 	 \brief	 This group of functions are the stages of the submenu "Read date".
 	 \param[in]  terminal to use.
 	 \return void
 */
void ReadDate_Start(terminal_t terminal);
void ReadDate_Write(terminal_t terminal);
void ReadDate_Exit(terminal_t terminal);

#endif /* TIMEDATE_H_ */
