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
}