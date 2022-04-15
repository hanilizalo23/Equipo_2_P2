
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
#include "Menus.h"

#define NOTHING 0U
#define ONE_LENGHT 1U
#define my_sizeof(type) (char *)(&type+1)-(char*)(&type) //Own function for my_sizeof, cause we need it

static program_status_t g_status_mem[2];
static uint8_t g_data_mem[2];
static uint8_t g_digits_counter;
static uint8_t g_mem_counter;
static uint8_t g_temp_address[ADRESS_DIGS_MAX];

//Only 2 digits, max. 99
static uint8_t g_temp_size_mem[DATA_SIZE_DIGS];
static uint8_t g_temp_data_mem[DATA_SIZE_MAX];
static uint16_t g_real_address;
static uint8_t g_real_data_size;
static uint8_t g_valid_address;
static eeprom_transfer_t transference_mem;

//Strings for messages
uint8_t WriteM1	[] = "\r\nDir. de escritura (en hexadecimal con formato 0x0000): ";
uint8_t WriteM2	[] = "\r\nLongitud en bytes (en decimal, dos dig.): ";
uint8_t WriteM3	[] = "\r\nTexto a guardar:\r\n";
uint8_t WriteM4	[] = "\r\nSu texto ha sido guardado...\r\n";
uint8_t ReadM1	[] = "\r\nDir. de lectura (en hexadecimal con formato 0x0000): ";
uint8_t ReadM2	[] = "\r\nLongitud en bytes: ";
uint8_t ReadM3	[] = "\r\nContenido:\r\n";
uint8_t ReadM4	[] = "\r\nPresione una tecla para continuar...\r\n";
uint8_t I2C_in_use0 [] = "\r\nDispositivo en uso por otra terminal\r\n";
uint8_t I2C_Error0 [] = "\r\nERROR: No se pudo comunicar con el dispositivo I2C\r\n";
uint8_t Press_ESC0 [] = "\r\n\nPresione ESC para salir\r\n";
uint8_t Mem_Error [] = "\r\nIngrese una dir. de memoria accesible: ";
uint8_t Mem_Error1 [] = "\r\nIngrese una longitud correcta: ";

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

	switch(g_mem_counter)
	{
	case ADDRESS:
		if((ASCII_ENTER == g_data_mem[terminal]) && (ADRESS_DIGS_MAX == g_digits_counter))
		{
			//Restart digits counter
			g_digits_counter = NOTHING;
			//Convert to real address and verify if it is valid
			g_valid_address = Array_to_address(g_temp_address,&g_real_address);
			if(g_valid_address)
			{
				g_mem_counter = LENGTH;
				Print_On_Terminal(terminal,WriteM2,my_sizeof(WriteM2) - ONE_LENGHT);
			}
			else
			{
				Print_On_Terminal(terminal,Mem_Error,my_sizeof(Mem_Error) - ONE_LENGHT);
			}
		}
		else if(ADRESS_DIGS_MAX > g_digits_counter)
		{
			//Save value on temporal variable
			g_temp_address[g_digits_counter] = g_data_mem[terminal];
			Print_On_Terminal(terminal,&g_data_mem[terminal],ONE_LENGHT);
			g_digits_counter++;
		}
		break;

	case LENGTH:
		if((ASCII_ENTER == g_data_mem[terminal]) && (DATA_SIZE_DIGS == g_digits_counter))
		{
			//Restart digits counter
			g_digits_counter = NOTHING;
			//Convert to real number
			g_valid_address = Array_to_size(g_temp_size_mem,&g_real_data_size);
			if(g_valid_address)
			{
				g_mem_counter = TEXT;
				Print_On_Terminal(terminal,WriteM3,my_sizeof(WriteM3) - ONE_LENGHT);
			}
			else
			{
				Print_On_Terminal(terminal,Mem_Error1,my_sizeof(Mem_Error1) - ONE_LENGHT);
			}

		}
		else if(DATA_SIZE_DIGS > g_digits_counter)
		{
			//Save value
			Print_On_Terminal(terminal,&g_data_mem[terminal],ONE_LENGHT);
			g_temp_size_mem[g_digits_counter] = g_data_mem[terminal];
			g_digits_counter++;
		}
		break;

	default:
		if((ASCII_ENTER == g_data_mem[terminal]) && (g_real_data_size == g_digits_counter))
		{
			//Save value on special variable
			g_status_mem[terminal].stage = SUBMENU_OUT;
			//Write on memory
			transference_mem.address = g_real_address;
			transference_mem.dataSize = g_real_data_size;
			transference_mem.data = g_temp_data_mem;
			EEPROM_write(transference_mem);
			Print_On_Terminal(terminal,WriteM4,my_sizeof(WriteM4) - ONE_LENGHT);
			Print_On_Terminal(terminal,Press_ESC0,my_sizeof(Press_ESC0) - ONE_LENGHT);
		}
		else if(g_real_data_size > g_digits_counter)
		{
			//Save text
			g_temp_data_mem[g_digits_counter] = g_data_mem[terminal];
			Print_On_Terminal(terminal,&g_data_mem[terminal],ONE_LENGHT);
			g_digits_counter++;
		}
		break;
	}
}

