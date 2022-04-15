/*
 * \file 	HC05.h
**		This is the source file for the HC-05 device driver. It contains the
**		functions and special macros to configure and use the bluetooth module.
**		
 *		
 *  Created on: 14/04/2022
 *      Author: Nelida Hernández
 */

#ifndef HC05_H_
#define HC05_H_

#include "fsl_uart.h"
#include "MCP7940M.h"
#include "Menus.h"

//Macros for configuration
#define BAUD_RATE_HC05 				9600U
#define UART_HC05					UART4
#define UART_CLK_FREQ_HC05 			CLOCK_GetFreq(UART4_CLK_SRC)
#define RX_RING_HC05_BUFFER_SIZE	20U
#define PRINT_DIGS1					6U
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function configures the pins needed for the HC05 transmission.
 	  	 	 In this case, RX is PTC14 and TX is PTC15. It also configures the UART4
 	  	 	 with the default configuration.
 	 \param[in]  void
 	 \return void
 */
void HC05_configure_port(void);
/*!
 	 \brief	 This function transmits data to the HC05.
 	 \param[in]  const uint8_t *data, size_t length (of the data)
 	 \return void
 */
void HC05_write(uint8_t *data, size_t length);
/*!
 	 \brief	 This function allows to read data transmited data through the serial port
 	 from the PC.
 	 \param[in]  const uint8_t *data, size_t length (of the data)
 	 \return returns 'true' if a value has been read, or false if not
 */
void HC05_read(uint8_t *data, size_t length);
/*!
 	 \brief	 This function returns the interruption flag to know if something has been
 	 	 	 read from the UART.
 	 \param[in]  void
 	 \return returns 'true' if a value has been read, or 'false' if not
 */
uint8_t HC05_get_flag(void);
/*!
 	 \brief	 This function cleans the interruption flag.
 	 \param[in]  void
 	 \return void
 */
void HC05_clear_flag(void);
/*!
 	 \brief	 This function prints a date on the UART in the format hh:mm:ss
 	 \param[in]  time_store_t time = Variable where the time is saved
 	 \return void
 */
void HC05_print_time(time_store_t time);
/*!
 	 \brief	 This function prints a date on the UART in the format dd/mm/yy
 	 \param[in]  uint8_t* date = Variable where the date is saved
 	 \return void
 */
void HC05_print_date(date_store_t date);
#endif /* HC05_H_ */
