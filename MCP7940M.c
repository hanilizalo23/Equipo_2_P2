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

time_store_t RTCLOCK_read_time(void)
{
	uint8_t temp_read_data;
	time_store_t read_time;
	//Read seconds from RTC
	I2C_read(RTC_ADDRESS,g_address_time[SEC],RTC_SUBA_SIZE,&temp_read_data,RTC_DATA_SIZE);
	//Store
	read_time.sec = temp_read_data;
	//Read minutes from RTC
	I2C_read(RTC_ADDRESS,g_address_time[MIN],RTC_SUBA_SIZE,&temp_read_data,RTC_DATA_SIZE);
	//Store
	read_time.min = temp_read_data;
	//Read hours from RTC
	I2C_read(RTC_ADDRESS,g_address_time[HOURS],RTC_SUBA_SIZE,&temp_read_data,RTC_DATA_SIZE);
	//Store
	read_time.hour= temp_read_data;
	//Convert to real value
	read_time = RTCLOCK_bits_to_time(read_time);
	return(read_time);
}

time_store_t RTCLOCK_bits_to_time(time_store_t rtc_time)
{
	uint8_t ten, one;
	time_store_t real_time;
	//Obtain tens and units for seconds
	ten = ((rtc_time.sec & TEN_MASK) >> TEN_SHIFT);
	one = rtc_time.sec & ONE_MASK;
	real_time.sec = (ten * TEN_VALUE) + one;
	//Obtain tens and units for minutes
	ten = ((rtc_time.min & TEN_MASK) >> TEN_SHIFT);
	one = rtc_time.min & ONE_MASK;
	real_time.min = (ten * TEN_VALUE) + one;
	//Obtain tens and units for hours
	ten = ((rtc_time.hour & TEN_MASK_HR) >> TEN_SHIFT);
	one = rtc_time.hour & ONE_MASK;
	real_time.hour = (ten * TEN_VALUE) + one;
	//For format
	real_time.format = ((rtc_time.hour & FORMAT_MASK) >> FORMAT_SHIFT);
	return(real_time);
}

uint8_t RTCLOCK_write_time(time_store_t new_time)
{
	time_store_t stored_time;
	uint8_t temp_data;
	uint8_t status = true;
	//Verify if it is a valid time
	if(new_time.format)
	{
		//12h format
		if((TIME_MAX_HR_12 < new_time.hour) | (TIME_MAX_SEC_MIN < new_time.min) | (TIME_MAX_SEC_MIN < new_time.sec))
		{
			status = false;
		}
	}

	else
	{
		//24h format
		if((TIME_MAX_HR_24 < new_time.hour) | (TIME_MAX_SEC_MIN < new_time.min) | (TIME_MAX_SEC_MIN < new_time.sec))
		{
			status = false;
		}
	}

	if(status)
	{
		stored_time = RTCLOCK_time_to_bits(new_time);
		//Store on temp. variable
		temp_data = stored_time.sec;
		//Write seconds to RTC
		I2C_write(RTC_ADDRESS,g_address_time[SEC],RTC_SUBA_SIZE,&temp_data,RTC_DATA_SIZE);
		//Store on temp. variable
		temp_data = stored_time.min;
		//Write seconds to RTC
		I2C_write(RTC_ADDRESS,g_address_time[MIN],RTC_SUBA_SIZE,&temp_data,RTC_DATA_SIZE);
		//Store on temp. variable
		temp_data = stored_time.hour;
		//Write seconds to RTC
		I2C_write(RTC_ADDRESS,g_address_time[HOURS],RTC_SUBA_SIZE,&temp_data,RTC_DATA_SIZE);
	}
	return(status);
}

time_store_t RTCLOCK_time_to_bits(time_store_t real_time)
{
	uint8_t ten, one;
	time_store_t rtc_time;
	//Change tens and units to the format
	ten = real_time.sec / TEN_VALUE;
	one = real_time.sec - (ten * TEN_VALUE);
	rtc_time.sec = (ST_MASK) | ((ten << TEN_SHIFT) & TEN_MASK) | (one & ONE_MASK);
	//Change tens and units to the format
	ten = real_time.min / TEN_VALUE;
	one = real_time.min - (ten * TEN_VALUE);
	rtc_time.min = (((ten << TEN_SHIFT) & TEN_MASK) | (one & ONE_MASK));
	//Change tens and units to the format
	ten = real_time.hour / TEN_VALUE;
	one = real_time.hour - (ten * TEN_VALUE);
	rtc_time.hour = (((ten << TEN_SHIFT) & TEN_MASK_HR) | (one & ONE_MASK)) | ((real_time.format << FORMAT_SHIFT) & FORMAT_MASK);
	//For format
	rtc_time.format = real_time.format;
	return(rtc_time);
}


