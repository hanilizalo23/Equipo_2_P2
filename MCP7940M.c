/*
 * MCP7940M.c
 *		Device driver for the Real-Time Clock/Calendar with SRAM
 *
 *  Created on: 14/04/2022
 *      Author: Mauricio Peralta
 */

#include "MCP7940M.h"
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

//Addresses for time and date registers
const uint8_t g_address_time[3] =
{
		0x00U,
		0x01U,
		0x02U
};
const uint8_t g_address_date[3] =
{
		0x04U,
		0x05U,
		0x06U
};

uint8_t RTC_config(void)
{
	uint8_t temp_data = ST_MASK;
	uint8_t status;
	time_store_t time_start_system = {00, 00, 00, 00}; //Direct numbers, not magic numbers
	date_store_t date_start_system = {1,1,00}; //Direct numbers, not magic numbers
	I2C_config();
	//Start 37.768 kHz oscillator
	status = I2C_write(RTC_ADDRESS,g_address_time[0],RTC_SUBA_SIZE,&temp_data,RTC_DATA_SIZE);
	//Sets initial time on 00:00:00, 24hour format
	RTCLOCK_write_time(time_start_system);
	RTCLOCK_write_date(date_start_system);
	return(status);
}

uint8_t RTC_verify_communication(void)
{
	uint8_t data_verif, status;
	status = I2C_read(RTC_ADDRESS,g_address_time[0],RTC_SUBA_SIZE,&data_verif,RTC_DATA_SIZE);
	return(status);
}

