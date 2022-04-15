/*
 * MCP7940M.h
 *		Device driver for the Real-Time Clock/Calendar with SRAM
 *
 *  Created on: 14/04/2022
 *      Author: Mauricio Peralta
 */

#ifndef MCP7940M_H_
#define MCP7940M_H_

#include "MK64F12.h"
//For I2C
#define RTC_ADDRESS			   		0x6FU
#define WAIT_TIME                   10U
#define RTC_SUBA_SIZE				1U
#define RTC_DATA_SIZE				1U
//Digits of time
#define TIME_TOTAL_DIGS				6U
//Limits for time
#define TIME_MAX_HR_12				12U
#define TIME_MAX_HR_24				23U
#define TIME_MAX_SEC_MIN			59U
//Limits for date
#define DATE_MIN					0U
#define DATE_MAX_DAYS				30U
#define DATE_MAX_MTH				12U
#define DATE_MAX_YR					99U
//Masks and shifts to transform
#define ST_MASK						0x80U
#define TEN_SHIFT					4U
#define TEN_MASK					0x70
#define ONE_MASK					0xF
#define TEN_MASK_HR					0x30
#define TEN_VALUE					10U

#define TEN_DATE_MASK				0x30U


//Enumerations
typedef enum{
	SEC,
	MIN,
	HOURS
}time_reg_t;

typedef enum{
	DAY,
	MONTH,
	YEAR
}date_reg_t;

typedef enum{
	ONE,
	TEN
}time_dig_t;

typedef enum{
	AM,
	PM
}time_am_pm_t;

typedef enum{
	TWENTY_FOUR,
	TWELVE
}time_format_t;

//New type of data (structures)
//To store time
typedef struct
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t format;
}time_store_t;


/*!
 	 \brief	 This function configures the RTC with initial date and time values and starts the oscillator
 	 \param[in]  void
 	 \return true if the k64 could not communicate with the RTC
 */
uint8_t RTC_config(void);
/*!
 	 \brief	 This function reads a part of the RTC in order to verify the connectivity between the board and
 	 	 	 the RTC.
 	 \param[in]  void
 	 \return true if the k64 had problems communicating with the RTC
 */
uint8_t RTC_verify_communication(void);
/*!
 	 \brief	 This function reads the time stored on the RTC
 	 \param[in]  void
 	 \return time read
 */
time_store_t RTCLOCK_read_time(void);
/*!
 	 \brief	 This function converts the time on the format needed to be stored on the RTC to normal
 	 	 	 	(decimal).
 	 \param[in]  real_time = variable where the time on RTC format is stored
 	 \return the conversion
 */
time_store_t RTCLOCK_bits_to_time(time_store_t rtc_time);
/*!
 	 \brief	 This function converts a type time_store_t to an array with the time digits.
 	 \param[in]  time = variable that stores the time, new_time = array where the digits' values
 	 	 	 	 will be stored.
 	 \return void.
 */
void Time_to_array(time_store_t time,uint8_t* new_time);
#endif /* MCP7940M_H_ */