void Time_to_array(time_store_t time, uint8_t* new_time)
{
	new_time[0] = time.hour / TEN_VALUE;
	new_time[1] = time.hour - (new_time[0] * TEN_VALUE);
	new_time[2] = time.min / TEN_VALUE;
	new_time[3] = time.min - (new_time[2] * TEN_VALUE);
	new_time[4] = time.sec / TEN_VALUE;
	new_time[5] = time.sec - (new_time[4] * TEN_VALUE);
}

date_store_t RTCLOCK_read_date(void)
{
	uint8_t temp_read_data;
	date_store_t read_date;
	//Read the day from RTC
	I2C_read(RTC_ADDRESS,g_address_date[DAY],RTC_SUBA_SIZE,&temp_read_data,RTC_DATA_SIZE);
	//Store
	read_date.day = temp_read_data;
	//Read the month from RTC
	I2C_read(RTC_ADDRESS,g_address_date[MONTH],RTC_SUBA_SIZE,&temp_read_data,RTC_DATA_SIZE);
	//Store
	read_date.month = temp_read_data;
	//Read the year from RTC
	I2C_read(RTC_ADDRESS,g_address_date[YEAR],RTC_SUBA_SIZE,&temp_read_data,RTC_DATA_SIZE);
	//Store
	read_date.year = temp_read_data;
	//Convert to real value
	read_date = RTCLOCK_bits_to_date(read_date);
	return(read_date);
}

date_store_t RTCLOCK_bits_to_date(date_store_t rtc_date)
{
	uint8_t ten, one;
	date_store_t real_date;
	//Obtain tens and units for day
	ten = ((rtc_date.day & TEN_DATE_MASK) >> TEN_SHIFT);
	one = rtc_date.day & ONE_MASK;
	real_date.day = (ten * TEN_VALUE) + one;
	//Obtain tens and units for month
	ten = ((rtc_date.month & TEN_MTH_MASK) >> TEN_SHIFT);
	one = rtc_date.month & ONE_MASK;
	real_date.month = (ten * TEN_VALUE) + one;
	//Obtain tens and units for year
	ten = ((rtc_date.year & TEN_YEAR_MASK) >> TEN_SHIFT);
	one = rtc_date.year & ONE_MASK;
	real_date.year = (ten * TEN_VALUE) + one;
	return(real_date);
}

uint8_t RTCLOCK_write_date(date_store_t new_date)
{
	date_store_t stored_date;
	uint8_t temp_data;
	uint8_t status = true;
	//Verify if it is a valid DATE
	if((DATE_MAX_DAYS < new_date.day) | (DATE_MIN == new_date.day) | (DATE_MAX_MTH < new_date.month) | (DATE_MIN == new_date.month) | (DATE_MAX_YR < new_date.year))
	{
		status = false;
	}
	if(status)
	{
		stored_date = RTCLOCK_date_to_bits(new_date);
		//Store on temp. variable
		temp_data = stored_date.day;
		//Write date to RTC
		I2C_write(RTC_ADDRESS,g_address_date[DAY],RTC_SUBA_SIZE,&temp_data,RTC_DATA_SIZE);
		//Store on temp. variable
		temp_data = stored_date.month;
		//Write month to RTC
		I2C_write(RTC_ADDRESS,g_address_date[MONTH],RTC_SUBA_SIZE,&temp_data,RTC_DATA_SIZE);
		//Store on temp. variable
		temp_data = stored_date.year;
		//Write year to RTC
		I2C_write(RTC_ADDRESS,g_address_date[YEAR],RTC_SUBA_SIZE,&temp_data,RTC_DATA_SIZE);
	}

	return(status);
}

date_store_t RTCLOCK_date_to_bits(date_store_t real_date)
{
}




