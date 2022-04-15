/*
 * \file 	HC05.c
**		This is the source file for the HC-05 device driver. It contains the
**		functions and special macros to configure and use the bluetooth module.
**		
 *		
 *  Created on: 14/04/2022
 *      Author: Nelida Hernández
 */

#include "HC05.h"
#include "stdio.h"
#include "MK64F12.h"
#include "stdbool.h"
#include "fsl_uart.h"
#include "fsl_port.h"
#include "Bits.h"

#define NOTHING 0U
#define ONE_LENGHT 1U

uart_handle_t g_uartPCHandle_hc05;
static size_t g_receivedBytes_hc05;
static uint8_t g_rxRingBuffer_hc05[RX_RING_HC05_BUFFER_SIZE] = {NOTHING}; /*RX ring buffer*/
static uart_transfer_t g_rxbuffer_hc05;

//Flag to know when a character has been received
volatile uint8_t g_rx_flag_hc05 = false;

void HC05_callback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData)
{
	if(kStatus_UART_RxIdle == status)
	{
		//The flag value changes when the interruption enters
		g_rx_flag_hc05 = true;
	}
}

void HC05_configure_port(void)
{
	uart_config_t uart_config;

	/* Port C Clock Gate Control: Clock enabled */
	CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC14 (pin 86) is configured as UART4_RX */
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt3);

    /* PORTC15 (pin 87) is configured as UART4_TX */
    PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt3);

	//Get default configuration for UART
	UART_GetDefaultConfig(&uart_config);
	uart_config.baudRate_Bps = BAUD_RATE_HC05;
	uart_config.enableTx = true;
	uart_config.enableRx = true;

	//Configure UART
	UART_Init(UART_HC05, &uart_config, UART_CLK_FREQ_HC05);
	UART_TransferCreateHandle(UART_HC05, &g_uartPCHandle_hc05, HC05_callback, NULL);
	UART_TransferStartRingBuffer(UART_HC05, &g_uartPCHandle_hc05, g_rxRingBuffer_hc05, RX_RING_HC05_BUFFER_SIZE);
}

void HC05_write(uint8_t *data, size_t length)
{
	UART_WriteBlocking(UART_HC05, data, length);
}

void HC05_read(uint8_t *data, size_t length)
{
	g_rxbuffer_hc05.data = data;
	g_rxbuffer_hc05.dataSize = length;
	UART_TransferReceiveNonBlocking(UART_HC05, &g_uartPCHandle_hc05, &g_rxbuffer_hc05, &g_receivedBytes_hc05);
}

uint8_t HC05_get_flag(void)
{
	return(g_rx_flag_hc05);
}

void HC05_clear_flag(void)
{
	g_rx_flag_hc05 = false;
}

void HC05_print_time(time_store_t time)
{

}

void HC05_print_date(date_store_t date)
{

}