void WriteMem_Exit(terminal_t terminal)
{
	if(ASCII_ESC == g_data_mem[terminal])
	{
		//Once the process ends, the global variables must return to the original value
		g_status_mem[terminal].stage = MAIN_MENU;
		g_status_mem[terminal].submenu = NONE;
		//Free the device
		g_mem_in_use = false;
	}
}

//Read Memory
void ReadMem_Start(terminal_t terminal)
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
			Print_On_Terminal(terminal,ReadM1,my_sizeof(ReadM1) - ONE_LENGHT);
			g_digits_counter = 0;
			g_status_mem[terminal].stage = SUBMENU;
		}
	}
}

void ReadMem_Write(terminal_t terminal)
{
	switch(g_mem_counter)
		{
		case ADDRESS:
			if((ASCII_ENTER == g_data_mem[terminal]) && (ADRESS_DIGS_MAX == g_digits_counter))
			{
				//Restart digits counter
				g_digits_counter = NOTHING;
				//Convert to real address and verify if it is valid
				g_valid_address = Array_to_address(g_temp_address,&g_real_address);
				if(g_valid_address)
				{
					g_mem_counter = LENGTH;
					Print_On_Terminal(terminal,ReadM2,my_sizeof(ReadM2) - ONE_LENGHT);
				}
				else
				{
					Print_On_Terminal(terminal,Mem_Error,my_sizeof(Mem_Error) - ONE_LENGHT);
				}
			}
			else if(ADRESS_DIGS_MAX > g_digits_counter)
			{
				//Save value on temporal variable
				g_temp_address[g_digits_counter] = g_data_mem[terminal];
				Print_On_Terminal(terminal,&g_data_mem[terminal],ONE_LENGHT);
				g_digits_counter++;
			}
			break;

		case LENGTH:
			if((ASCII_ENTER == g_data_mem[terminal]) && (DATA_SIZE_DIGS == g_digits_counter))
			{
				//Restart digits counter
				g_digits_counter = NOTHING;
				//Convert to real number
				g_valid_address = Array_to_size(g_temp_size_mem,&g_real_data_size);
				if(g_valid_address)
				{
					g_mem_counter = TEXT;
					Print_On_Terminal(terminal,ReadM3,my_sizeof(ReadM3) - ONE_LENGHT);
					g_status_mem[terminal].stage = SUBMENU_OUT;
					//Read from memory
					transference_mem.address = g_real_address;
					transference_mem.dataSize = g_real_data_size;
					transference_mem.data = g_temp_data_mem;
					EEPROM_read(transference_mem);
					Print_On_Terminal(terminal,g_temp_data_mem,g_real_data_size);
					Print_On_Terminal(terminal,Press_ESC0,my_sizeof(Press_ESC0) - ONE_LENGHT);
				}
				else
				{
					Print_On_Terminal(terminal,Mem_Error1,my_sizeof(Mem_Error1) - ONE_LENGHT);
				}

			}
			else if(DATA_SIZE_DIGS > g_digits_counter)
			{
				//Save value
				Print_On_Terminal(terminal,&g_data_mem[terminal],ONE_LENGHT);
				g_temp_size_mem[g_digits_counter] = g_data_mem[terminal];
				g_digits_counter++;
			}
			break;

		default:
			break;
		}

}

void ReadMem_Exit(terminal_t terminal)
{
	//Only if ESC is pressed, exit
	if(ASCII_ESC == g_data_mem[terminal])
	{
		//Once the process ends, the global variables must return to the original value
		g_status_mem[terminal].stage = MAIN_MENU;
		g_status_mem[terminal].submenu = NONE;
		//Free the device
		g_mem_in_use = false;
	}
}
