
/*
 * memory.c
 *		Device driver for memory
 *		
 *  Created on: 14/04/2022
 *      Author: Mauricio Peralta
 */

#include <MEM24LC32A.h>
#include "Memory.h"
#include "MK64F12.h"
#include "stdbool.h"
#include "fsl_uart.h"
#include "Bits.h"
#include "HC05.h"
#include "UART_PC.h"
#include "I2C.h"
#include <stdio.h>


#define NOTHING 0U
#define ONE_LENGHT 1U
#define my_sizeof(type) (char *)(&type+1)-(char*)(&type) 

static program_status_t g_status_mem[2];
static uint8_t g_data_mem[2];
static uint8_t g_digits_counter;
static uint8_t g_mem_counter;
static uint8_t g_temp_address[ADRESS_DIGS_MAX];

static uint8_t g_temp_size_mem[DATA_SIZE_DIGS];
static uint8_t g_temp_data_mem[DATA_SIZE_MAX];
static uint16_t g_real_address;


//Strings for messages
uint8_t WriteM1	[] = "\r\nDir. de escritura (en hexadecimal con formato 0x0000): ";
uint8_t WriteM2	[] = "\r\nLongitud en bytes (en decimal, dos dig.): ";
uint8_t WriteM3	[] = "\r\nTexto a guardar:\r\n";
uint8_t WriteM4	[] = "\r\nSu texto ha sido guardado...\r\n";
uint8_t ReadM1	[] = "\r\nDir. de lectura (en hexadecimal con formato 0x0000): ";
uint8_t ReadM2	[] = "\r\nLongitud en bytes: ";
uint8_t ReadM3	[] = "\r\nContenido:\r\n";
uint8_t ReadM4	[] = "\r\nPresione una tecla para continuar...\r\n";


uint8_t test_text [] = "\r\nWORKING\r\n";

//Flags for "I2C device in use"
static uint8_t g_mem_in_use = false;

//Functions pointers arrays for all the stages of each submenu
void (*p_writemem_pc_funcs[3])(terminal_t terminal) = {WriteMem_Start,WriteMem_Read,WriteMem_Exit};
void (*p_readmem_pc_funcs[3])(terminal_t terminal) = {ReadMem_Start,ReadMem_Write,ReadMem_Exit};

program_status_t Memory_Choose(terminal_t terminal,program_status_t actual_status, uint8_t *read_data)
{
	uint8_t submenu_stage;
	g_status_mem[terminal] = actual_status;
	submenu_stage = actual_status.stage - ONE_LENGHT;
	g_data_mem[terminal] = *read_data;
	if(WRITE_MEM == actual_status.submenu)
	{
		p_writemem_pc_funcs[submenu_stage](terminal);
	}
	else
	{
		p_readmem_pc_funcs[submenu_stage](terminal);
	}
	return(g_status_mem[terminal]);
}

//Write Memory
void WriteMem_Start(terminal_t terminal)
{
	if(g_mem_in_use)
	{
		Print_On_Terminal(terminal,I2C_in_use0,my_sizeof(I2C_in_use0) - ONE_LENGHT);
		g_status_mem[terminal].stage = MAIN_MENU;
		g_status_mem[terminal].submenu = NONE;
	}
	else
	{
		if(EEPROM_verify_communication())
		{
			Print_On_Terminal(terminal,I2C_Error0,my_sizeof(I2C_Error0) - ONE_LENGHT);
			g_status_mem[terminal].stage = SUBMENU_OUT;
		}
		else
		{
			g_mem_counter = ADDRESS;
			g_mem_in_use = true;
			//Depending on what must read next, prints a different message
			Print_On_Terminal(terminal,WriteM1,my_sizeof(WriteM1) - ONE_LENGHT);
			g_digits_counter = NOTHING;
			g_status_mem[terminal].stage = SUBMENU;
		}
	}
}
void WriteMem_Read(terminal_t terminal)
{
}

