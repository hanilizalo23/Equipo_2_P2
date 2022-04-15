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
 	 \brief	 This function writes the given time on the RTC
 	 \param[in]  new_time = the time to be stored
 	 \return true if the time was valid
 */
