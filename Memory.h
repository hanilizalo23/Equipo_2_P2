
/*
 * memory.h
 *		Device driver for memory
 *		
 *  Created on: 14/04/2022
 *      Author: Mauricio Peralta
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include "MK64F12.h"
#include "Menus.h"

//To store memory data for transferences
typedef enum
{
	ADDRESS,
	LENGTH,
	TEXT
}mem_data_t;

/*!
 	 \brief	 This function chooses which function to do according to the submenu and program stage.
 	 \param[in]  terminal to use, actual status (stage and submenu) and data read from terminal.
 	 \return void
 */
program_status_t Memory_Choose(terminal_t terminal, program_status_t actual_status, uint8_t *read_data);
/*!
 	 \brief	 This group of functions are the stages of the submenu "Write memory".
 	 \param[in]  terminal to use.
 	 \return void
 */
void WriteMem_Start(terminal_t terminal);
void WriteMem_Read(terminal_t terminal);
void WriteMem_Exit(terminal_t terminal);

#endif /* MEMORY_H_ */
