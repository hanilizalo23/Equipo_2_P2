/*
 * MEM24LC32A.c
 *		Device driver for memory
 *
 *  Created on: 14/04/2022
 *      Author: Mauricio Peralta
 */

#include <MEM24LC32A.h>
#include "stdint.h"
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "Bits.h"
#include "fsl_i2c.h"
#include "stdbool.h"
#include "I2C.h"
#include "Menus.h"

#define NOTHING 0U
#define ONE_LENGHT 1U
#define TENS 10U

uint8_t EEPROM_config(void)
{
	uint8_t data = 0;
	uint8_t status;
	eeprom_transfer_t test_data = {0x0000, ONE_LENGHT, &data}; //0000 as verify, not magic numbers
	//Writes 0 on the first byte to verify the connectivity the first time,
	//but also to have some data on the place that will be read on each verification
	status = EEPROM_read(test_data);
	return(status);
}

uint8_t EEPROM_write(eeprom_transfer_t transfer)
{
	uint8_t status;
	//Writes data
	status = I2C_write(EEPROM_ADDRESS,transfer.address,EEPROM_SUBA_SIZE,transfer.data,transfer.dataSize);
	return(status);
}

uint8_t EEPROM_read(eeprom_transfer_t transfer)
{
	uint8_t status;
	//Reads data
	status = I2C_read(EEPROM_ADDRESS,transfer.address,EEPROM_SUBA_SIZE,transfer.data,transfer.dataSize);
	return(status);
}

uint8_t Array_to_address(uint8_t *digs_address, uint16_t *real_address)
{
	uint8_t status = true;
	uint8_t cycle_counter = NOTHING;
	uint16_t real_address_h, real_address_l;
	//Verify if the first digit is 0
	if((ASCII_0 != digs_address[0]))
	{
		status = false;
	}
	//Verify if the second digit is x
	if((ASCII_X != digs_address[1]) && (ASCII_x_min != digs_address[1]))
	{
		status = false;
	}
	cycle_counter = 2;
	while(ADRESS_DIGS_MAX > cycle_counter)
	{
		//If it is a number
		if((ASCII_0 <= digs_address[cycle_counter]) && (ASCII_9 >= digs_address[cycle_counter]))
		{
			digs_address[cycle_counter] = digs_address[cycle_counter] - ASCII_0;
		}
		//If it is a letter
		else if((ASCII_A <= digs_address[cycle_counter]) && (ASCII_F >= digs_address[cycle_counter]))
		{
			//Adjust the value
			digs_address[cycle_counter] = digs_address[cycle_counter] - ASCII_A + HEX_LETTERS_OFFSET;
		}
		else if((ASCII_a_min <= digs_address[cycle_counter]) && (ASCII_f_min >= digs_address[cycle_counter]))
		{
			digs_address[cycle_counter] = digs_address[cycle_counter] - ASCII_a_min + HEX_LETTERS_OFFSET;
		}
		else
		{
			//Then there is at least one not valid digit
			status = false;
		}
		cycle_counter++;
	}
	//Calculate the real address
	//High part and Low part
	real_address_h = ((digs_address[2] << ADDRESS_H1_SHIFT) & ADDRESS_H1_MASK) | ((digs_address[3] << ADDRESS_H0_SHIFT) & ADDRESS_H0_MASK);
	real_address_l = ((digs_address[4] << ADDRESS_L1_SHIFT) & ADDRESS_L1_MASK) | (digs_address[5] & ADDRESS_L0_MASK);
	*real_address = real_address_h + real_address_l;
	//If the address exceeds the limit
	if(EEPROM_MAX_ADDRESS < *real_address)
	{
		status = false;
	}
	return(status);
}

uint8_t Array_to_size(uint8_t *digs, uint8_t* real_size)
{
	uint8_t status = true;
	//Verify if it is a real number
	if((ASCII_0 > digs[0]) || (ASCII_9 < digs[0]) || (ASCII_0 > digs[1]) || (ASCII_0 > digs[1]))
	{
		status = false;
	}
	*real_size = ((digs[0] - ASCII_0) * TENS) + (digs[1] - ASCII_0);
	return(status);
}


uint8_t EEPROM_verify_communication(void)
{
	uint8_t data, status;
	eeprom_transfer_t test_data = {0x0000, ONE_LENGHT, &data}; //0000 as verify, not magic numbers
	//Tries to read the memory to know if it is connected
	status = EEPROM_read(test_data);
	return(status);
}